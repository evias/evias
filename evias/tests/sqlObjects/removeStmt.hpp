#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../../core/sql_queries.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

namespace sqlObjects {

    class removeStmt : public unitTest
    {
        public :

            removeStmt () : unitTest() {};

            ~removeStmt ();

            inline void prepare ()
            {
                _queryObj = new removeQuery;

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                // test should check if the delete condition
                // is correctly parsed and if the table we are
                // deleting from is right as well.

                _queryObj->from ("student")
                         ->where ("id_student = 2");

                if (_queryObj->getWhere()->getLines().size() != 1) {
                    _returnMsg = "code defines one condition but object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getFrom()->getLines().size() != 1) {
                    _returnMsg = "code defines one relation but object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                return _returnCode;
            }

            inline int shutdown()
            {
                if (_queryObj != NULL)
                    delete _queryObj;

                return _returnCode;
            }

        private :

            removeQuery* _queryObj;

    };

}; // end namespace sqlObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

