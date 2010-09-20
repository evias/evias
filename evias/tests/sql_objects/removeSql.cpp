#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class removeSql : public unitTest
    {
        public :

            removeSql () : unitTest() {};

            ~removeSql ();

            inline int prepare (vector<string> params)
            {
                _queryObj = new removeQuery;

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "-- Execution removeSql unitary test"
                     << endl;

                _queryObj->from ("student")
                         ->where ("id_student = 2");

                cout << endl
                     << "-- Query resulting from :"
                     << endl
                     << "_queryObj->from()->where()"
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

            removeQuery* _queryObj;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[]) {
    using evias::core::test::removeSql;
    using namespace std;

    removeSql* unitTester = new removeSql();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
