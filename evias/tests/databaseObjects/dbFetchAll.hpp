#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "../../core/unit_test_abstract.hpp"
#include "../../core/db_adapter.hpp"
#include "../../core/db_table.hpp"
#include "../../core/date.hpp"

namespace evias {

namespace core {

namespace test {

namespace databaseObjects {

    using namespace std;

    using evias::core::dbAdapter;
    using evias::core::pSqlAdapter;
    using evias::core::dbTable;

    class dbFetchAll : public unitTest
    {
        public :
            dbFetchAll () : unitTest() {};
            ~dbFetchAll() {};

            inline void prepare ()
            {
                assertable<int>::assertEqual(_options.size(), 4);

                vector<string>::iterator it = _options.begin();
                string dbname = (*it),
                       user   = *(it+1),
                       pass   = *(it+2),
                       host   = *(it+3);

                // init database work
                _dbAdapter = new pSqlAdapter (dbname, user, pass, host);

                assertable<bool>::assertEqual(_dbAdapter->good(), true);

                // init database static work
                dbTable<pSqlAdapter>::setDefaultAdapter(_dbAdapter);

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<int>::assertEqual(_returnCode, (int) RETURN_SUCCESS);

                dbTable<pSqlAdapter>* tStudents = new dbTable<pSqlAdapter>(_dbAdapter, "student", "public");
                vector<string> fieldNames = tStudents->fieldNames();

                // fetch data ;
                vector<dbRow> rows = dbTable<pSqlAdapter>::fetchAll("student");

                assertable<bool>::assertEqual(rows.empty(), false);

                delete tStudents;

                return _returnCode;
            }

            inline int shutdown () {

                unitTest::shutdown();

                return _returnCode;
            }

        private :

            pSqlAdapter* _dbAdapter;

    };

}; // end namespace databaseObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

