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

#include "core/config_ini.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace configFiles {

    class objectWrite : public unitTest
    {
        public :

            objectWrite () : unitTest () { };

            ~objectWrite () {};

            inline void prepare ()
            {
                if (_options.size() != 1) {
                    _returnMsg = "configFile::objectWrite test expects 1 option to be set.";
                    setReturnCode((int) ERROR_DEVELOPMENT);
                    return ;
                }

                vector<string>::iterator it = _options.begin();

                _configObj = new configIni ((*it));

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                _configObj->parse();

                assertable<bool>::assertEqual(_configObj->state(), true);

                // configuration file has a section named 'updateSection'
                // containing a key name "updateKey" of which the value
                // will be appended of a '1' for each execution.

                string successVal = _configObj->getValue("updateSection", "updateKey").append("1");

                _configObj->changeValue("updateSection", "updateKey", successVal);

                assertableString<const char*>::assertEqual(
                    _configObj->getValue("updateSection", "updateKey").c_str(),
                    successVal.c_str()
                );

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                if (_configObj != NULL)
                    delete _configObj;

                return _returnCode;
            }

        private :

            configIni*  _configObj;
    };

}; // end namespace configFiles

}; // end namespace test

}; // end namespace core

}; // end namespace evias

