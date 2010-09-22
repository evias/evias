#include "jsonParser.hpp"

namespace evias {

namespace core {

    jsonSingleEntry* jsonSingleEntry::fromJSON (string jsonIn)
    {
        string json = jsonIn;

        evias::core::trim (json, " "); // trim spaces l+r

        vector<string> pairParts = evias::core::split (json, ':');

        string pairKey = "key_unknown";
        string pairVal = "val_unknown";
        if (pairParts.size() == 2) {
            // valid key:value sequence
            pairKey = pairParts.at(0);
            pairVal = pairParts.at(1);

            string cleanedKey = "";
            string::iterator itKey = pairKey.begin ();
            for (; itKey != pairKey.end(); itKey++) {
                if (*itKey == '\"')
                    continue;

                cleanedKey = cleanedKey + *itKey;
            }

            pairKey = cleanedKey;
        }

        jsonSingleEntry* entry = new jsonSingleEntry (pairKey, pairVal);

        return entry;
    }

    jsonArrayEntry* jsonArrayEntry::fromJSON (string jsonIn)
    {
        string json = jsonIn;

        vector<string> pairParts = evias::core::split (json, ':');

        string pairKey = "key_unknown";
        string pairVal = "[\"no_elements_found\"]";
        if (pairParts.size() >= 2) {
            // valid key:value sequence
            pairKey = pairParts.at(0); // all before the first ":"
            pairVal = pairParts.at(1); // from after first ":"

            string cleanedKey = "";
            string::iterator itKey = pairKey.begin ();
            for (; itKey != pairKey.end(); itKey++) {
                if (*itKey == '\"')
                    continue;

                cleanedKey = cleanedKey + *itKey;
            }

            pairKey = cleanedKey;

            if (pairParts.size() > 2) {
                // needs to append the rest of the JSON (we only needed to parse the general key:[] sequence
                // but splitting by ":" made us a vector<string> of multiple entries whenever there is another
                // ":" in the JSON array representation.

                vector<string>::iterator itVal = pairParts.begin() + 2;
                for (; itVal != pairParts.end(); itVal++) {
                    // append further JSON code
                    pairVal = pairVal + ":" + (*itVal);
                }
            }
        }

        string arrayEntriesData = pairVal;
        vector<string> arrayEntries;

        // parse JSON array
        string::size_type findOpeningBracket = arrayEntriesData.find ("[");
        string::size_type findClosingBracket = arrayEntriesData.find ("]");

        if (findOpeningBracket != string::npos &&
            findClosingBracket != string::npos &&
            findClosingBracket > findOpeningBracket) {
            // valid JSON array representation

            arrayEntriesData = arrayEntriesData.substr (findOpeningBracket+1, findClosingBracket - findOpeningBracket - 1);

            vector<string> entries = evias::core::split (arrayEntriesData, ',');

            vector<string>::iterator itSwap = entries.begin();
            for (; itSwap != entries.end(); itSwap++) {
                string cleaned = "";

                string::iterator itStr = (*itSwap).begin();
                for ( ; itStr != (*itSwap).end(); itStr++) {
                    if ((*itStr) == '\"')
                        continue;

                    cleaned = cleaned + (*itStr);
                }

                // save cleaned data
                arrayEntries.push_back (cleaned);
            }

            entries.clear();
        }

        jsonArrayEntry* entry = new jsonArrayEntry (pairKey);

        entry->setData (arrayEntries);

        return entry;
    }

