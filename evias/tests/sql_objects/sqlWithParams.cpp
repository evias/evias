#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class sqlWithParams : public unitTest
    {
        public :

            sqlWithParams () : unitTest() {};

            ~sqlWithParams ();

            inline int prepare (vector<string> params)
            {
                _queryObj = new selectQuery;

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "-- Executing sqlWithParams unitary test"
                     << endl;

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

                cout << endl
                     << "-- Query resulting from :"
                     << endl
                     << "_queryObj->select()->select()->select()->from()->join()->where()->where()->with()"
                     << endl
                     << endl
                     << _queryObj->toString()
                     << endl;

                return 1;
            }

            inline int shutdown ()
            {
                return 0;
            }

        private :

            selectQuery* _queryObj;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[]) {
    using evias::core::test::sqlWithParams;
    using namespace std;

    sqlWithParams* unitTester = new sqlWithParams();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
