#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

namespace sqlObjects {

    class updateStmt : public unitTest
    {
        public :

            updateStmt () : unitTest() {};

            ~updateStmt ();

            inline void prepare ()
            {
                _queryObj = new updateQuery;

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                _queryObj->table ("student")
                         ->field ("first_name", "Grégory")
                         ->field ("last_name", "Saive")
                         ->where ("id_student = 1");

                if (_queryObj->getTable() != "student") {
                    _returnMsg = "code defines table to be 'student', object returned other name.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getFields().size() != 2) {
                    _returnMsg = "code defines 2 fields for update, object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getWhere()->getLines().size() != 1) {
                    _returnMsg = "code defines 1 condition, object return other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                return _returnCode;
            }

            inline int shutdown ()
            {
                if (_queryObj != NULL)
                    delete _queryObj;

                return _returnCode;
            }

        private :

            updateQuery* _queryObj;

    };

}; // end namespace sqlObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

