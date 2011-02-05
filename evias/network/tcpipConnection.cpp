#include "tcpipConnection.hpp"

using namespace evias::network;

/**
 * constructor logic
 *
 * create the connection using the TCP/IP protocol.
 * This constructor will connect to any hostname
 * or IP accessible for the hosting PC.
 **/
evias::network::tcpipConnection::tcpipConnection(string name, uint16_t port, uint64_t timeOut)
    : _isUp(false)
    , _socket(0)
{
    memset(&_counter, 0, sizeof(_counter));

    this->bootstrap(name, port, timeOut);
}

/**
 * constructor logic
 *
 * use an existing connection. the second argument is a
 * a descriptor of the connection written in the socket
 **/
evias::network::tcpipConnection::tcpipConnection(SOCKET existingSock, SOCKADDR_IN connDesc)
    : _isUp(true)
    , _socket(existingSock)
    , _counter(connDesc)
{
}

/**
 * destructor logic
 *
 * shutdown the connection.
 **/
evias::network::tcpipConnection::~tcpipConnection()
{
    this->shutdown();
}

/**
 * shutdown logic
 *
 * the connection socket is closed and the packet queue destroyed.
 **/
void evias::network::tcpipConnection::shutdown()
{
    Lock l(_mutex);

    if (_isUp) {
        close(_socket);
        _isUp = false;
    }

    if (! _queue.empty()) {

        for(std::list<netPacket*>::iterator i = _queue.begin() ;
            i != _queue.end() ;
            i++) {

            // retrieve the packet memory allocation
            // and free the memory
            netPacket* p = *i;
            releaseMemory(p);
        }
    }
}

/**
 * getCounterIP
 *
 * return the counterpart IPv4 address.
 **/
string evias::network::tcpipConnection::getCounterIP() const
{
    return string( inet_ntoa(_counter.sin_addr) );
}

/**
 * bootstrap logic
 *
 * set up the connection to a given host name.
 * first the IPv4 is resolved, then the connection
 * is set up.
 **/
