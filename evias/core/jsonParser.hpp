#ifndef __EVIAS_CORE_NS_JSONPARSER__
#define __EVIAS_CORE_NS_JSONPARSER__

#include <string>
#include <map>
#include <vector>

#include "string_utils.hpp"
#include "common_utils.hpp"

namespace evias {

namespace core {

    using namespace std;

    enum entryType {
        ENTRY_SINGLE = 1,
        ENTRY_ARRAY  = 2,
        ENTRY_OBJECT  = 3
    };

    enum jsonCodes {
        /* Valid data */
        RET_SUCCESS         = 0,
        RET_EMPTY           = 1,
        RET_VALID_JSON      = 2,
        RET_VALID_OBJ       = 3,

        /* Errors */
        ERR_BEFORE_BRACKET  = 10,
        ERR_BEFORE_CURLY    = 11,
        ERR_INSIDE_ARRAY_DECL  = 12,
        ERR_INSIDE_OBJECT_DECL = 13,
        ERR_QUOTES_COUNT    = 14
    };

    static string jsonMessages[] = {
        "Success",
        "No work",
        "JSON data validated",
        "Object created",
        "Error: Before bracket [ at #LINE# in #FILE# ]",
        "Error: Before curly bracket [ at #LINE# in #FILE# ]",
        "Error: Inside array declaration [ at #LINE# in #FILE# ]",
        "Error: Inside object declaration [ at #LINE# in #FILE# ]",
        "Error: There is a missing quote [ starts at #FIRST_QUOTE_POS# ; last at #LAST_QUOTE_POS# ]"
    };

    using evias::core::countElm;

    class jsonEntry {

        public :
            jsonEntry (string key)
                : _key (key)
            {
                _initMessages();
            }

            ~jsonEntry () {};

            string key ()
                { return _key; }

            entryType getType ()
                { return _type; }

            virtual string toJSON() = 0;

            void storeReturn (jsonCodes code)
                { _lastReturn = code; _codesTrace.push_back (code); }

            int lastReturn ()
                { return (int) _lastReturn; }

            string lastReturnMsg ()
                { return _codeGateway [(int) _lastReturn]; }

        protected :

            string      _key;
            entryType   _type;

            map<int, string> _codeGateway;

            jsonCodes   _lastReturn;

            vector<jsonCodes> _codesTrace;

        private :

            inline void _initMessages ()
            {
                int arrCodes[] = {
                    0, 1, 2, 3,
                    10, 11, 12, 13, 14
                };

                int cntElm = countElm (jsonMessages);

                for (int i = 0; i < cntElm; i++)
                    _codeGateway.insert( make_pair(arrCodes[i], jsonMessages[i]) );
            }
    };

    class jsonSingleEntry : public jsonEntry {
        public :
            jsonSingleEntry (string key, string value)
                : jsonEntry (key), _value (value)
            {
                _type = ENTRY_SINGLE;
            }

            ~jsonSingleEntry() {};

            inline string toJSON () {
                return string( "\"" + _key + "\"" + ":\"" + _value + "\"" );
            }

            static jsonSingleEntry* fromJSON(string);

        private :

            string _value;
    };

    class jsonArrayEntry : public jsonEntry {
        public :
            jsonArrayEntry (string key)
                : jsonEntry (key)
            {
                _type = ENTRY_ARRAY;
            }

            ~jsonArrayEntry () {};

            inline void setData (vector<string> valueLines) {
                s_vectorCopy(valueLines, _entries);
            }

            vector<string> getData ()
                { return _entries; }

            inline string toJSON () {
                string json = "\"" + _key + "\":";

                if (_entries.empty()) {
                    json = json + "\"\""; // empty value
                }
                else {
                    // JSON array

                    json = json + "[";

                    vector<string>::iterator itLines = _entries.begin();
                    int max = _entries.size();
                    for (int i = 0; itLines != _entries.end(); itLines++, i++) {
                        // append current entry to array representation

                        json = json + (i > 0 ? ",":"") + "\"" + (*itLines) + "\"" ;
                    }

                    json = json + "]";
                }

                return json;
            }

            static jsonArrayEntry* fromJSON(string);

        private :

            vector<string> _entries;

    };

    class jsonObjectEntry : public jsonEntry {
        public :

            jsonObjectEntry (string key="")
                : jsonEntry (key)
            {
                _type = ENTRY_OBJECT;
            }

            ~jsonObjectEntry () {};

            inline void setEntries (vector<jsonEntry*> entries) {
                _entries.clear();

                vector<jsonEntry*>::iterator itSwap = entries.begin ();
                for (; itSwap != entries.end(); itSwap++)
                    _entries.push_back ((*itSwap));
            }

            inline vector<jsonEntry*> getEntries()
                { return _entries; };

            inline string toJSON () {
                string json = "";

                if (_key.empty())
                    json = "{";
                else
                    json = _key + "={";

                vector<jsonEntry*>::iterator itEntry = _entries.begin ();
                for (int i = 0 ; itEntry != _entries.end (); itEntry++, i++ ) {
                    // may be a recursivity point since an entry maybe of type jsonObjectEntry

                    json = json + (i > 0 ? "," : "") + (*itEntry)->toJSON ();
                }

                json = json + "}";

                return json;
            }

            static jsonObjectEntry* fromJSON (string);

        private :

            vector<jsonEntry*>      _entries;
            map<string,jsonEntry*>  _pairs;
    };

}; // end namespace core

}; // end namespace evias

#endif
