#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class easySql : public unitTest
    {
        public :

            easySql () : unitTest() {};

            ~easySql ();

            inline int prepare (vector<string> params)
            {
                _queryObj = new selectQuery;

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "-- Execution easySql unitary test"
                     << endl;

                _queryObj->select ("first_name")
                         ->select ("last_name")
                         ->select ("access_role.id_access_role")
                         ->from   ("access_role")
                         ->join   ("student", "", "access_role.id_access_type_value = student.id_student")
                         ->where  ("id_student = 1")
                         ->where  ("first_name = 'Grégory'");

                cout << endl
                     << "-- Query resulting from :"
                     << endl
                     << "_queryObj->select()->select()->select()->from()->join()->where()->where()"
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
    using evias::core::test::easySql;
    using namespace std;

    easySql* unitTester = new easySql();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
