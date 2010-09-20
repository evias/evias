/**
 * Package: 	iSchool Core Unit Testing
 *
 * Implemented Namespaces:
 * 	- evias::core::test
 *
 * Implemented Classes:
 *  - configFileParse
 *
 * Code Description:
 *  - tests configIni's parse method
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

    class configFileParse : public unitTest
    {
        public :

            configFileParse () : unitTest () { };

            ~configFileParse ();

            inline int prepare (vector<string> params)
            {
                if (params.size() != 1) {
                    cout    << "The prepare method has to be called with 1 parameter for this test." << endl;

                    return -1;
                }

                vector<string>::iterator it = params.begin();

                _configObj = new configIni ((*it));

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     <<"--- Will now parse ini file." << endl;

                _configObj->parse();

                debugIni* debugger = new debugIni (_configObj);

                debugger->vs_linesDump();
                debugger->vs_sectionsDump();
                debugger->mss_pairsDump();

                cout << "--- After ini parse" << endl
                     << endl
                     << "[RESULT]" << endl
                     << "-- Lines count : " << _configObj->getLinesCount() << endl
                     << "-- Sections count : " << _configObj->getSectionsCount() << endl
                     << "-- Pairs count : " << _configObj->getPairsCount() << endl
                     << "[TLUSER]" << endl;

                cout << endl
                     << "--- Will now change key2 value to [" << _configObj->getValue("Section1", "key2") << "1"
                     << endl;

                _configObj->changeValue("Section1", "key2", _configObj->getValue("Section1", "key2").append("1"));

                debugger->mss_pairsDump();

                return 1;
            }

            inline int shutdown ()
            {
                if (_configObj != NULL)
                    delete _configObj;

                return 0;
            }

        private :

            configIni*  _configObj;
    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int arcg, char* argv[])
{
    using namespace std;

    evias::core::test::configFileParse* unitTester = new evias::core::test::configFileParse ();

    vector<string> params;

    params.push_back ("/home/greg/srv/home.work/iSchool/devel/tests/bin/config/configFileParse.ini");

    int endCode = unitTester->prepare (params);

    if (endCode == 2) {
        unitTester->execute ();

        endCode = unitTester->shutdown ();
    }

    return endCode;
}
