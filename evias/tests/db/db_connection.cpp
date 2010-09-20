#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "core/db_adapter.hpp"
#include "core/unit_test_abstract.hpp"

using namespace std;

namespace evias {

namespace core {

namespace test {

    class dbConnection : public unitTest
    {
        public :
            dbConnection () : unitTest () {};
            ~dbConnection() {};

            int prepare (vector<string> params) {
                // initialize unit test

                if ( params.size() != 4 ) {
                    _printMessage = "You need to call dbConnection::prepare with 4 parameters in the vector.";

                    _state = false;

                    return 2;
                }
                else if (params.at(0) == "database_name" || params.at(3) == "host") {
                    _printMessage = "You need to edit the dbConnection in order to connect to the right server / database";

                    _state = false;

                    return 2;
                }

                vector<string>::iterator it = params.begin();

                _dbAdapter = new pSqlAdapter (*(it), *(it+1), *(it+2), *(it+3));

                return 2;
            }

            int execute () {
                // execute unit test

                if (_state && _dbAdapter->good()) {
                    cout << "-- DB adapter is good !" << endl;
                }
                else {
                    if (_state == false) {
                        // unit test call misuses (DEV)
                        cout << _printMessage << endl;
                    }
                    else {
                        // connection error
                        cout << "{ERR} DB adapter connection error !" << endl;
                        cout << _dbAdapter->lastError() << endl;
                    }
                }

                return 1;
            }

            int shutdown () {

                cout << endl << "dbConnection unit test shutdown" << endl;

                unitTest::shutdown();

                delete _dbAdapter;

                return 0;
            }

        private :

            pSqlAdapter* _dbAdapter;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias


int main (int argc, char* argv[])
{
    evias::core::test::dbConnection* unitTester = new evias::core::test::dbConnection();

    vector<string> params;

    params.push_back ("database_name");
    params.push_back ("database_user");
    params.push_back ("database_pass");
    params.push_back ("host");

    unitTester->prepare (params);

    unitTester->execute();

    int endCode = unitTester->shutdown();

    delete unitTester;

	return endCode;
}
