#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/jsonParser.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    class jsonParsing : public unitTest
    {
        public :

            jsonParsing () : unitTest() {};

            ~jsonParsing ();

            inline int prepare (vector<string> params)
            {
                return 2;
            }

            inline int execute ()
            {
                jsonSingleEntry* single1 = new jsonSingleEntry("my_single1", "data");
                jsonSingleEntry* single2 = new jsonSingleEntry("my_single2", "dontknow");
                jsonSingleEntry* single3 = new jsonSingleEntry("my_single3", "one more..");
                jsonArrayEntry*  array1  = new jsonArrayEntry ("greg");
                jsonArrayEntry*  array2  = new jsonArrayEntry ("yan");

                vector<string> arrayOneData;
                arrayOneData.push_back ("Grégory");
                arrayOneData.push_back ("Saive");
                arrayOneData.push_back ("1988-08-29");

                vector<string> arrayTwoData;
                arrayTwoData.push_back ("Yannick");
                arrayTwoData.push_back ("Saive");
                arrayTwoData.push_back ("1983-12-10");

                array1->setData(arrayOneData);
                array2->setData(arrayTwoData);

                vector<jsonEntry*> entries;
                entries.push_back (single1);
                entries.push_back (single2);
                entries.push_back (array1);
                entries.push_back (array2);
                entries.push_back (single3);

                jsonObjectEntry* object = new jsonObjectEntry;
                object->setEntries (entries);

                string json = object->toJSON ();

                cout << endl
                     << endl
                     << "jsonObjectEntry::toJSON : {" << json << "}"
                     << endl
                     << endl;

                string arrayJson = "\"my_array\":[\"key1\",\"array content\",\"should work\"]";
                jsonArrayEntry* staticArray1 = jsonArrayEntry::fromJSON (arrayJson);

                cout << endl << endl
                     << "JSON : {" << arrayJson << "}"
                     << endl
                     << "-- Array key : " << staticArray1->key()
                     << endl
                     << "-- Count of entries : " << staticArray1->getData().size()
                     << endl
                     << "JSON : {" << staticArray1->toJSON() << "}"
                     << endl ;

                // void memory
                entries.clear ();
                delete single1;
                delete single2;
                delete single3;
                delete array1;
                delete array2;
                delete staticArray1;

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
    using evias::core::test::jsonParsing;
    using namespace std;

    jsonParsing* unitTester = new jsonParsing();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare(params);

    if (endCode == 2) {

        unitTester->execute();
        endCode = unitTester->shutdown();
    }

    return endCode;
}