int evias::network::tcpipConnection::bootstrap(string host, uint16_t port, uint64_t timeOutMillis)
{
    if (_isUp) {
        this->shutdown();
    }

    hostent* serverInfo = NULL;
    int iRet = -1;

    {
        // protect the process
        Lock l(_mutex);

        // SOCKET setup
#if OS_WINDOWS
        if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
#else
        if ((_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
#endif
            return -1;
        }

        // resolve IPv4
        if ( NULL == (serverInfo = gethostbyname(host.c_str())) ) {

            // error occured
            close(_socket);
            return -2;
        }

        // prepare connection description
        _counter.sin_family = serverInfo->h_addrtype;
        memcpy((char *)& _counter.sin_addr.s_addr,
               serverInfo->h_addr_list[0],
               serverInfo->h_length);
        _counter.sin_port = htons(port);

    } // end of mutex lock

    // setting the socket as non blocking means we can cancel
    // the connection request if it cannot be established.
    this->setCancelable(_socket, true);

    // connection may take some time, protect the program process
    // from freezing
    Lock l(_mutex);

    runChrono chrono;

    do {

        iRet = connect(_socket, (struct sockaddr *)& _counter, sizeof(_counter));

        if(iRet == 0) {
            // connection established

            _isUp = true;
            return RET_SUCCESS;

#if WINDOOF
        } else if(WSAGetLastError() == WSAEISCONN) {
#else
        } else if(errno == EISCONN) {
#endif
            // connection already active

            _isUp = true;
            return RET_SUCCESS;

#if WINDOOF
#else
        } else if(errno == EINPROGRESS) {
            // wait for response by polling
            // the file descriptor

            int socketErr = 0;

            pollfd pfd;
            pfd.fd      = _socket;
            pfd.events  = 0 | POLLOUT;
            pfd.revents = 0;

            // ((uint64_t)(-1)) is parsed as "infinite"
            if ( timeOutMillis == ((uint64_t)(-1)) )
                socketErr = ::poll( &pfd, 1, -1 );
            else
                socketErr = ::poll( &pfd, 1, (int) timeOutMillis );

            if (socketErr == SOCKET_ERROR) {
                close(_socket);
                return -4;
            }

            int result = 0;
            socklen_t len = sizeof(int);
            getsockopt(_socket, SOL_SOCKET, SO_ERROR, &result, &len);

            if (result == 0 || result == EISCONN) {

                _isUp = true;
                return RET_SUCCESS;
            }
            else {
                // connection error

                close(_socket);
                return -5;
            }
#endif

            // There was another error then the retry/in proggress/busy error.
#if OS_WINDOWS
            }
            else if (WSAGetLastError() != WSAEWOULDBLOCK &&
                     WSAGetLastError() != WSAEALREADY &&
                     WSAGetLastError() != WSAEINVAL) {
#else
        }
        else if (errno != EALREADY && errno != EAGAIN) {
#endif
            close(_socket);
            return -3;
        }

    }
    while (chrono.measure() < (((double) timeOutMillis) / 1000.0) );

    // timeout
    return -4;
}

/**
 * doGet
 *
 * \access protected
 *
 * get data as standardized by the TCP/IP protocol.
 * the data is stored in the previously allocated
 * buffer.
 **/
int evias::network::tcpipConnection::doGet(void* buffer, uint32_t bufLen, uint64_t timeoutMillisec)
{
    int readRet = 0;
    uint32_t readLen = bufLen;

    // cast buffer, working with bits
    int8_t* buf = (int8_t*) buffer;

    runChrono timeout;

    do {

        {
            Lock l(_mutex);

#if OS_WINDOWS
            // windows recv() implementation writes chars to the buffer
            readRet = ::recv(_socket, (char*) buf, readLen, 0);
            if (readRet == SOCKET_ERROR && (WSAGetLastError() == WSAEINTR ||
                                            WSAGetLastError() == WSATRY_AGAIN ||
                                            WSAGetLastError() == WSAEWOULDBLOCK)) {

                // needs retry, caught error in gotten data

                if(timeout.measure()*1000.0 > timeoutMillisec)
                    return TIME_OUT;

                continue; // unlocks mutex and tries again
            }
#else
            // linux recv() correctly writes bits to the memory
            readRet = ::recv(_socket, buf, readLen, 0);
            if (readRet < 0 && (
                errno == EINTR  ||
                errno == EAGAIN ||
                errno == EWOULDBLOCK)) {

                // needs retry, caught error in gotten data

                if(timeout.measure()*1000.0 > timeoutMillisec)
                    return TIME_OUT;

                continue; // unlocks mutex and tries again
            }
#endif
        }

        if(readRet <= 0) { // readRet is some error code
            // no more data there

            this->shutdown();
            return readRet;
        }

        // getting some data, so reinit the timeout to
        // be able to have enough time to get some
        // remaining data
        timeout.reset();
        readLen -= readRet;
        buf += readRet;
    }
    while (readLen);

    return RET_SUCCESS;
}

/**
 * _readLine
 *
 * \access protected
 * \internal
 *
 * get data until reading a specified delimiter.
 **/
string evias::network::tcpipConnection::readLine(string delim, uint64_t timeOut)
{
    bool multiDelim = false;
    if (delim.size() > 1)
        multiDelim = true;

    string readLine;
    uint8_t readByte[2] = {0};
    int     delimLen = delim.size();

    while (this->doGet(&readByte[0], 1, timeOut) == RET_SUCCESS) {
        // data available

        readLine += (char*) readByte;

        int delimPos = 0;
        if ( (delimPos = readLine.find(delim)) != string::npos) {
            // delimiter found, we can safely end

            return readLine.substr(0, ((int)delimPos-1));
        }
    }

    return readLine; // time out or connection lost. delimiter not gotten
}

/**
 * _parseInternalPacket
 *
 * \access protected
 * \internal
 *
 * this method reads a packet either from queue or from connection.
 * packets parsed by this method must contain a specific header
 * 'EVIAS'.
 **/
netPacket* evias::network::tcpipConnection::_parseInternalPacket(bool useQueue, uint64_t timeOut)
{

    // First, check the queue if wanted.
    if (useQueue) {
        netPacket* packet = this->searchQueue();
        if (packet)
            return packet;
    }

    if(!_isUp) { // not connected
        return NULL;
    }

    int socketErr = 0;

    runChrono timeout;

#if OS_WINDOWS
    fd_set filedesc;
    timeval tv = {0, (size_t)timeOut*1000};

    FD_ZERO( &filedesc );
    FD_SET( _socket, &filedesc );

    if(timeOut == ((uint64_t)(-1)))
        // no timeout configured, wait infinitely for
        // packet download
        socketErr = ::select(1, &filedesc,NULL, NULL, NULL);
    else
        socketErr = ::select(1, &filedesc,NULL, NULL, &tv);
#else
    do {

        pollfd pfd;
        pfd.fd = _socket;
        pfd.events = 0 | POLLIN;
        pfd.revents = 0;

        if(timeOut == ((uint64_t)(-1)))
            socketErr = ::poll( &pfd, 1, -1 );
        else
            socketErr = ::poll( &pfd, 1, (int)(timeOut) );
    }
    while( socketErr == SOCKET_ERROR && errno == EINTR );
#endif

    if( socketErr == SOCKET_ERROR ) { // errno is no more EINTR, transfer over
        this->shutdown();
        return NULL;
    }

    if( socketErr == 0 || timeout.measure()*1000.0 > timeOut ) {
        return NULL;
    }

    // we have a valid file descriptor
    // now we need to filter the packets,
    // searching for a special header
    // 'eVias'

    int8_t buf;

    while (true) { // XXX

        // needs to fetch each character of the header
        // in order to process the header.
        // first needs to get the starting E, then
        // continue fetching every character in the same order ..

        do {
            if (RET_SUCCESS != this->doGet(&buf, 1, (uint64_t)(timeOut - 1000.0*timeout.measure()))) {
                // data get interruption
                return NULL;
            }
        }
        while(buf != 'E');

        if(RET_SUCCESS != this->doGet(&buf, 1, (uint64_t)(timeOut - 1000.0*timeout.measure()))) {
            // data get interruption
            return NULL;
        }

        if(buf != 'V')
            continue;

        if(RET_SUCCESS != this->doGet(&buf, 1, (uint64_t)(timeOut - 1000.0*timeout.measure()) )) {
            // data get interruption
            return NULL;
        }

        if(buf != 'I')
            continue;

        if(RET_SUCCESS != this->doGet(&buf, 1, (uint64_t)(timeOut - 1000.0*timeout.measure()) )) {
            // data get interruption
            return NULL;
        }

        if(buf != 'A')
            continue;

        if(RET_SUCCESS != this->doGet(&buf, 1, (uint64_t)(timeOut - 1000.0*timeout.measure()) )) {
            // data get interruption
            return NULL;
        }

        if(buf != 'S')
            continue;
        break;
    }

    // We already got the "EVIAS" header, now get the rest of the header.

    netPacket* pack = new netPacket(EVIAS_MSG_NULL);
    socketErr = this->doGet(&pack->m_dataPointer[5], sizeof(__packetHeader_t)-5, (uint64_t)(timeOut - 1000.0*timeout.measure()) );

    if(socketErr != RET_SUCCESS)
        return NULL;

    // packet header gotten
    // get the packet data

    if (pack->getPayloadLen() <= 0) {
        // no data can be gotten

        releaseMemory(pack);
        return NULL;
    }

    // tcpipConnection class is friend of netPacket class
    pack->m_dataPointer = (int8_t*)realloc(pack->m_dataPointer,pack->getTotalLen());

    //assert(pack->m_dataPointer != NULL); // make sur we have no data
    if (pack->m_dataPointer != NULL)
        return NULL;

    // get the packet
    socketErr = this->doGet(&pack->m_dataPointer[sizeof(__packetHeader_t)], pack->getPayloadLen(), (uint64_t)(timeOut - 1000.0*timeout.measure()));
    if(socketErr != RET_SUCCESS)
        return NULL;

    if(pack->isValid()) {
        // validated packet ID

        return pack;
    }

    // gotten packet was not recognized
    // no EVIAS header present or ID invalid
    releaseMemory(pack);
    return NULL;
}

/**
 * recvPacket
 *
 * this function waits for a packet to be received. The packet must
 * contain a EVIAS header.
 *
 * \note 0 timeout means infinity as well as ((uint64_t)(-1))
 *
 **/
netPacket* evias::network::tcpipConnection::recvPacket(bool useQueue, uint64_t timeOut)
{
    return this->_parseInternalPacket(useQueue, timeOut == 0 ? ((uint64_t)(-1)) : timeOut);
}

netPacket* evias::network::tcpipConnection::recvPacket(bool useQueue)
{
    return this->_parseInternalPacket(useQueue, 0);
}

/**
 * recvPacketByID
 *
 * receive a packet from queue or network. Will fetch
 * packets until the received packet header corresponds
 * to the request identification.
 *
 **/
netPacket* evias::network::tcpipConnection::recvPacketByID(__netRequest_t request, uint64_t timeOut)
{
    // argument validation
    if (request == EVIAS_MSG_NONE || request > EVIAS_MSG_MAX)
        return NULL;

    // as the queue may not be checked in the fetch loop
    // we need to first look in the queue manually.
    // checking the queue in the fetch loop would result
    // in an infinite recursion due to the queue element
    // push done in the recvPacket function.

    // check for packet presence in queue
    netPacket* packet = this->searchQueue(request);
    if (packet)
        return packet;

    // network data get

    do {
        // receive packets while the requested one has not
        // been gotten.

        packet = this->recvPacket(false, timeOut);

        if (! packet)
            return NULL;

        if(((__packetHeader_t*)packet->m_dataPointer)->requestId == request)
            return packet;

        this->pushQueue(packet);
    }
    while(true);

    return NULL;
}

/**
 * use the other recvPacket method for getting the
 * packet. time out is not used
 **/
netPacket* evias::network::tcpipConnection::recvPacketByID(__netRequest_t request)
{
    // arguments validation
    if (request == EVIAS_MSG_NONE || request > EVIAS_MSG_MAX)
        return NULL;

    netPacket* packet = this->searchQueue(request);
    if(packet)
        return packet;

    while((packet = this->recvPacket(true)) != NULL) {
        // packet fetch done

        if (((__packetHeader_t*)packet->m_dataPointer)->requestId == request)
            return packet;

        this->pushQueue(packet);
    }

    return NULL;
}

/**
 * send
 *
 * send data over the network. the size sent
 * is defined by the second argument.
 **/
int evias::network::tcpipConnection::_send(const void* dataPointer, uint32_t dataLen)
{
    if (! _isUp)
        return (int) -1;

    errno = 0;

    int sizeSent = 0;
    uint32_t sizeSend = dataLen;

    // work with bits for sending data over the network
    const int8_t* buf = (const int8_t*) dataPointer;

    Lock l(_mutex);

    do {
        // send data while possible and needed

#if OS_WINDOWS
        // windows send() implementation requires
        // char representation of the bits.
        sizeSent = ::send(_socket, (const char*) buf, sizeSend, 0);
        if(sizeSent == SOCKET_ERROR && (WSAGetLastError() == WSAEINTR ||
                                        WSAGetLastError() == WSATRY_AGAIN ||
                                        WSAGetLastError() == WSAEWOULDBLOCK)) {
#else
        // linux, is better ;)
        sizeSent = ::send(_socket, buf, sizeSend, 0);
        if (sizeSent < 0 && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)) {
#endif

            continue;
        }

        if(sizeSent < 0)
            return -1;

        sizeSend -= sizeSent;
        buf += sizeSent;
    }
    while(sizeSend > 0);

    return RET_SUCCESS;
}

/**
 * higher level send() implementation using netPacket
 * class. Easy usability.
 **/
int evias::network::tcpipConnection::send(netPacket* packet)
{
    if (!_isUp || packet == NULL)
        return -1;

    if (this->_send(packet->m_dataPointer, packet->getTotalLen()) != RET_SUCCESS)
        return -1;

    return RET_SUCCESS;
}

/**
 * get
 *
 * send a request packet to the server on the current
 * connection.
 *
 * \return
 *  -1 not connected
 *  -2 request error
 *  -3 output error
 *  -4 input error
 *  -5 wrong response
 *
 * \note works the ioPacket argument as it first represents
 *       the request packet then potentially the output
 *       packet.
 **/
int evias::network::tcpipConnection::get(netPacket* ioPacket, uint64_t timeOut)
{
    if (!_isUp)
        return -1;

    __netRequest_t request;

    request = ioPacket->getType();

    if (request == EVIAS_MSG_NULL || request == EVIAS_MSG_NONE || request > EVIAS_MSG_MAX ) {
        return -2;
    }

    if (this->send(ioPacket) != RET_SUCCESS) {
        return -3;
    }

    netPacket *packet = this->recvPacket(request, timeOut);
    if (packet == NULL)
        return -4;

    if (packet->getType() != request) {
        // wrong response packet gotten

        releaseMemory(packet);
        return -5;
    }

    // got the right packet
    // can now change the packet pointer
    // the packet will point to the received buffer

    releaseAlloc(ioPacket->m_dataPointer);

    ioPacket->m_dataPointer = packet->m_dataPointer;

    packet->m_dataPointer = NULL;
    releaseMemory(packet);

    ioPacket->rewind(); // set back read pointer
    return RET_SUCCESS;
}

/**
 * setCancelableSocket
 *
 * set a socket's blocking byte or remove it
 **/
int evias::network::tcpipConnection::setCancelable(SOCKET socket, bool canCancel)
{
#if OS_WINDOWSi
    u_long mode = canCancel ? 1 : 0;

    return ioctlsocket(socket, FIONBIO, &mode);
#else
    int flags;

    if(! canCancel)
        if((flags = fcntl(socket, F_GETFL, 0)) < 0 ||
            fcntl(socket, F_SETFL, flags & (~O_NONBLOCK)) < 0)
            return -1;
    else
        if((flags = fcntl(socket, F_GETFL, 0)) < 0 ||
            fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
            return -1;

    return 0;
#endif
}