    jsonObjectEntry* jsonObjectEntry::fromJSON (string json)
    {
        string objectKey = "unknown_object_key";
        string objectData= json;

        string::size_type findObjectDeclareBracket = objectData.find_first_of ("{");
        string::size_type findObjectCloseBracket   = objectData.find_first_of ("}");
        string::size_type findObjectAffectOperator = objectData.find_first_of ("=");

        // check for object JSON syntax

        // (1)
        bool hasValidObjectDeclareBegin = (
            findObjectAffectOperator < findObjectDeclareBracket && // = before {
            // make sure there is nothing else than spaces between = and {
            (objectData.find_first_not_of (" ", findObjectAffectOperator, findObjectDeclareBracket-findObjectAffectOperator) == string::npos)
        );

        // (2)
        bool hasKeyValSequence = (
            findObjectAffectOperator != string::npos    &&      // has =
            findObjectDeclareBracket != string::npos    &&      // has {
            findObjectCloseBracket != string::npos      &&      // has }
            hasValidObjectDeclareBegin                  &&      // see (1)
            findObjectCloseBracket > findObjectDeclareBracket   // { is after }
        );

        // XXX (3) validate syntax

        objectKey = "unknown_object_key";
        if (hasKeyValSequence) {
            // object_name={ data } format

            objectKey = objectData.substr(0, findObjectAffectOperator);

            string cleaned = "";
            string::iterator itClean = objectKey.begin ();
            for (; itClean != objectKey.end(); itClean++)
                if (*itClean != '\"')
                    cleaned = cleaned + *itClean;

            objectKey = cleaned;
        }

        // parse the magic between { }

        string betweenBrackets = objectData.substr (findObjectDeclareBracket+1, findObjectCloseBracket - findObjectDeclareBracket - 1);

        vector<jsonEntry*>  objEntries;
        vector<string>   objDeclarations;

        bool hasDoneWork = false;

        // used for storing current pair of key:value (or key:array ; key : object ..)
        string currentDeclaration = "";

        int searchOffset = 0; // used for going through the string ..
        int lastColumn  = 0;
        do {
            // find "," until not in string / array / object anymore ;
            // mean it will loop until we are in a the root sequence
            // Example JSON: {key:value,key:{subkey:subval,subkey:subval}}
            // there will be 1 match for the above example, the one between the subval,subkey is not counted

            string::size_type firstColumn = betweenBrackets.find_first_of (",", searchOffset+1);

            // check for "," emplacement : should not be in "", [] or {}

            string::size_type firstQuote  = betweenBrackets.find_first_of ("\"", lastColumn);
            string::size_type secondQuote = betweenBrackets.find_first_of ("\"", firstQuote + 1);
            string::size_type firstABracket = betweenBrackets.find_first_of ("[", lastColumn);
            string::size_type secondABracket= betweenBrackets.find_first_of ("]", firstABracket + 1);
            string::size_type firstOBracket = betweenBrackets.find_first_of ("{", lastColumn);
            string::size_type secondOBracket= betweenBrackets.find_first_of ("}", firstOBracket + 1);

            // flag the found column
            bool isInString = false;
            bool isInArray  = false;
            bool isInObject = false;

            isInString = (
                (firstQuote != string::npos && secondQuote != string::npos) &&
                (firstColumn != string::npos && firstColumn > firstQuote && firstColumn < secondQuote)
            );

            isInArray = (
                (firstABracket != string::npos && secondABracket != string::npos) &&
                (firstColumn != string::npos && firstColumn > firstABracket && firstColumn < secondABracket)
            );

            isInObject = (
                (firstOBracket != string::npos && secondOBracket != string::npos) &&
                (firstColumn != string::npos && firstColumn > firstOBracket && firstColumn < secondOBracket)
            );

            // set loop data

            if (firstColumn == string::npos) {
                hasDoneWork = true;
            }

            // set right search offset (for having currentDeclaration string)
            if (hasDoneWork)
                searchOffset = betweenBrackets.size() - 1;

            // store declaration

            if (firstColumn == string::npos) {
                // has only one declaration

                currentDeclaration = betweenBrackets.substr (lastColumn > 0 ? lastColumn + 1 : lastColumn, betweenBrackets.size() - lastColumn);

                objDeclarations.push_back (currentDeclaration);

                hasDoneWork = true;
            }
            else {
                // has multiple declarations

                if (! isInString && ! isInObject && ! isInArray) {
                    currentDeclaration = betweenBrackets.substr (
                        (lastColumn > 0 ? lastColumn + 1 : lastColumn),     // from first char after last "," or from begin
                        firstColumn - lastColumn - (lastColumn > 0 ? 1 : 0) // count between part
                    );

                    objDeclarations.push_back (currentDeclaration);

                    lastColumn   = firstColumn;
                }

                searchOffset = firstColumn;
            }

        }
        while (
            ! hasDoneWork &&
            (searchOffset != string::npos)
        );


        // iterate throug entries declarations strings
        // and create corresponding jsonEntry* child instance


        vector<string>::iterator itDeclaration = objDeclarations.begin ();
        for ( ; itDeclaration != objDeclarations.end (); itDeclaration++ ) {
            vector<string> declarationParts = evias::core::split ((*itDeclaration), ':');

            string currentDeclKey = "unknown_object_key";
            string currentDeclVal = "{}";

            string cleanedKey = "";
            string cleanedVal = "";

            if (declarationParts.size() >= 2) {
                // has key / value sequence

                // split into cleanedKey and currentDeclVal
                // respectively declaration's key and declaration's value

                currentDeclKey = declarationParts.at(0);

                string::iterator itKey = currentDeclKey.begin();
                for (;itKey != currentDeclKey.end(); itKey++) {
                    if (*itKey == ' ' || *itKey == '\"')
                        continue;
                    else
                        cleanedKey = cleanedKey + (*itKey);
                }

                vector<string> dataParts;
                s_vectorCopy (declarationParts, dataParts);

                dataParts.erase (dataParts.begin(), dataParts.begin() + 1);

                currentDeclVal = evias::core::assemble (dataParts, ":");
            }

            // iterate declaration value to parse object openings,
            // array openings or string openings.

            string::iterator itId = currentDeclVal.begin ();
            jsonEntry* entry;

            bool hasIdentifier;
            bool isValidEntry = false;
            for ( ; itId != currentDeclVal.end (); itId++ ) {

                if (*itId == ' ')
                    // skip spaces..
                    continue;

                if (*itId == '\"') {
                    // single entry beginning/closing character found

                    cleanedVal = "";
                    string::iterator itClean = currentDeclVal.begin();

                    // iterate through string for getting a cleaned value (no " character)
                    for (int x = 0; itClean != currentDeclVal.end(); itClean++, x++) {
                        if (*itClean == '\"' && (x == 0 || x == currentDeclVal.size()-1))
                            // skip quote only if at begin or end
                            continue;
                        else
                            cleanedVal = cleanedVal + (*itClean);
                    }

                    // create the instance
                    entry = new jsonSingleEntry( cleanedKey, cleanedVal );

                    isValidEntry = true;
                }
                else if (*itId == '[') {
                    // array entry beginning character found

                    // format to key:[value1,value2,value3]

                    string arrayJson = (
                        cleanedKey != "unknown_object_key"
                            ? currentDeclKey
                            : "\"unknown_array_key\""
                    );

                    arrayJson.append (":" + currentDeclVal);

                    // create the instance
                    entry = jsonArrayEntry::fromJSON (arrayJson);

                    isValidEntry = true;
                }
                else if (*itId == '{') {
                    // object entry beginning character found

                    string objectJson = (
                        cleanedKey != "unknown_object_key"
                            ? cleanedKey + "="
                            : ""
                    );
                    objectJson.append (currentDeclVal);

                    // create the instance
                    entry = jsonObjectEntry::fromJSON (objectJson);

                    isValidEntry = true;
                }

                break; // should never loop if not special character: space, ", [, {

            } // end for currentDeclVal

            if (isValidEntry)
                // add the processed entry
                objEntries.push_back (entry);

        } // end for itDeclaration


        // create the output object
        // and add its child entries, now processed

        jsonObjectEntry* objEntry = new jsonObjectEntry ((objectKey != "unknown_object_key" ? objectKey : ""));

        objEntry->setEntries (objEntries);

        objEntries.clear(); // delete address copy

        return objEntry;
    };

}; // end namespace core

}; // end namespace evias
