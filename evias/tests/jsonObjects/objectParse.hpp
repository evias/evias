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

    class objectParse : public unitTest
    {
        public :

            objectParse () : unitTest() {};

            ~objectParse () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                string jsonObjectGreg = "\"json.user\"={\"first_name\":\"Grégory\",\"last_name\":\"Saive\",\"address\":[\"rue de l'église\",\"37\",\"4720\",\"Kelmis\"]}";

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

                if (objectGreg->getEntries().size() != 3) {
                    _returnMsg = "code defines 3 entries, object returns other entries count.";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // free
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

