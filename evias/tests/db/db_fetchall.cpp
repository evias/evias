#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "core/unit_test_abstract.hpp"
#include "core/db_adapter.hpp"
#include "core/db_table.hpp"
#include "core/date.hpp"

namespace evias {

namespace core {

namespace test {

    using namespace std;

    using evias::core::dbAdapter;
    using evias::core::pSqlAdapter;
    using evias::core::dbTable;

    class dbFetchAll : public unitTest
    {
        public :
            dbFetchAll () : unitTest() {};
            ~dbFetchAll() {};

            inline int prepare (vector<string> params) {
                // initialize unit test

                if (params.size() != 4) {
                    _printMessage = "You need to call dbFetchAll::prepare with 4 parameters in the vector.";

                    _state = false;

                    return 2;
                }
                else if (params.at(0) == "database_name" || params.at(3) == "host") {
                    _printMessage = "You need to edit the dbConnection in order to connect to the right server / database";

                    _state = false;

                    return 2;
                }

                vector<string>::iterator it = params.begin();

                // init database work
                _dbAdapter = new pSqlAdapter (*(it), *(it+1), *(it+2), *(it+3));

                if (_dbAdapter->good()) {
                    _state = true;
                }
                else {
                    _printMessage = _dbAdapter->lastError();
                    _state = false;
                }

                // init database static work
                dbTable<pSqlAdapter>::setDefaultAdapter(_dbAdapter);

                return 2;
            }

            inline int execute () {
                // execute unit test

                if (! _state) {
                    cout << _printMessage << endl;
                    return 1;
                }

                dbTable<pSqlAdapter>* tStudents = new dbTable<pSqlAdapter>(_dbAdapter, "student", "public");
                vector<string> fieldNames = tStudents->fieldNames();

                // fetch data ;
                vector<dbRow> rows = dbTable<pSqlAdapter>::fetchAll("student");

                // browse data
                for (int i = 0, cntStudents = rows.size(); i < cntStudents; i++) {

                    cout << endl << "---- ENTRY [idx: " << i << endl;
                    for (int j = 0, cntFields = fieldNames.size(); j < cntFields; j++) {
                        cout    << "["  << fieldNames.at(j) << " => " << rows[i][j] << "]" << endl;
                    }
                    cout << "----" << endl;
                }

                delete tStudents;
            }

            inline int shutdown () {

                cout << endl << "iSchool dbFetchall shutdown" << endl;

                unitTest::shutdown();

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
    evias::core::test::dbFetchAll* unitTester = new evias::core::test::dbFetchAll();

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
