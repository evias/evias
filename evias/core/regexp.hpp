#ifndef EVIAS_CORE_NS_REGEXP
#   define EVIAS_CORE_NS_REGEXP

#include <map>
#include <string>
#include <vector>

#include <algorithm>

#include "regexp_chars_group.hpp"
#include "regexp_counter.hpp"

namespace evias {

namespace core {

namespace regexp {

    typedef enum {

        PARSE_DONE      = 1, // success
        NOT_SUPPORTED   = 0,
        PARSE_FAILED    = -1, // ..
        DATA_MISS       = -2, // input data miss
        SYNTAX_ERROR    = -3

    } parseReturns;

    const string expressionStarters = "([{";
    const string expressionEnders   = ")]}";
    const string expressionOperators= "+?";

    class eRegExp
    {
    public :

        // pattern,value
        eRegExp(string,string);
        ~eRegExp();

        map<int,string> parse();
        map<int,string> parse(string,string);

        void setPattern(string);
        void setValue(string);

        string getPattern();
        string getValue();
        map<string,string> getNamedMatches();
        map<int, string>   getMatches();

        map<int,charsGroup> getGroups();
        charsGroup          getGroup(int);

        bool isExpressionStarter(string);
        bool isExpressionEnder(string);
        bool isOperator(string);

    private :

        int _parsePattern();
        int _parseValue();

        string  _pattern;
        string  _value;

        map<int, string>    _matches;
        map<string,string>  _namedMatches;

        map<int, charsGroup> _groupsByPosition;
    };

};

}; // core

}; // evias

#endif

