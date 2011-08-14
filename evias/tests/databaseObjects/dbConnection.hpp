#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "../../core/db_adapter.hpp"
#include "../../core/unit_test_abstract.hpp"

using namespace std;

namespace evias {

namespace core {

namespace test {

namespace databaseObjects {

    class dbConnection : public unitTest
    {
        public :
            dbConnection () : unitTest () {};
            ~dbConnection() {};

            inline void prepare ()
            {
                assertable<int>::assertEqual(_options.size(), 4);

                vector<string>::iterator it = _options.begin();
                string dbname = (*it),
                       user   = *(it+1),
                       pass   = *(it+2),
                       host   = *(it+3);

                // start connection
                _dbAdapter = new pSqlAdapter (dbname, user, pass, host);

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                // execute unit test

                assertable<int>::assertEqual(_returnCode, (int) RETURN_SUCCESS);
                assertable<bool>::assertEqual(_dbAdapter->good(), true);

                return _returnCode;
            }

            inline int shutdown () {

                unitTest::shutdown();

                delete _dbAdapter;

                return _returnCode;
            }

        private :

            pSqlAdapter* _dbAdapter;

    };

}; // end namespace databaseObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

