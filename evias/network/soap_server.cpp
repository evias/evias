#include "soap_server.hpp"

namespace evias {

namespace network {

    soapServer::soapServer(string host, string wsdl)
    {

    }

    soapServer::soapServer(const soapServer& rhs)
    {

    }

    soapServer::~soapServer()
    {

    }

    soapMethod* soapServer::addFunction(const soapMethod& methodObj)
    {
        m_methods.insert(make_pair(methodObj.getName(), new soapMethod(methodObj)));

        return (new soapMethod(methodObj));
    }

    void soapServer::setHandler(string handlerClass)
    {

    }

    void soapServer::setHandler(soapHandler* handler)
    {

    }

    void soapServer::setPersistence (serverPersistenceMode mode)
    {

    }

    vector<soapMethod*> soapServer::getFunctions()
    {
        vector<soapMethod*> methodsVec;
        map<string, soapMethod*>::iterator currentFunc = m_methods.begin();
        for ( ; currentFunc != m_methods.end(); currentFunc++) {

            methodsVec.push_back ((*currentFunc).second);
        }

        return methodsVec;
    }

    soapFault* soapServer::fault(string code, string message, string actor, string details, string name)
    {
        soapFault* faultHandler = new soapFault(code, message);

        return faultHandler;
    }

    soapResult* soapServer::handle(soapRequest* request)
    {
        soapResult* result = new soapResult();

        return result;
    }

}; // network

}; // evias

