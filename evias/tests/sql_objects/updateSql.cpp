#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class updateSql : public unitTest
    {
        public :

            updateSql () : unitTest() {};

            ~updateSql ();

            inline int prepare (vector<string> params)
            {
                _queryObj = new updateQuery;

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "-- Execution updateSql unitary test"
                     << endl;

                _queryObj->table ("student")
                         ->field ("first_name", "Grégory")
                         ->field ("last_name", "Saive")
                         ->where ("id_student = 1");

                cout << endl
                     << "-- Query resulting from :"
                     << endl
                     << "_queryObj->table()->field()->field()->where()"
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

            updateQuery* _queryObj;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[]) {
    using evias::core::test::updateSql;
    using namespace std;

    updateSql* unitTester = new updateSql();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
