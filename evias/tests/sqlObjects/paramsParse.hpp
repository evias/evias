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

    class paramsParse : public unitTest
    {
        public :

            paramsParse () : unitTest() {};

            ~paramsParse () {};

            inline void prepare ()
            {
                _queryObj = new selectQuery;

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                map<string,string> params;

                params.insert(make_pair("?studentId?", "1"));
                params.insert(make_pair("?firstName?", "Grégory"));

                _queryObj->select ("first_name")
                         ->select ("last_name")
                         ->select ("access_role.id_access_role")
                         ->from   ("access_role")
                         ->join   ("student", "", "access_role.id_access_type_value = student.id_student")
                         ->where  ("id_student = ?studentId?")
                         ->where  ("first_name = ?firstName?")
                         ->with   (params);

                if (_queryObj->getSelect()->getFields().size() != 3) {
                    _returnMsg = "code defines 3 select fields, object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getFrom()->getLines().size() != 2) {
                    _returnMsg = "code defines 2 from lines, object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getWhere()->getParam("?studentId?") != "1") {
                    _returnMsg = "code defines param '?studentId?' to be '1', object returned other value.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                if (_queryObj->getWhere()->getParam("?firstName?") != "Grégory") {
                    _returnMsg = "code defines param '?firstName?' to be 'Grégory', object returned other value.";
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

