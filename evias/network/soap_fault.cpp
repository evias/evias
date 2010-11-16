#include "soap_fault.hpp"

namespace evias {

namespace network {

    soapFault::soapFault(string code, string msg)
        : m_code (code), m_message (msg)
    {

    }

    soapFault::~soapFault()
    {

    }

}; // network

}; // evias

