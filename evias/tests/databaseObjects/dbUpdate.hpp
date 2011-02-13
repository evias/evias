#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "../../core/unit_test_abstract.hpp"
#include "../../core/db_adapter.hpp"
#include "../../core/db_table.hpp"
#include "../../core/date.hpp"
#include "../../core/sql_queries.hpp"

namespace evias {

namespace core {

namespace test {

namespace databaseObjects {

    using namespace std;

    using evias::core::dbAdapter;
    using evias::core::pSqlAdapter;
    using evias::core::dbTable;

    class dbUpdate : public unitTest
    {
        public :
            dbUpdate () : unitTest() {};
            ~dbUpdate() {};

            inline void prepare ()
            {
                if (_options.size() != 4) {
                    _returnMsg = "dbUpdate unitary test requires 4 call arguments.";
                    setReturnCode((int) ERROR_DATA_INPUT);
                    return ;
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

                _tStudents = new dbTable<pSqlAdapter>(_dbAdapter, "student");

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (_returnCode != (int) RETURN_SUCCESS) {
                    return _returnCode;
                }

                updateQuery* queryObj = new updateQuery;

                queryObj->table ("student")
                        ->field ("first_name", "Grégory")
                        ->field ("last_name", "Saive")
                        ->where ("id_student = 1");

                // fetch data ;
                int cntRows = _tStudents->update (queryObj);

                if (cntRows == 0) {
                    _returnMsg = "update query did not work.";
                    return setReturnCode((int) ERROR_ENVIRONMENT);
                }

                return _returnCode;
            }

            inline int shutdown ()
            {
                if (_tStudents != NULL)
                    delete _tStudents;

                if (_dbAdapter != NULL)
                    delete _dbAdapter;

                unitTest::shutdown();

                return _returnCode;
            }

        private :

            pSqlAdapter*            _dbAdapter;
            dbTable<pSqlAdapter>*   _tStudents;

    };

}; // end namespace databaseObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

