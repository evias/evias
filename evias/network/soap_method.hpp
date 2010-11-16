#ifndef __EVIAS_NETWORK_NS_SOAP_METHOD__
#define __EVIAS_NETWORK_NS_SOAP_METHOD__

#include <string>

namespace evias {

namespace network {

    using namespace std;

    class soapMethod
    {
        public :

            soapMethod (string functionName, string functionDef);

            ~soapMethod();

            string getName() const { return m_name; };

        private :

            string m_name;
    };

}; // network

}; // evias

#endif

