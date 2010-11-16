#ifndef __EVIAS_NETWORK_NS_SOAP_FAULT__
#define __EVIAS_NETWORK_NS_SOAP_FAULT__

#include <string>

namespace evias {

namespace network {

    using namespace std;

    class soapFault
    {
        public :

            soapFault(string = "", string = "");

            ~soapFault();

        private :

            string m_code;
            string m_message;
    };

}; // network

}; // evias

#endif

