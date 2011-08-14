#include "regexp.hpp"

namespace evias {

namespace core {

    using std::string;
    using std::vector;
    using std::map;
    using boost::xpressive::sregex;
    using boost::xpressive::smatch;
    using boost::xpressive::regex_match;

    regex::regex(string p)
        : _noMatchThrows(false)
    {
        setPattern(p);
    }
    regex::regex(string p, string v)
        : _value(v),
          _noMatchThrows(false)
    {
        setPattern(p);
        parse();
    }
    regex::regex(const regex& rgt)
        : _pattern(rgt._pattern),
          _value(rgt._value),
          _noMatchThrows(false)
    {
        setPattern(rgt._pattern);
    }

    int regex::parse(string v)
    {
        _imatches.clear();
        _nmatches.clear();

        if (v.empty() && _value.empty())
            return setReturnCode((int) DATA_MISS);
        else if (! v.empty())
            _value = v;

        smatch matches;
        if (! regex_match(_value, matches, _origin)) {
            if (_noMatchThrows) {
                setReturnCode((int) PARSE_FAILED);
                throw(noMatchException());
            }

            return setReturnCode((int) PARSE_FAILED);
        }

        if (_noMatchThrows && matches.size() == 0) {
            setReturnCode((int) PARSE_FAILED);
            throw(noMatchException());
        }

        for (int i = 0, c = matches.size(); i < c; i++) {
            string match = matches[i];

            _imatches.insert(std::pair<int, string>(i, match));
        }

        _computeNamedMatches();

        return setReturnCode((int) PARSE_DONE);
    }

    void regex::setGroups(vector<string> v)
    {
        _names.clear();
        for (vector<string>::iterator i = v.begin();
             i != v.end(); i++) {

            _names.push_back((*i));
        }
    }

    void regex::setPattern(string p)
    {
        try {
            _origin = sregex::compile(p);
            _pattern = p;

            setReturnCode((int) SYNTAX_OK);
        }
        catch (boost::xpressive::regex_error &e) {
            setReturnCode((int) SYNTAX_ERROR);
            throw(syntaxError());
        }
    }

    void regex::_computeNamedMatches()
    {
        if (_return == PARSE_FAILED) {
            return ;
        }

        for (int i = 0, j = 1, c = _imatches.size(), d = _names.size();
             i < c; i++, j++) {

            string name;
            if (i == 0) {
                name = "__auto__entire_match";
            }
            else if (j-1 <= d) {
                name = _names[j-2];
            }
            else {
                name = string("__auto__group_") + (intToString(i+1));
            }

            _nmatches.insert(std::pair<string,string>(name, _imatches[i]));
                
        }
    }

}; // core

}; // evias

    
