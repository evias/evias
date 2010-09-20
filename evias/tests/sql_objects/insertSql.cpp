#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/sql_queries.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class insertSql : public unitTest
    {
        public :

            insertSql () : unitTest() {};

            ~insertSql ();

            inline int prepare (vector<string> params)
            {
                _queryObj = new insertQuery;

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "-- Executing insertSql unitary test"
                     << endl;

                map<string,string> data;
                data.insert (make_pair ("first_name", "Yannick"));
                data.insert (make_pair ("last_name", "Yannick"));
                data.insert (make_pair ("address", "Rue de l'église 37"));
                data.insert (make_pair ("zipcode", "4720"));
                data.insert (make_pair ("city", "Kelmis"));
                data.insert (make_pair ("country", "Belgium"));

                _queryObj->into ("student")
                         ->values (data);

                cout << endl
                     << "-- Query resulting from :"
                     << endl
                     << "_queryObj->into()->values()"
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

            insertQuery* _queryObj;

    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[]) {
    using evias::core::test::insertSql;
    using namespace std;

    insertSql* unitTester = new insertSql();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
