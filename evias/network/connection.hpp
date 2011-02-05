#ifndef EVIAS_NETWORK_NS_CONNECTION
#   define EVIAS_NETWORK_NS_CONNECTION

#include "../core/defines.h"
#include "../core/threading.hpp"
#include "packet.hpp"

#if OS_WINDOWS
#  include <Winsock2.h>
inline void close(SOCKET sock)
{
    closesocket(sock);
    return (void);
}
#else
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <poll.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <sys/types.h>
#  include <resolv.h>
#  include <netdb.h>
#  include <fcntl.h>
#endif

#include <cerrno>
#include <list>

// defining those makes it easier working with the standard
// functions provided by windows os's.
#define SOCKET      int
#define SOCKADDR_IN sockaddr_in
#ifndef SOCKET_ERROR
#  define SOCKET_ERROR -1
#endif

#define TIME_OUT  3600
#define QUEUE_MAX 32

namespace evias {

namespace network {

    using evias::core::Mutex;
    using evias::core::Lock;

    /**
     * evias::network::netConnection
     *
     * object for creating socket using connections. These may send and receive
     * packets, each of which will be at 32bits of size, or less.
     **/
    class netConnection
    {
    public:
        virtual ~netConnection() {};

        typedef enum {
            TRADITIONAL  = 0,
            ONDEMAND_CLI = 1,
            ONDEMAND_SRV = 2
        } __connectionType;

    /** abstracted public methods **/

        virtual void shutdown() = 0;
        virtual bool isUp() = 0;

        virtual __connectionType getType() const = 0;

        virtual string getCounterIP() const = 0;

        virtual int send(netPacket*) = 0;
        virtual int get(netPacket*, uint64_t = TIME_OUT) = 0;

        // wait for packet
        virtual netPacket* recvPacket(bool = true, uint64_t = TIME_OUT) = 0;
        // get existing packet
        virtual netPacket* recvPacket(bool = true) = 0;

    protected:
        std::list<netPacket*>   _queue;  // packet queue
        Mutex                   _mutex; // Mutex for keeping a healthy process

        netConnection() {};

    /** abstracted protected methods **/

        virtual netPacket*  searchQueue (__netRequest_t = EVIAS_MSG_NONE);
        virtual void        pushQueue   (netPacket*);
    };

}; // network

}; // evias

#endif /** EVIAS_NETWORK_NS_CONNECTION */

