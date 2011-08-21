#ifndef EVIAS_CORE_NS_REGEXP
#   define EVIAS_CORE_NS_REGEXP

#include <map>
#include <string>
#include <vector>

#include <algorithm>

#include <boost/xpressive/xpressive.hpp>

#include "string_utils.hpp"

namespace evias {

namespace core {

    typedef enum {

        SYNTAX_OK       = 1,
        PARSE_DONE      = 0, // success
        PARSE_FAILED    = -1, // ..
        DATA_MISS       = -2, // input data miss
        SYNTAX_ERROR    = -3,
        NO_MATCHES      = -4

    } parseReturns;

    class syntaxError : public std::exception
    {
        public :
            virtual const char* what () const throw()
                { return "Syntax error in your pattern."; }
    };

    class noMatchException : public std::exception
    {
        public :
            virtual const char* what () const throw()
                { return "No matches found."; }
    };

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
        typedef map<int, std::string>           indexed_matches;
        typedef map<std::string, std::string>   named_matches;

        // pattern(,value)
        regex(std::string);
        regex(std::string,std::string);

        // copy
        regex(const regex&);

        virtual ~regex() {};

        int parse(std::string = "");

        void setGroups(std::vector<std::string>);

        void setPattern(std::string);
        inline void setValue(std::string v)
            { _value = v; }

        inline int setReturnCode(int c)
            { return (_return = c); }

        inline indexed_matches getIndexedMatches()
            { return _imatches; }
        inline named_matches getNamedMatches()
            { return _nmatches; }

        inline std::string getPattern()
            { return _pattern; }
        inline std::string getValue()
            { return _value; }
        inline boost::xpressive::sregex getOrigin()
            { return _origin; }
        inline int lastReturnCode()
            { return _return; }

        inline void setNoMatchThrows(bool t)
            { _noMatchThrows = t; }

    protected :

        void _computeNamedMatches();

        boost::xpressive::sregex _origin;

        int     _return;
        bool    _noMatchThrows;

        indexed_matches _imatches;
        named_matches   _nmatches;

        std::vector<std::string> _names;

        std::string  _pattern;
        std::string  _value;
    };

    namespace containers {
        typedef evias::core::regex::indexed_matches imatches;
        typedef evias::core::regex::named_matches   nmatches;
    }

}; // core

}; // evias

#endif
