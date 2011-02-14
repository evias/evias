#include "config_ini.hpp"

namespace evias {

namespace core {

    configIni::configIni (string file)
        : _file (file)
    {
        _state = true;

        ifstream testStream;

        testStream.open(_file.c_str());
        if (! testStream) {
            _state = false;
        }

        testStream.close();

        _countLines     = 0;
        _countSections  = 0;
        _countPairs     = 0;
    }

    int configIni::parse ()
    {
        if (! _state) {
            // has not passed init test
            cout << endl
                 << "[EXIT] Could not pass initialization test. Look for the config file access rights because it could not be opened."
                 << endl;

            return 0;
        }

        _countLines = this->_parseFile ();

        if (_state) {

            _countSections  = this->_parseSections ();
            _countPairs     = this->_parsePairs ();
        }

        return 1;
    }

    int configIni::_parseFile()
    {
        ifstream reader;

        reader.open(_file.c_str());

        if (! reader) {

            cout << endl
                 << "[ERROR] Could not open file: " << _file << endl;
            _state = false;

            return 0;
        }

        string line = "";
        int count = 0;
        while (reader.good()) {
            getline (reader, line);

            _lines.push_back (line);
            count ++;
        }
        reader.close();

        return count;
    }

    int configIni::_parseSections()
    {
        if (_lines.empty()) {
            return 0;
        }

        vector<string>::iterator lineIt = _lines.begin();
        int count = 0;
        string sectionValidChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
        string currentSection = "";
        vector<string> currentSectionLines;
        bool hasSavedData = false;
        for (; lineIt != _lines.end(); lineIt++) {
            string::size_type findOpeningBracket = (*lineIt).find_first_of ("[");
            string::size_type findClosingBracket = (*lineIt).find_first_of ("]");

            if (findOpeningBracket != string::npos && findClosingBracket != string::npos) {
                // has []

                string beforeOpeningPart = (*lineIt).substr (0, findOpeningBracket);
                string afterClosingPart  = (*lineIt).substr(findClosingBracket, (*lineIt).size() - findClosingBracket - 1);
                string betweenBrackets   = (*lineIt).substr(findOpeningBracket + 1, findClosingBracket - findOpeningBracket - 1);

                // - (1) look if there is some other character than spaces before the opening bracket
                // - (2) look if there is an invalid character in between the brackets
                // - (3) look if there is more text after the closing bracket (invalid)
                string::size_type findNotSpace      = beforeOpeningPart.find_first_not_of (" ");
                string::size_type findInvalidChar   = betweenBrackets.find_first_not_of (sectionValidChars);
                string::size_type findFurtherText   = afterClosingPart.find_first_not_of (" ");
                if (
                    findNotSpace != string::npos        ||  // (1)
                    findInvalidChar != string::npos     ||  // (2)
                    findFurtherText != string::npos         // (3)
                   )
                {
                    // not a section line
                    if (! currentSection.empty()) {
                        currentSectionLines.push_back (*lineIt);
                    }
                    continue ;
                }

                // will now add a new section

                // data save
                if (! currentSection.empty())
                    _linesBySection.insert (pair<string, vector<string> >(currentSection, currentSectionLines));

                // add new section
                _sections.push_back (betweenBrackets);
                currentSection = betweenBrackets;
                currentSectionLines.clear();
                count++;
            }
            else {
                if (! currentSection.empty()) {
                    currentSectionLines.push_back (*lineIt);
                }
            }
        }

        if (! hasSavedData && ! currentSection.empty()) {
            _linesBySection.insert (pair<string,vector<string> >(currentSection, currentSectionLines));
        }

        return count;
    }

