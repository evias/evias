#ifndef EVIAS_CORE_REGEXP_NS_CHARSGROUP
#   define EVIAS_CORE_REGEXP_NS_CHARSGROUP

#include "string_utils.hpp"

#include <map>
#include <string>
#include <vector>
#include <sstream>

#include <algorithm>
#include <functional>
#include <utility>

namespace evias {

namespace core {

namespace regexp {

    const string groupSpecialKeys[] = {
        ":alpha:",
        ":digit:",
        ":any:"
    };

    // a charsGroup represent data between brackets in
    // a regular expression. by example :
    // [A-Z0-9;-\.]    => the charsGroup would be A-Z0-9;-\.
    // [abcde034]      => the charsGroup would be abcde034
    class charsGroup
    {
    public :

        charsGroup();
        charsGroup(string);
        virtual ~charsGroup();

        void setPattern(string);
        string getPattern();

        void setMatchingChars(vector<string>);

        int addMatching(string);
        int addMatchingIfNotExists(string);

        bool        isSpecialGroup(string);
        charsGroup  getSpecialGroup(string);

        vector<string> getMatchingChars();

    private :

        int _internalParse();

        // abc ; A-Z0-9 ..
        string _charsGroupPattern;

        // abcdef... ; 01234.. ; ..
        vector<string> _matchingChars;

        vector<string> _matchingDigits;
        vector<string> _matchingAlphas;
        vector<string> _matchingOthers;
    };

}; // regexp

}; // core

}; // evias

#endif

