#ifndef EVIAS_NETWORK_NS_NETFUNCTIONS
#   define EVIAS_NETWORK_NS_NETFUNCTIONS

#include "tcpipConnection.hpp"

#include "../core/base_container.hpp"
#include "packet.hpp"

namespace evias {

namespace network {

    using evias::core::baseContainer;

    typedef enum {
        ERR_NOT_CONNECTED   = -1, // connection is down
        ERR_SOCKET          = -2, // socket error
        ERR_TIMEOUT         = -3, // request timed out
        ERR_RESPONSE        = -4, // invalid response
        ERR_REQUEST         = -5, // invalid request
        ERR_STATUS          = -6, // responseStatus code relative
        ERR_SHUTDOWN        = -7,
        ERR_INPUT           = -8, // packet received is invalid
        ERR_OUTPUT          = -9  // packet send is invalid
    } __networkErrors;

    int             doHttpGetRequest(baseContainer&, string, string, uint64_t);
    baseContainer*  doHttpGetRequest(string, string, uint64_t);
    int             doHttpDownload  (string, string, string, uint64_t);

}; // network

}; // evias

#endif /** EVIAS_NETWORK_NS_NETFUNCTIONS **/
