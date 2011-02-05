#ifndef EVIAS_NETWORK_NS_TCPIPCONNECTION
#   define EVIAS_NETWORK_NS_TCPIPCONNECTION

#include <string>

#include "connection.hpp"
#include "../core/base_container.hpp"
#include "../core/run_chrono.hpp"

namespace evias {

namespace network {

    using std::string;
    using evias::core::baseContainer;
    using evias::core::runChrono;

    class tcpipConnection : public netConnection {

    friend int evias::network::doHttpGetRequest( baseContainer&, string, string, uint64_t );

    public:
        tcpipConnection(string, uint16_t, uint64_t);
        tcpipConnection(SOCKET, SOCKADDR_IN);

        virtual ~tcpipConnection();

        // the _bootstrap function sets up the connection
        int                 bootstrap( string, uint16_t, uint64_t );
        virtual void        shutdown();
        bool                isUp() { return _isUp; };

        __connectionType getType() const {
            return TRADITIONAL;
        };

        virtual string getCounterIP() const;

        virtual int send(netPacket*);
        virtual int get(netPacket*, uint64_t = TIME_OUT);

        virtual netPacket* recvPacket(bool, uint64_t);
        virtual netPacket* recvPacket(bool = true);

        virtual netPacket* recvPacketByID(__netRequest_t, uint64_t);
        virtual netPacket* recvPacketByID(__netRequest_t);

        static int setCancelable(SOCKET, bool = true);

        virtual string      readLine( string, uint64_t = TIME_OUT);
        virtual int         doGet( void *, uint32_t, uint64_t );

    protected:

        virtual netPacket*  _parseInternalPacket( bool = true, uint64_t = TIME_OUT);

        virtual int         _send(const void *, uint32_t in_uiLen);

        bool            _isUp;
        SOCKET          _socket;
        SOCKADDR_IN     _counter;

        unsigned long   _callTimestamp;

    };

}; // network

}; // evias

#endif /** EVIAS_NETWORK_NS_TCPIPCONNECTION **/
