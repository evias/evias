#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/jsonParser.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class jsonFurther : public unitTest
    {
        public :

            jsonFurther () : unitTest() {};

            ~jsonFurther ();

            inline int prepare (vector<string> params)
            {

                return 2;
            }

            inline int execute ()
            {
                cout << endl
                     << "--- [TEST] Will now execute object parsing test"
                     << endl;

                string jsonObjectGreg = "\"iSchool.Student\"=  {\"first_name\":\"Grégory\",\"last_name\":\"Saive\",\"address\":[\"rue de l'église\",\"37\",\"4720\",\"Kelmis\"]}";

                jsonObjectEntry* objectGreg = new jsonObjectEntry ("student.greg");

                jsonSingleEntry* firstNameGreg  = new jsonSingleEntry ("first_name", "Grégory");
                jsonSingleEntry* lastNameGreg   = new jsonSingleEntry ("last_name", "Saive");
                jsonArrayEntry*  addressGreg    = new jsonArrayEntry  ("address");

                vector<string> addrData;
                addrData.push_back ("rue de l'église");
                addrData.push_back ("37");
                addrData.push_back ("4720");
                addrData.push_back ("Kelmis");

                // set address data
                addressGreg->setData (addrData);

                vector<jsonEntry*> entriesForObject;
                entriesForObject.push_back (firstNameGreg);
                entriesForObject.push_back (lastNameGreg);
                entriesForObject.push_back (addressGreg);

                // set object data
                objectGreg->setEntries (entriesForObject);

                string jsonWithCppInit = objectGreg->toJSON ();

                cout << endl
                     << "-- Raw JSON : "
                     << jsonObjectGreg
                     << endl
                     << "-- Object parse : "
                     << jsonWithCppInit
                     << endl
                     << endl;

                jsonObjectEntry* fromJSONGreg = jsonObjectEntry::fromJSON (jsonObjectGreg);

                cout << endl
                     << "-- JSON for init : "
                     << jsonObjectGreg
                     << endl
                     << "-- JSON from toJSON : "
                     << fromJSONGreg->toJSON ()
                     << endl
                     << endl;

                // free
                delete fromJSONGreg;
                delete objectGreg;
                delete firstNameGreg;
                delete lastNameGreg;
                delete addressGreg;

                return 1;
            }

            inline int shutdown ()
            {

                return 0;
            }
    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[]) {
    using evias::core::test::jsonFurther;
    using namespace std;

    jsonFurther* unitTester = new jsonFurther();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
