#include "regexp_objects.hpp"

namespace evias {

namespace core {

    charsGroup::charsGroup()
        : _charsGroupPattern("")
    {

    }

    charsGroup::charsGroup(string groupPattern)
        : _charsGroupPattern(groupPattern)
    {
        _internalParse();
    }

    charsGroup::~charsGroup()
    {

    }

    void charsGroup::setMatchingChars(vector<string> in)
    {
        std::copy(in.begin(), in.end(), _matchingChars.begin());
    }

    vector<string> charsGroup::getMatchingChars()
    {
        return _matchingChars;
    }

    void charsGroup::setPattern(string pattern)
    {
        _charsGroupPattern = pattern;
        _internalParse();
    }

    bool charsGroup::isSpecialGroup(string group)
    {
        for (int i = 0; i < countElm(groupSpecialKeys); i++) {
            if (group == groupSpecialKeys[i])
                return true;
        }

        return false;
    }

    charsGroup charsGroup::getSpecialGroup(string group)
    {
        charsGroup returnGroup;
        vector<string> matchingChars;
        string dictionnary = "";

        if (group == ":alpha") {
            dictionnary = __mLowercaseDictionnary + "," + __mUppercaseDictionnary + "," + __mDigitDictionnary;
        }
        else if (group == ":digit") {
            dictionnary = __mDigitDictionnary;
        }
        else if (group == ":any") {
            dictionnary = __mAnyDictionnary;
        }

        string::iterator it = dictionnary.begin();
        for (; it != dictionnary.end(); it++) {
            stringstream ss;
            ss << *it;
            matchingChars.push_back(ss.str());
        }

        returnGroup.setMatchingChars(matchingChars);

        return returnGroup;
    }

    int charsGroup::_internalParse()
    {
        if (_charsGroupPattern.empty()) {
            return 0;
        }

        _matchingChars.clear();

        if (_charsGroupPattern[0] == ':' && isSpecialGroup(_charsGroupPattern)) {
            // :alpha,:digit,:any

             vector<string> specialChars = getSpecialGroup(_charsGroupPattern).getMatchingChars();
             std::copy(specialChars.begin(), specialChars.end(), _matchingChars.begin());

             return _matchingChars.size();
        }

        // XXX if first character is '^', means NOT

        vector<string> matchingDigits;
        vector<string> matchingAlphas;
        vector<string> matchingOthers;

        string alphaDictionnary = (__mDigitDictionnary + __mLowercaseDictionnary + __mUppercaseDictionnary);
        string::iterator patternIt  = _charsGroupPattern.begin();
        string::iterator itBefore;
        int position = 0,
            realPos  = 0,// used for escaped issue
            patternSize = _charsGroupPattern.size();
        for (bool isEscape = false; patternIt != _charsGroupPattern.end(); patternIt++) {

            string currentCharacter;
            stringstream workaround;

            workaround << *patternIt;
            currentCharacter = workaround.str();

            if (! isEscape && *patternIt == '\\') {
                // escape the next character

                realPos = position + 1; // escape duration is 1 character
                itBefore = patternIt;
                isEscape = true;
                continue;
            }

            if (isEscape && *itBefore == '\\' && realPos == position + 1) {
                // char before was escaped, means this one should simply
                // be added to the matching chars. as escaping means
                // not processing.

                isEscape = false;
                realPos  = 0;

                if (isNumeric(currentCharacter))
                    matchingDigits.push_back(currentCharacter);
                else if (isAlpha(currentCharacter))
                    matchingAlphas.push_back(currentCharacter);
                else
                    matchingOthers.push_back(currentCharacter);

                itBefore = patternIt;
                position++;
                continue;
            }

            isEscape = false;

            if (currentCharacter == ".") {
                // . means ANY

                for (int i = 0, max = __mAnyDictionnary.size(); i < max; i++) {
                    stringstream ss;
                    ss << __mAnyDictionnary[i];
                    _matchingChars.push_back(ss.str());
                }
                itBefore = patternIt;
                break; // be the end would actually be logic..
            }

            if ((isNumeric(currentCharacter) || isAlpha(currentCharacter)) &&
                (position < patternSize - 1 && _charsGroupPattern[position+1] == '-')) {
                // followed by a minus .. must be an alpha numerical list of chars

                stringstream sEnd;
                sEnd << _charsGroupPattern[position+2];

                string  startListChar = currentCharacter;
                string  endListChar   = sEnd.str();
                size_t  startPos      = alphaDictionnary.find(startListChar);
                size_t  endPos        = alphaDictionnary.find(endListChar);

                if (startPos != string::npos && endPos != string::npos && startPos < endPos) {
                    // valid start/end characters

                    for (int alphaPos = startPos; alphaPos <= endPos; alphaPos++) {

                        stringstream ss;
                        ss << alphaDictionnary[alphaPos];
                        if (isNumeric(ss.str())) {
                            matchingDigits.push_back(ss.str());
                        }
                        else {
                            matchingAlphas.push_back(ss.str());
                        }
                    }


                    itBefore  = patternIt;
                    patternIt = patternIt + 2;
                    position  = position + 3;
                    continue;
                }
                else {
                    // pattern syntax error
                    // erroneous "-" found, should probably be
                    // escaped to be added to the matching chars

                    return -1;
                }
            }

            // coming here means the current character was not relevant for expression
            // process/parse. make sure it is not matching already and then add it

            if (isNumeric(currentCharacter) && ! in_vector(currentCharacter, matchingDigits)) {
                matchingDigits.push_back(currentCharacter);
            }
            else if (isAlpha(currentCharacter) && ! in_vector(currentCharacter, matchingAlphas)) {
                matchingAlphas.push_back(currentCharacter);
            }
            else if (! in_vector(currentCharacter, matchingOthers) && currentCharacter != "\0") {
                matchingOthers.push_back(currentCharacter);
            }

            itBefore = patternIt;
            position++;
        }

        // done parsing pattern, should now sort the parsed data.
        // digits belong to the begin as it matches a small list.
        // lowercase + uppercase alphabet is next.
        // and special characters end up the list.

        if (_matchingChars.empty()) {
            std::sort(matchingDigits.begin(), matchingDigits.end(), &smallerThan);
            std::sort(matchingAlphas.begin(), matchingAlphas.end(), &smallerThan);

            // std::copy would take years
            vector<string>::iterator it;
            for(it = matchingDigits.begin(); it != matchingDigits.end(); it++) {
                //cout << "digit [" << *it << "]" << endl;
                _matchingChars.push_back(*it);
            }
            for(it = matchingAlphas.begin(); it != matchingAlphas.end(); it++) {
                //cout << "alpha [" << *it << "]" << endl;
                _matchingChars.push_back(*it);
            }
            for(it = matchingOthers.begin(); it != matchingOthers.end(); it++) {
                //cout << "other [" << *it << "]" << endl;
                _matchingChars.push_back(*it);
            }
        }

        return _matchingChars.size(); // good boy
    }

}; // core

}; // evias

