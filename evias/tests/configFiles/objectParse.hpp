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

#include "../../core/config_ini.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace configFiles {

    class objectParse : public unitTest
    {
        public :

            objectParse () : unitTest () { };

            ~objectParse () {};

            inline void prepare ()
            {
                if (_options.size() != 1) {
                    _returnMsg = "configFile::objectParse test expects 1 option to be set.";
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

                // configuration file for test should have 2 sections
                // each of which should contain 2 key declarations.

                assertable<int>::assertEqual(_configObj->getIniSections().size(), 2);

                vector<iniSection> sections     = _configObj->getIniSections();
                vector<iniSection>::iterator it;
                for (it = sections.begin(); it != sections.end(); it++)
                    assertable<int>::assertEqual((*it).getPairs().size(), 2);

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

