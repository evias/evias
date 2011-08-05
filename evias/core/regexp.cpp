#include "regexp.hpp"

namespace evias {

namespace core {

    using std::string;

    regex::regex(string p)
    {
        setPattern(p);
    }
    regex::regex(string p, string v)
        : _value(v)
    {
        setPattern(p);
        parse();
    }
    regex::regex(boost::regex r)
    {
        setPattern(r.str());
    }
    regex::regex(boost::regex r, string v)
        : _value(v)
    {
        setPattern(r.str());
        parse();
    }
    regex::regex(const regex& rgt)
        : _pattern(rgt._pattern),
          _value(rgt._value)
    {
        setPattern(rgt._origin.str());
    }

    int regex::parse(string v)
    {
        if (v.empty() && _value.empty())
            return setReturnCode((int) DATA_MISS);
        else if (! v.empty()) {
            _value = v;
        }

        boost::cmatch matches;
        if (! boost::regex_match(_value.c_str(), matches, _origin)) {
            return setReturnCode((int) PARSE_FAILED);
        }

        for (int i = 0, c = matches.size(); i < c; i++) {

            string match(matches[i].first, matches[i].second);
            _imatches.insert(std::pair<int, string>(i, match));
        }

        return setReturnCode((int) PARSE_DONE);
    }

    void regex::setPattern(string p)
    {
        try {
            _origin.assign(p);
            _pattern = p;

            setReturnCode((int) SYNTAX_OK);
        }
        catch (boost::regex_error &e) {
            setReturnCode((int) SYNTAX_ERROR);
        }
    }

}; // core

}; // evias

