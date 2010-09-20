#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "core/unit_test_abstract.hpp"
#include "core/db_adapter.hpp"
#include "core/db_table.hpp"
#include "core/date.hpp"
#include "core/sql_queries.hpp"

namespace evias {

namespace core {

namespace test {

    using namespace std;

    using evias::core::dbAdapter;
    using evias::core::pSqlAdapter;
    using evias::core::dbTable;

    class dbInsert : public unitTest
    {
        public :
            dbInsert () : unitTest() {};
            ~dbInsert() {};

            inline int prepare (vector<string> params) {
                // initialize unit test

                if (params.size() != 4) {
                    _printMessage = "You need to call dbInsert::prepare with 4 parameters in the vector.";

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

                // init orm class
                _tableORM = new dbTable<pSqlAdapter>(_dbAdapter);

                // init insert data
                _data.insert (make_pair ("first_name", "Yannick"));
                _data.insert (make_pair ("last_name", "Yannick"));
                _data.insert (make_pair ("address", "Rue de l'église 37"));
                _data.insert (make_pair ("zipcode", "4720"));
                _data.insert (make_pair ("city", "Kelmis"));
                _data.insert (make_pair ("country", "Belgium"));

                return 2;
            }

            inline int execute () {
                // execute unit test

                if (! _state) {
                    cout << _printMessage << endl;
                    return 1;
                }

                cout << endl
                     << "-- build the query object"
                     << endl;

                insertQuery* queryObj = new insertQuery;

                queryObj->into ("student")
                        ->values (_data);

                cout << endl
                     << "-- query object built"
                     << endl
                     << "-- Will now execute: " << queryObj->toString()
                     << endl;

                int cntRows = _tableORM->insert (queryObj);

                cout << endl << "-- Size : " << cntRows
                     << endl;

                delete queryObj;

                return 1;
            }

            inline int shutdown () {

                cout << endl << "iSchool dbInsert shutdown" << endl;

                if (_tableORM != NULL)
                    delete _tableORM;

                if (_dbAdapter != NULL)
                    delete _dbAdapter;

                unitTest::shutdown();

                return 0;
            }

        private :

            pSqlAdapter*            _dbAdapter;
            dbTable<pSqlAdapter>*   _tableORM;

            map<string,string>      _data;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias


int main (int argc, char* argv[])
{
    evias::core::test::dbInsert* unitTester = new evias::core::test::dbInsert();

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
