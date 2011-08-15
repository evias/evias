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

    class commentedFile : public unitTest
    {
        public :

            commentedFile () : unitTest () { };

            ~commentedFile () {};

            inline void prepare ()
            {
                if (_options.size() != 1) {
                    _returnMsg = "configFile::commentedFile test expects 1 option to be set.";
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

                // the configuration file has two sections (workSection & workSection2) and
                // one commented section. The first has 3 entries, the second has no.
                // check if comments work without messing up the entries attachments.

                iniSection sectionOne = _configObj->getSection("workSection");
                iniSection sectionTwo = _configObj->getSection("workSection2");
                iniSection sectionOut = _configObj->getSection("commentedSection");

                assertable<int>::assertEqual(sectionOne.getPairs().size(), 3);
                assertable<int>::assertEqual(sectionTwo.getPairs().size(), 0);
                assertable<string>::assertEqual(sectionTwo.getLabel(), "workSection2");

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

