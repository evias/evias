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

    class objectLoad : public unitTest
    {
        public :

            objectLoad () : unitTest() {};

            ~objectLoad () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                // should check the object creation per class instanciation (new)
                // as well as using the static fromJSON methods

                string jsonObjectGreg = "\"json.user\"=  {\"first_name\":\"Grégory\",\"last_name\":\"Saive\",\"address\":[\"rue de l'église\",\"37\",\"4720\",\"Kelmis\"]}";

                jsonObjectEntry* objectGreg = new jsonObjectEntry ("json.user.greg");

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

                jsonObjectEntry* fromJSONGreg = jsonObjectEntry::fromJSON (jsonObjectGreg);

                assertable<int>::assertEqual(fromJSONGreg->getEntries().size(), 3);
                assertable<int>::assertEqual(objectGreg->getEntries().size(), 3);

                // free
                delete fromJSONGreg;
                delete objectGreg;
                delete firstNameGreg;
                delete lastNameGreg;
                delete addressGreg;

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

