#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../../core/jsonParser.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

namespace jsonObjects {

    class simpleParse : public unitTest
    {
        public :

            simpleParse () : unitTest() {};

            ~simpleParse () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
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

                string arrayJson = "\"my_array\":[\"key1\",\"array content\",\"should work\"]";
                jsonArrayEntry* staticArray1 = jsonArrayEntry::fromJSON (arrayJson);

                assertable<int>::assertEqual(staticArray1->getData().size(), 3);
                assertable<int>::assertEqual(object->getEntries().size(), 5);

                // void memory
                entries.clear ();
                delete single1;
                delete single2;
                delete single3;
                delete array1;
                delete array2;
                delete staticArray1;

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                return _returnCode;
            }

    };

}; // end namespace jsonObjects

}; // end namespace test

}; // end namespace core

}; // end namespace evias

