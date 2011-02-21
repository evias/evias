#include "regexp.hpp"

namespace evias {

namespace core {

namespace regexp {

/*****
 * PUBLIC
 *****/

    eRegExp::eRegExp(string pattern, string value)
        : _pattern(pattern), _value(value)
    {
        _parsePattern();
        _parseValue();
    }

    eRegExp::~eRegExp()
    {

    }

    void eRegExp::setPattern(string pattern)
    {
        _pattern = pattern;
    }

    string eRegExp::getPattern()
    {
        return _pattern;
    }

    void eRegExp::setValue(string value)
    {
        _value = value;
    }

    string eRegExp::getValue()
    {
        return _value;
    }

    map<string,string> eRegExp::getNamedMatches()
    {
        return _namedMatches;
    }

    map<int,string> eRegExp::getMatches()
    {
        return _matches;
    }

    map<int,string> eRegExp::parse()
    {
        _parsePattern();
        _parseValue();

        return getMatches();
    }

    map<int,string> eRegExp::parse(string pattern, string value)
    {
        setPattern(pattern);
        setValue(value);

        _parsePattern();
        _parseValue();

        return getMatches();
    }

    map<int,charsGroup> eRegExp::getGroups()
    {
        return _groupsByPosition;
    }

    charsGroup eRegExp::getGroupAt(int pos)
    {
        map<int,charsGroup> groups = getGroups();

        if (pos < groups.size()) {
            return groups[pos];
        }

        charsGroup group;
        return group;
    }

    bool eRegExp::isExpressionStarter(string character)
    {
        if (character.size() > 1) {
            return false;
        }

        return (expressionStarters.find(character) != string::npos);
    }

    bool eRegExp::isExpressionEnder(string character)
    {
        if (character.size() > 1) {
            return false;
        }

        return (expressionEnders.find(character) != string::npos);
    }

    bool eRegExp::isOperator(string character)
    {
        if (character.size() > 1) {
            return false;
        }

        return (expressionOperators.find(character) != string::npos);
    }

/******
 * PRIVATE
 ******/