    int configIni::_parsePairs ()
    {
        if (_linesBySection.empty()) {
            return 0;
        }

        map<string, vector<string> >::iterator itSection = _linesBySection.begin();
        int count = 0;
        for (; itSection != _linesBySection.end(); itSection++) {
            // for each section, save lines of format "key = value"

            map<string,string> currentSectionPairs;

            iniSection currentSection;
            currentSection.setLabel((*itSection).first);

            vector<string>::iterator itLines = (*itSection).second.begin();
            for (; itLines != (*itSection).second.end(); itLines++) {
                // for each line, check the syntax and save if needed

                string::size_type posEqual = (*itLines).find ("=");
                int lineSize = (*itLines).size();
                if (posEqual != string::npos) {
                    // line to be added_
                    string beforeEqual = (*itLines).substr (0, posEqual);
                    string afterEqual  = (*itLines).substr (posEqual + 1, lineSize - posEqual - 1);

                    // XXX trim values
                    evias::core::trim (beforeEqual, " ");
                    evias::core::trim (afterEqual, " ");

                    currentSectionPairs.insert (pair<string,string>(beforeEqual, afterEqual));

                    count++;
                }
            }

            // save pairs for section
            currentSection.setPairs (currentSectionPairs);

            _iniSections.push_back (currentSection);
        }

        return count;
    }

    int configIni::changeValue (string section, string pairKey, string newValue)
    {
        ofstream writer;
        // erase and rewrite
        writer.open (_file.c_str(), ios::out);

        if (! writer.good()) {
            return 0;
        }

        // foreach section
        vector<iniSection>::iterator currentSectionIt = _iniSections.begin();
        for (int i = 0; currentSectionIt != _iniSections.end(); currentSectionIt++, i++) {
            // write section label line
            writer  << "[" << (*currentSectionIt).getLabel() << "]" << endl;

            map<string,string> pairsToWrite = (*currentSectionIt).getPairs();

            if ((*currentSectionIt).getLabel() == section) {
                // is section of pair to be changed

                // foreach pair of current section
                map<string,string> currentSectionPairs = (*currentSectionIt).getPairs();
                map<string,string> newSectionPairs;

                // keys are re-written since there is a modification to proceed
                pairsToWrite.clear();

                map<string,string>::iterator pairsIt   = currentSectionPairs.begin();
                for (int j = 0; pairsIt != currentSectionPairs.end(); pairsIt++) {
                    string currentPairKey = (*pairsIt).first;
                    string currentPairVal = (*pairsIt).second;
                    if (currentPairKey == pairKey) {
                        // pair to be changed
                        currentPairVal = newValue;
                    }

                    pairsToWrite.insert(pair<string,string>(currentPairKey, currentPairVal));
                }

                (*currentSectionIt).setPairs(pairsToWrite);
            }

            // write each pair in file
            map<string,string>::iterator itPair = pairsToWrite.begin();
            for (; itPair != pairsToWrite.end(); itPair++) {
                writer << (*itPair).first << " = " << (*itPair).second << endl;
            }

            // each section declaration is followed by a new empty line
            writer << endl;
        }

        writer.close();

        return 1;
    }

    string configIni::getValue (string section, string pairKey)
    {
        string notFound = "";

        vector<iniSection>::iterator itSec = _iniSections.begin();
        map<string,string>::iterator itPairs;
        for (; itSec != _iniSections.end(); itSec++) {

            if ((*itSec).getLabel() == section) {
                // section to read attribute of

                map<string,string> sectionPairs = (*itSec).getPairs();
                itPairs = sectionPairs.begin();
                for ( ; itPairs != sectionPairs.end(); itPairs++) {

                    if ((*itPairs).first == pairKey) {

                        return (*itPairs).second;
                    }
                }
            }
        }

        return notFound;
    }

    map<string,string> configIni::getValues(string section, vector<string> keys)
    {
        map<string,string> outputMap;
        vector<string>::iterator itKey = keys.begin();
        for (; itKey != keys.end(); itKey++) {

            string keyValue = getValue(section, (*itKey));

            outputMap.insert(make_pair((*itKey), keyValue));
        }

        return outputMap;
    }

}; // end namespace core

}; // end namespace evias
