#include "regexp_chars_group.hpp"

namespace evias {

namespace core {

namespace regexp {

    bool operator==(charsGroup lft, charsGroup rgt)
    {
        // pattern and matches count should be the same
        return (
            lft.getPattern() == rgt.getPattern() &&
            lft.getMatchingChars().size() == rgt.getMatchingChars().size()
        );
    }

    charsGroup::charsGroup()
        : _charsGroupPattern("")
    {

    }

    charsGroup::charsGroup(string groupPattern)
        : _charsGroupPattern(groupPattern)
    {
        _internalParse();
    }

    charsGroup::charsGroup(const charsGroup & rgt)
        : _charsGroupPattern(rgt._charsGroupPattern),
          _matchingChars(rgt._matchingChars),
          _matchingDigits(rgt._matchingDigits),
          _matchingAlphas(rgt._matchingAlphas),
          _matchingOthers(rgt._matchingOthers)
    {
    }

    charsGroup::~charsGroup()
    {

    }

    void charsGroup::setMatchingChars(vector<string> in)
    {
        std::copy(in.begin(), in.end(), _matchingChars.begin());
    }

    int charsGroup::addMatching(string in)
    {
        if (isNumeric(in)) {
            _matchingDigits.push_back(in);
            return 1;
        }
        else if (isAlpha(in)) {
            _matchingAlphas.push_back(in);
            return 2;
        }
        else {
            _matchingOthers.push_back(in);
            return 3;
        }

        return 0;
    }

    int charsGroup::addMatchingIfNotExists(string in)
    {
        bool isDigit = isNumeric(in);
        bool isChar = isAlpha(in);
        if (isDigit && ! in_vector(in, _matchingDigits)) {
            _matchingDigits.push_back(in);
            return 1;
        }
        else if (isChar && ! in_vector(in, _matchingAlphas)) {
            _matchingAlphas.push_back(in);
            return 2;
        }
        else if (! isDigit && ! isChar && ! in_vector(in, _matchingOthers)) {
            _matchingOthers.push_back(in);
            return 3;
        }

        return 0;
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

    string charsGroup::getPattern()
    {
        return _charsGroupPattern;
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

        if (group == ":alpha:") {
            dictionnary = __mLowercaseDictionnary + "," + __mUppercaseDictionnary + "," + __mDigitDictionnary;
        }
        else if (group == ":digit:") {
            dictionnary = __mDigitDictionnary;
        }
        else if (group == ":any:") {
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
        _matchingDigits.clear();
        _matchingAlphas.clear();
        _matchingOthers.clear();

        if (_charsGroupPattern[0] == ':' && isSpecialGroup(_charsGroupPattern)) {
            // :alpha,:digit,:any

             vector<string> specialChars = getSpecialGroup(_charsGroupPattern).getMatchingChars();
             std::copy(specialChars.begin(), specialChars.end(), _matchingChars.begin());

             return _matchingChars.size();
        }

        bool isExcludeMode = (_charsGroupPattern[0] == '^');

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

            if (isExcludeMode && position == 0) {
                patternIt++;
                itBefore = patternIt;
                position++;
            }

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

                addMatching(currentCharacter);

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

                        addMatching(ss.str());
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

            addMatchingIfNotExists(currentCharacter);

            itBefore = patternIt;
            position++;
        }

        // done parsing pattern, should now sort the parsed data.
        // digits belong to the begin as it matches a small list.
        // lowercase + uppercase alphabet is next.
        // and special characters end up the list.

        if (_matchingChars.empty()) {
            std::sort(_matchingDigits.begin(), _matchingDigits.end(), &smallerThan);
            std::sort(_matchingAlphas.begin(), _matchingAlphas.end(), &smallerThan);

            vector<string> sortedAlphas = vector_merge(_matchingDigits, _matchingAlphas);

            _matchingChars = vector_merge(sortedAlphas, _matchingOthers);
        }

        if (isExcludeMode) {
            // all the characters we got as matching should be excluded. all others taken.
            vector<string> otherDict = splitParts(__mAnyDictionnary, 1);
            _matchingChars = vector_remove(otherDict, _matchingChars);
        }

        return _matchingChars.size(); // good boy
    }

}; // regexp

}; // core

}; // evias

