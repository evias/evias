#ifndef __EVIAS_NETWORK_NS_SOAP_SERVER__
#define __EVIAS_NETWORK_NS_SOAP_SERVER__

#include <string>
#include <map>
#include <vector>

#include "soap_method.hpp"
#include "soap_handler.hpp"
#include "soap_fault.hpp"
#include "soap_result.hpp"
#include "soap_request.hpp"

namespace evias {

namespace network {

    class soapMethod;
    class soapHandler;
    class soapFault;
    class soapResult;
    class soapRequest;

    using namespace std;

    enum serverPersistenceMode {
        PERSISTENCE_REQUEST = 1,
        PERSISTENCE_SESSION = 2
    };

    class soapServer
    {
        public :

            soapServer(string = "", string = "");
            soapServer(const soapServer& rhs);

            ~soapServer();

            void setHandler(string handlerClass);
            void setHandler(soapHandler*);

            void setPersistence (serverPersistenceMode);

            vector<soapMethod*> getFunctions();

            soapMethod* addFunction (const soapMethod&);
            soapFault*  fault(string code, string message, string = "", string = "", string = "");

            soapResult* handle(soapRequest*);

        private :

            string  m_host;
            string  m_wsdl;

            map<string, soapMethod*>    m_methods;
    };

}; // network

}; // evias

#endif
