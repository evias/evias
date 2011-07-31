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

    class ircClient : public unitTest
    {
        public :

            ircClient () : unitTest () { };

            ~ircClient () {};

            inline void prepare ()
            {
                _host   = "irc.freenode.net";
                _port   = 6667;
                _nick1  = "eViasBot";;
                _user   = "GregSaive";
                _name   = "eVias Open Development";
                _pass   = "";

                _client  = new Irc;
                _client->setQuietMode(true);
                
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                _client->start(
                    (char*)(_host.c_str()), _port,
                    (char*)(_nick1.c_str()),
                    (char*)(_user.c_str()),
                    (char*)(_name.c_str()),
                    (char*)(_pass.c_str())
                );

                if (! _client->isConnected()) {
                    return setReturnCode((int) ERROR_ENVIRONMENT);
                }

                // catch until endofmotd (connected status fulfilled)
                _client->catchIt(Irc::MSG_ENDMOTD);

                // Channel JOIN
                string codes = string(Irc::MSG_INVALIDCHAN).append("|").append(Irc::MSG_ENDLIST);

                // test channel join
                _client->channelJoin((char*) "#gregchan");
                _client->catchIt(codes);

                if (_client->getLastTreatedCommand() != Irc::MSG_ENDLIST) {
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

                // Channel PART
                codes = string(Irc::MSG_CHANPART).append("|")
                        .append(Irc::MSG_NOTONCHAN).append("|")
                        .append(Irc::MSG_INVALIDCHAN);

                // test valid channel part
                _client->channelPart((char*) "#gregchan");
                _client->catchIt(codes);

                if (_client->getLastTreatedCommand() != Irc::MSG_CHANPART) {
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

                // test "notonchan" channel part
                _client->channelPart((char*) "#whocares");
                _client->catchIt(codes);

                if (_client->getLastTreatedCommand() != Irc::MSG_NOTONCHAN) {
                    return setReturnCode((int) ERROR_ENVIRONMENT);
                }

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                if (_client != NULL)
                    delete _client;

                if (_client2 != NULL)
                    delete _client2;

                return _returnCode;
            }

        private :

            Irc*    _client;
            Irc*    _client2;
            
            string _host;
            int    _port;
            string _nick1;
            string _nick2;
            string _user;
            string _user2;
            string _name;
            string _name2;
            string _pass;
    };

}; // end namespace networkObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

