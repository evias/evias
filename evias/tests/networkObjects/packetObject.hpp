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

#include "../../network/packet.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace networkObjects {

    using namespace evias::network;

    class packetObject : public unitTest
    {
        public :

            packetObject () : unitTest () { };

            ~packetObject () {};

            inline void prepare ()
            {
                // will create header for msg HELLO
                // and initialize packet info
                _packetObj = new netPacket(EVIAS_MSG_HELLO);

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                string data = "i'm saying hello through a packet.";
                string header = "EVIAS";

                _packetObj->append(data);

                string readTo = "";
                string readHdr = "";

                readTo = _packetObj->readString();
                readHdr= _packetObj->readHeaderString();

                if (readHdr != header) {
                    _returnMsg = "packet object returns wrong header. [return: '" + readHdr + "']";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (readTo != data) {
                    _returnMsg = "packet object returns wrong data. [return: '" + readTo + "']";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // packet correctly built

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                if (_packetObj != NULL)
                    delete _packetObj;

                return _returnCode;
            }

        private :

            netPacket*  _packetObj;
    };

}; // end namespace configFiles

}; // end namespace test

}; // end namespace core

}; // end namespace evias

