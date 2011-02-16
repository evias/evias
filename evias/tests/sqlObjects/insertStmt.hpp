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

    class insertStmt : public unitTest
    {
        public :

            insertStmt () : unitTest() {};

            ~insertStmt () {};

            inline void prepare ()
            {
                _queryObj = new insertQuery;

                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                // test creation of insertQuery using map<string,string>.
                // if this does not work it means the data storage is
                // broken.

                map<string,string> data;
                data.insert (make_pair ("first_name", "Yannick"));
                data.insert (make_pair ("last_name", "Yannick"));
                data.insert (make_pair ("address", "Rue de l'église 37"));
                data.insert (make_pair ("zipcode", "4720"));
                data.insert (make_pair ("city", "Kelmis"));
                data.insert (make_pair ("country", "Belgium"));

                _queryObj->into ("student")
                         ->values (data);

                // make sure we inserted everything
                if (_queryObj->getValuesDispatched().size() != 6) {
                    _returnMsg = "code defines 6 field, value pairs but object returned other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // check if all fields can be retrieved
                if (! _queryObj->hasValueForField("first_name") ||
                    ! _queryObj->hasValueForField("last_name")  ||
                    ! _queryObj->hasValueForField("address")  ||
                    ! _queryObj->hasValueForField("zipcode")  ||
                    ! _queryObj->hasValueForField("city")  ||
                    ! _queryObj->hasValueForField("country")) {

                    _returnMsg = "code defines first_name,last_name,address,zipcode,city,country but some are missing at call.";
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

            insertQuery* _queryObj;

    };

}; // end namespace sqlObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

