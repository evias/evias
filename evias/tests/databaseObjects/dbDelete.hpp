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

    class dbDelete : public unitTest
    {
        public :
            dbDelete () : unitTest() {};
            ~dbDelete() {};

            inline void prepare () {
                // initialize unit test

                if (_options.size() != 4) {
                    _returnMsg = "dbDelete unitary test requires 4 call arguments";
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
                    _returnMsg = "could not initialize database adapter. (adapter error: " + _dbAdapter->lastError();
                    setReturnCode((int) ERROR_ENVIRONMENT);
                    return ;
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

                removeQuery* queryObj = new removeQuery;

                queryObj->from ("student")
                        ->where ("id_student = 2");

                int cntRows = _tStudents->remove (queryObj);

                if (cntRows == 0) {
                    _returnMsg = "no record to be delete for student.id_student = 2.";
                    return setReturnCode((int) ERROR_TEST_DATA);
                }

                delete queryObj;

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

