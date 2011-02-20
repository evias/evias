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

    charsGroup eRegExp::getGroup(int pos)
    {
        if (pos <= _groupsByPosition.size()-1) {
            return _groupsByPosition[pos];
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
        int i = 0;
        for (bool doEscape = false; patternIt != _pattern.end(); patternIt++, currentCharPos++, i++) {

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

            if (isNumeric(currentCharacter) || isAlpha(currentCharacter)) {

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
            // XXX else if (currentCharacter == "{")
            // XXX else if (isOperator(currentCharacter)
            else {

                return (int) NOT_SUPPORTED;
            }

            _groupsByPosition.insert(make_pair(i, groupForPos));
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

        return (int) NOT_SUPPORTED;
    }

}; // regexp

}; // core

}; // evias

