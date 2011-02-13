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

    class dbInsert : public unitTest
    {
        public :
            dbInsert () : unitTest() {};
            ~dbInsert() {};

            inline void prepare ()
            {
                if (_options.size() != 4) {
                    _returnMsg = "dbInsert unitary test requires 4 call arguments.";
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

                // init orm class
                _tableORM = new dbTable<pSqlAdapter>(_dbAdapter);

                // init insert data
                _data.insert (make_pair ("first_name", "Yannick"));
                _data.insert (make_pair ("last_name", "Yannick"));
                _data.insert (make_pair ("address", "Rue de l'église 37"));
                _data.insert (make_pair ("zipcode", "4720"));
                _data.insert (make_pair ("city", "Kelmis"));
                _data.insert (make_pair ("country", "Belgium"));

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (_returnCode != (int) RETURN_SUCCESS) {
                    return _returnCode;
                }

                insertQuery* queryObj = new insertQuery;

                queryObj->into ("student")
                        ->values (_data);

                int cntRows = _tableORM->insert (queryObj);

                if (cntRows == 0) {
                    _returnMsg = "insert query did not work.";
                    return setReturnCode((int) ERROR_ENVIRONMENT);
                }

                delete queryObj;

                return _returnCode;
            }

            inline int shutdown ()
            {
                if (_tableORM != NULL)
                    delete _tableORM;

                if (_dbAdapter != NULL)
                    delete _dbAdapter;

                unitTest::shutdown();

                return _returnCode;
            }

        private :

            pSqlAdapter*            _dbAdapter;
            dbTable<pSqlAdapter>*   _tableORM;

            map<string,string>      _data;

    };

}; // end namespace databaseObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

