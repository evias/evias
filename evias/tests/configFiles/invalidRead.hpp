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

    class invalidRead : public unitTest
    {
        public :

            invalidRead () : unitTest () { };

            ~invalidRead () {};

            inline void prepare ()
            {
                if (_options.size() != 1) {
                    _returnMsg = "configFile::invalidRead test expects 1 option to be set.";
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

                // configuration file has two sections, one called Names, the other one
                // called Addresses. first get a valid entry and test it's value.
                // then try and read an invalid section:key and test the result.

                vector<string> validKeys;
                validKeys.push_back("firstname");
                validKeys.push_back("second_firstname");
                validKeys.push_back("third_firstname");
                validKeys.push_back("lastname");

                map<string,string> readData = _configObj->getValues("Names", validKeys);

                assertable<int>::assertEqual(readData.size(), 4);

                vector<string> oneInvalid;
                oneInvalid.push_back("i_am_not_in_the_configuration_file");

                string singleGetVal             = _configObj->getValue("Addresses", oneInvalid.at(0));
                map<string,string> multiGetVal  = _configObj->getValues("Addresses", oneInvalid);

                string expectedReturn = "";

                typedef map<string,string>::iterator __findType;

                assertable<string>::assertEqual(singleGetVal, expectedReturn);
                assertable<__findType>::assertNotEqual(multiGetVal.find("i_am_not_in_the_configuration_file"), multiGetVal.end());
                assertable<string>::assertEqual(multiGetVal["i_am_not_in_the_configuration_file"], string(""));

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

