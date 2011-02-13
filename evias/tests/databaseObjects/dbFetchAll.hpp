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
                if (_options.size() != 4) {
                    _returnMsg = "dbFetchAll unitary test requires 4 call arguments.";
                    setReturnCode((int) ERROR_DATA_INPUT);
                    return;
                }

                vector<string>::iterator it = _options.begin();
                string dbname = (*it),
                       user   = *(it+1),
                       pass   = *(it+2),
                       host   = *(it+3);

                // init database work
                _dbAdapter = new pSqlAdapter (dbname, user, pass, host);

                if (! _dbAdapter->good()) {
                    _returnMsg = "database adapter could not be initialized. (adapter error: " + _dbAdapter->lastError() + ")";
                    setReturnCode((int) ERROR_ENVIRONMENT);
                    return;
                }

                // init database static work
                dbTable<pSqlAdapter>::setDefaultAdapter(_dbAdapter);

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (_returnCode != (int) RETURN_SUCCESS) {
                    return _returnCode;
                }

                dbTable<pSqlAdapter>* tStudents = new dbTable<pSqlAdapter>(_dbAdapter, "student", "public");
                vector<string> fieldNames = tStudents->fieldNames();

                // fetch data ;
                vector<dbRow> rows = dbTable<pSqlAdapter>::fetchAll("student");

                if (rows.empty()) {
                    _returnMsg = "no student records to fetch for dbFetchAll test ..";
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

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

