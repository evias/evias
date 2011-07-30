/**
 * Package : eVias unitary test suite
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../../application/irc.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace networkObjects {

    // client hooks

    using evias::application::__ircReplyData;
    using evias::application::Irc;

    typedef int (* hookFunction) (char*, __ircReplyData*, void*);

    int catchPrivateMessage(char*, __ircReplyData*, void*);

    class ircClient : public unitTest
    {
        public :

            ircClient () : unitTest () { };

            ~ircClient () {};

            inline void prepare ()
            {
                _host   = "irc.freenode.net";
                _port   = 6667;
                _nick   = "eViasBot";
                _user   = "GregSaive";
                _name   = "eVias Open Development";
                _pass   = "";

                _client = new Irc;
                _client->setQuietMode(false);
                
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                _client->start(
                    (char*)(_host.c_str()), _port,
                    (char*)(_nick.c_str()),
                    (char*)(_user.c_str()),
                    (char*)(_name.c_str()),
                    (char*)(_pass.c_str())
                );

                if (! _client->isConnected()) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                if (_client != NULL)
                    delete _client;

                return _returnCode;
            }

        private :

            Irc*   _client;
            
            string _host;
            int    _port;
            string _nick;
            string _user;
            string _name;
            string _pass;
    };

}; // end namespace networkObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

