#ifndef EVIAS_CORE_NS_REGEXP
#   define EVIAS_CORE_NS_REGEXP

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
        ":alpha",
        ":digit",
        ":any"
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

        void setMatchingChars(vector<string>);

        bool        isSpecialGroup(string);
        charsGroup  getSpecialGroup(string);

        vector<string> getMatchingChars();

    private :

        int _internalParse();

        // abc ; A-Z0-9 ..
        string _charsGroupPattern;

        // abcdef... ; 01234.. ; ..
        vector<string> _matchingChars;
    };

}; // regexp

}; // core

}; // evias

#endif