    int eRegExp::_parsePattern()
    {
        if (_pattern.empty()) {
            return (int) DATA_MISS;
        }

        _groupsByPosition.clear();

        string::iterator patternIt = _pattern.begin();
        string::iterator beforeIt = patternIt;
        int currentCharPos = 0;
        int position = 0;
        int positionOccurences = 0;
        charsGroup infinityMatchingGroup;
        charsGroup lastGroup;

        for (bool doEscape = false, lastWasGroup = false, hasGroupInfinity = false, infinityAndEmpty;
             patternIt != _pattern.end();
             patternIt++, currentCharPos++, position++) {

            stringstream ssbefore;
            ssbefore << *beforeIt;

            stringstream ss;
            ss << *patternIt;

            string currentCharacter = ss.str();
            string characterBefore  = ssbefore.str();

            beforeIt = patternIt;

            // check type of character. if it is a simple character
            // or a numeric value, the corresponding group can be
            // created, if not it may be parsed by other classes
            // such as charsGroup and occurenceCounter.

            string groupPattern;
            charsGroup groupForPos;

            if (positionOccurences > 0) {

                groupForPos.setPattern(lastGroup.getPattern());
                positionOccurences--;
                position--;
                currentCharPos--;
                patternIt--; // should not go further until the occurences
                             // for the multiplied position have all been set
            }
            else if (isNumeric(currentCharacter) || isAlpha(currentCharacter)) {

                groupPattern = currentCharacter;
                groupForPos.setPattern(groupPattern);
            }
            else if (characterBefore == "\\" && doEscape) {

                // XXX may be a special charsGroup
                //     \d,\S,\w ...

                doEscape     = false;
                groupPattern = currentCharacter;
                groupForPos.setPattern(groupPattern);
            }
            else if (currentCharacter == "\\" && ! doEscape) {

                doEscape = true;
                continue;
            }
            else if (currentCharacter == "[") {
                // has to parse a charsGroup for this position

                size_t closingBracket = _pattern.find("]", currentCharPos);

                if (closingBracket != string::npos) {
                    // can copy content

                    groupPattern = _pattern.substr(currentCharPos + 1, closingBracket - currentCharPos - 1);
                    groupForPos.setPattern(groupPattern);

                    patternIt = patternIt + (closingBracket - currentCharPos);
                    currentCharPos = closingBracket;
                    lastWasGroup = true;
                }
                else { // missing closing bracket

                    return (int) SYNTAX_ERROR;
                }
            }
            else if (currentCharacter == ".") {
                // ANY for this position
                groupPattern = ":any:";
                groupForPos.setPattern(groupPattern);
            }
            else if (! isExpressionStarter(currentCharacter) && ! isExpressionEnder(currentCharacter) && ! isOperator(currentCharacter)) {
                groupPattern = currentCharacter;
                groupForPos.setPattern(groupPattern);
            }
            else if (isOperator(currentCharacter) && lastWasGroup) {

                // last group can possibly be applied to multiple positions

                hasGroupInfinity   = true;
                infinityAndEmpty   = (currentCharacter == "?");
                infinityMatchingGroup.setPattern(lastGroup.getPattern());

                lastWasGroup = false;
            }
            else if (currentCharacter == "{" && lastWasGroup) {

                size_t closingBracket = _pattern.find("}", currentCharPos);

                if (closingBracket != string::npos) {
                    // valid occurence counter
                    string occurenceCounterStr = _pattern.substr(currentCharPos + 1, closingBracket - currentCharPos - 1);

                    positionOccurences = 0;
                    if (occurenceCounterStr.find(",") != string::npos) {
                        // has list of counters ({1,2,4} => may occur once, twice or 4 times

                        // NOT IMPLEMENTED YET
                        positionOccurences = 1;
                    }
                    else {
                        positionOccurences = stringToInt(occurenceCounterStr);
                    }

                    if (positionOccurences > 0) {
                        positionOccurences--; // first position already parsed
                    }

                    patternIt = patternIt + (closingBracket - currentCharPos);
                    currentCharPos = closingBracket;
                }
            }
            else {

                return (int) NOT_SUPPORTED;
            }

            // having some infinity operators means we cannot return a reliable
            // "group by position" map, parsing the pattern is done in another way
            // as we will not match character by group for position but rather try
            // and see if the current group is somewhere next to the group being
            // infinit'd.

            int atPos = 0;
            if (position > 0) {
                map<int, charsGroup>::reverse_iterator rit = _groupsByPosition.rbegin();
                atPos = (*rit).first + 1;
            }

            if (infinityAndEmpty) {
                infinityAndEmpty = false;
                _optionalGroups.insert(make_pair(atPos-1, infinityMatchingGroup));
            }

            if (hasGroupInfinity && _groupInfinities.size() == 0) {
                // is + or ? operator

                hasGroupInfinity = false;

                _groupInfinities.insert(make_pair(atPos-1, infinityMatchingGroup));
                lastGroup = infinityMatchingGroup;
                continue; // no need to implement totalPos + insert group
            }
            else if (_groupInfinities.size() > 0) {
                hasGroupInfinity = false;

                _followingInfinities.push_back(groupForPos);
            }

            if (groupForPos.getPattern().empty()) {
                continue;
            }

            _groupsByPosition.insert(make_pair(atPos, groupForPos));

            lastGroup = groupForPos;
            _totalPositions++;
        }

        int atPos = 0;
        if ((bool) _groupsByPosition.size()) {
            map<int, charsGroup>::reverse_iterator rit = _groupsByPosition.rbegin();
            atPos = (*rit).first + 1;
        }

        // may have to copy lastGroup several times after the loop has
        // finished. (pattern "[abcd]{5}" should match 5 time [abcd], the loop
        // will terminate after parsing the closing '}' bracket and so
        // need to add 3 more entries of the same last group.
        int cnt = 0;
        while (positionOccurences > 0) {
            charsGroup groupForPos(lastGroup.getPattern());

            _groupsByPosition.insert(make_pair(atPos + cnt, groupForPos));
            positionOccurences--;
            cnt++;
        }

        return (int) PARSE_DONE;
    }

    int eRegExp::_parseValue()
    {
        if (_value.empty()) {
            return (int) DATA_MISS;
        }

        // XXX iterate through the value and see if each
        //     character matches the corresponding group

        // check _optionalGroups and _groupInfinities

        return (int) NOT_SUPPORTED;
    }

}; // regexp

}; // core

}; // evias

