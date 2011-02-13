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

    class dataChecks : public unitTest
    {
        public :

            dataChecks () : unitTest() {};

            ~dataChecks ();

            inline void prepare ()
            {
                _queryObj = new selectQuery;

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                _queryObj->select ("first_name")
                         ->select ("last_name")
                         ->select ("access_role.id_access_role")
                         ->from   ("access_role")
                         ->join   ("student", "", "access_role.id_access_type_value = student.id_student")
                         ->where  ("id_student = 1")
                         ->where  ("first_name = 'Grégory'");

                // test if after setting values, the object will return
                // the expected count of fields for each statement
                // part. having a wrong count means the children
                // storage is not done correctly.

                if (_queryObj->getSelect()->getFields().size() != 3) {
                    _returnMsg = "code defines 3 fields but sqlSelect::getFields returns other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getFrom()->getLines().size() != 2) {
                    _returnMsg = "code defines 3 FROM line but sqlFrom::getLines returns other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getWhere()->getLines().size() != 2) {
                    _returnMsg = "code defines 3 WHERE conditions but sqlWhere::getLines returns other entries count.";
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

            selectQuery* _queryObj;

    };

}; // end namespace sqlObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

