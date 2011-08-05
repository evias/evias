#ifndef EVIAS_CORE_NS_REGEXP
#   define EVIAS_CORE_NS_REGEXP

#include <map>
#include <string>
#include <vector>

#include <algorithm>

#include <boost/regex.hpp>

namespace evias {

namespace core {

    typedef enum {

        SYNTAX_OK       = 1,
        PARSE_DONE      = 0, // success
        PARSE_FAILED    = -1, // ..
        DATA_MISS       = -2, // input data miss
        SYNTAX_ERROR    = -3

    } parseReturns;

    typedef std::map<int, std::string>  indexed_matches;

    /**
     * @brief
     * regular expression class based on boost::regex.
     * this class handles the boost object and allows an easier
     * access to the different kinds of data the regular
     * expressions parse encounters. Also error messages
     * are thrown more specifically.
     *
     * @package evias::core
     **/
    class regex
    {
    public :

        // pattern(,value)
        regex(std::string);
        regex(std::string,std::string);
        regex(boost::regex);
        regex(boost::regex, std::string);

        // copy
        regex(const regex&);

        virtual ~regex() {};

        int parse(std::string = "");

        indexed_matches getIndexedMatches();

        void setPattern(std::string);
        inline void setValue(std::string v)
            { _value = v; }

        inline int setReturnCode(int c)
            { return (_return = c); }

        inline std::string getPattern() const
            { return _pattern; }
        inline std::string getValue() const
            { return _value; }
        inline boost::regex getOrigin() const
            { return _origin; }
        inline int lastReturnCode() const
            { return _return; }

    protected :

        boost::regex _origin;

        int     _return;

        indexed_matches _imatches;

        std::string  _pattern;
        std::string  _value;
    };

}; // core

}; // evias

#endif
