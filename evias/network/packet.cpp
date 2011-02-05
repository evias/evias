#include "packet.hpp"

using namespace evias::network;

/**
 * constructor logic
 *
 * fill header, set pointer after header
 **/
evias::network::netPacket::netPacket(__netRequest_t requestType)
{
    // alloc header size
    m_dataPointer = (int8_t *) calloc(1,HEADER_SIZE);
    memset(m_dataPointer, 0, HEADER_SIZE);

    // fill header
    fillHeader((__packetHeader_t*) m_dataPointer, requestType);

    // set request id
    this->setType(requestType);

    // set data pointer after header
    this->rewind();
}

/**
 * release memory
 **/
evias::network::netPacket::~netPacket()
{
    releaseAlloc(m_dataPointer);
}

bool evias::network::netPacket::isValid() const
{
    return headerValid((__packetHeader_t *)m_dataPointer);
}

/**
 * set the request type
 **/
netPacket* evias::network::netPacket::setType(__netRequest_t requestType)
{
    ((__packetHeader_t*) m_dataPointer)->requestId = requestType;

    return this;
}

/**
 * get request type id
 **/
__netRequest_t evias::network::netPacket::getType() const
{
    return ((__packetHeader_t*) m_dataPointer)->requestId;
}

/**
 * set cursor to begin of data (after header)
 **/
netPacket* evias::network::netPacket::rewind()
{
    _curPos = HEADER_SIZE;

    return this;
}

/**
 * append string data to buffer and set
 * cursor to point behind the data.
 **/
netPacket* evias::network::netPacket::append(string& in)
{
    size_t iLen =  in.length( ) + _curPos + 1;
    m_dataPointer = (int8_t*) realloc((void*)m_dataPointer, iLen);
    if (in.c_str() == NULL)
        // \0 terminating string
        m_dataPointer[_curPos] = 0;
    else
        strcpy((char *)&m_dataPointer[_curPos], in.c_str());

    _curPos += in.length( ) + 1;
    ((__packetHeader_t*)m_dataPointer)->dataSize = _curPos - HEADER_SIZE;

    return this;
}

/**
 * append C style "anything" data to the current packet
 * buffer. the size of the data appended has to be set
 * through the second argument
 **/
netPacket* evias::network::netPacket::append(const void* dataPointer, uint32_t dataSize)
{
    uint32_t iLen = dataSize + _curPos + 1;
    m_dataPointer = (int8_t*) realloc((void*) m_dataPointer, iLen);

    if (dataPointer == NULL) {
        // \0 terminating string
        m_dataPointer[_curPos] = 0;
        _curPos++;
    }
    else {
        memcpy(&m_dataPointer[_curPos], dataPointer, dataSize);
        _curPos += dataSize;
    }

    ((__packetHeader_t*)m_dataPointer)->dataSize = _curPos - HEADER_SIZE;

    return this;
}

/**
 * readString
 *
 * get the character representation of the current pointer
 * position.
 **/
string evias::network::netPacket::readString()
{
    size_t len = this->getTotalLen();
    if(_curPos >= len)
        return "";

    string ret = string((char*)& m_dataPointer[_curPos]);

    _curPos += ret.length() + 1;
    return ret;
}

/**
 * extractString
 *
 * get a character representation of the current pointer
 * cursor position and remove it from message.
 **/
string evias::network::netPacket::extractString()
{
    size_t len = this->getTotalLen();
    if(_curPos >= len)
        return "";

    string ret = string((char*)& m_dataPointer[_curPos]);

    // copy the new data
    int8_t* newData = (int8_t*) calloc(1,len - (ret.length() + 1));
    memcpy(newData, m_dataPointer, _curPos);
    memcpy(&newData[_curPos], &m_dataPointer[_curPos+ret.length()+1], len - ret.length() - 1 - _curPos);

    // decrement dataSize
    ((__packetHeader_t*) newData)->dataSize -= ret.length() + 1;

    releaseAlloc(m_dataPointer);
    m_dataPointer = newData;

    return ret;
}

/**
 * get binary data of the current pointer cursor position. If you
 * specify a length of 0, the get will work until reached the end
 * of the packet.
 *
 * oDataPointer
 * length
 *
 * \return int
 **/
int evias::network::netPacket::read(void *oDataPointer, uint32_t length)
{
    uint32_t maxLen = this->getTotalLen() - _curPos;

    // get the correct length to fetch
    maxLen = length == 0 ? maxLen : std::min(maxLen, length);

    // copy data
    memcpy(oDataPointer, &m_dataPointer[_curPos], maxLen);

    _curPos += maxLen;

    return RET_SUCCESS;
}

/**
 * get the total packet size, including packet header.
 **/
uint32_t evias::network::netPacket::getTotalLen() const
{
    return ((__packetHeader_t*)m_dataPointer)->dataSize + sizeof(__packetHeader_t);
}

/**
 * get size of payload.
 **/
uint32_t evias::network::netPacket::getPayloadLen( void ) const
{
    return ((__packetHeader_t*)m_dataPointer)->dataSize;
}

/**
 * write the current netPacket instance to the current
 * cursor position of the given output packet.
 **/
int evias::network::netPacket::write(netPacket *oPacket)
{
    // request type + payload
    oPacket->append(this->getType());
    oPacket->append(this->getPayloadLen());

    // insert data
    oPacket->append(this->getData(), this->getPayloadLen());
    return RET_SUCCESS;
}

/**
 * retrieve a packet from another one.
 * the ident has to be regenerated.
 **/
int evias::network::netPacket::read(netPacket* packet)
{
    releaseAlloc(m_dataPointer);

    __netRequest_t type = EVIAS_MSG_NULL;
    uint32_t payloadLen = 0;

    // initialize packet
    packet->read(type);
    packet->read(payloadLen);

    // initialize memory
    m_dataPointer = (int8_t*) calloc(1, HEADER_SIZE + payloadLen);
    memset(m_dataPointer, 0, HEADER_SIZE + payloadLen);
    m_dataPointer[0] = 'E';
    m_dataPointer[1] = 'V';
    m_dataPointer[2] = 'I';
    m_dataPointer[3] = 'A';
    m_dataPointer[4] = 'S';

    this->setType(type);

    packet->read(this->getData(), payloadLen);
    ((__packetHeader_t *) m_dataPointer)->dataSize = payloadLen;
    this->rewind();

    return RET_SUCCESS;
}

/**
 * print packet data to file.
 * file must be fopen'ed
 **/
int evias::network::netPacket::print(FILE* fileObj) const
{
    // write header
    int headerWrite = fwrite((__packetHeader_t *) m_dataPointer, sizeof(__packetHeader_t), 1, fileObj);
    if (headerWrite != 1)
        return -1;

    // write data
    int dataWrite = fwrite(this->getReadOnlyData(), sizeof(uint8_t), this->getPayloadLen(), fileObj);
    if (dataWrite != this->getPayloadLen())
        return -2;

    return RET_SUCCESS;
}

