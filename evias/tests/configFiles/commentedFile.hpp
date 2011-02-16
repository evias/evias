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

                if (! _configObj->state()) {
                    _returnMsg = "bad object state after file parse.";
                    return setReturnCode((int) ERROR_DATA_INPUT);
                }

                // the configuration file has two sections (workSection & workSection2) and
                // one commented section. The first has 3 entries, the second has no.
                // check if comments work without messing up the entries attachments.

                iniSection sectionOne = _configObj->getSection("workSection");
                iniSection sectionTwo = _configObj->getSection("workSection2");
                iniSection sectionOut = _configObj->getSection("commentedSection");

                if (sectionOne.getPairs().size() != 3) {
                    _returnMsg = "configuration file for configFiles::commentedFile should define 3 keys for section 'workSection'.";
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

                if (sectionTwo.getPairs().size() != 0) {
                    _returnMsg = "configuration file for configFiles::commentedFile should define 2 commented lines for section 'workSection2'.";
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

                if (sectionOut.getLabel() != "invalid") {
                    _returnMsg = "the label of the commented section should be set to invalid, as getSection() is failing.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

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

