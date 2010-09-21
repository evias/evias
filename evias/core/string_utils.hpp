#ifndef EVIAS_CORE_STRING_UTILS_HPP
#define EVIAS_CORE_STRING_UTILS_HPP

#include <string> 		// string
#include <algorithm> 	// transform
#include <sstream> 		// stringstream
#include <functional>   // ptr_fun, toupper, tolower
#include <vector>
#include <map>

#include <cstdlib> // atoi

namespace evias {

namespace core {

    using namespace std;

    // eViasString errorCodes
    enum errorCodes {
        SUCCESS				= 0,
        ERR_NOT_A_STRING	= 10
    };

    enum strPadType {
        PAD_RIGHT,
        PAD_LEFT,
        PAD_BOTH
    };

    int stringToInt(string);

    // int to string conversion
    string intToString(int);

    string strToLower(string);

    string strToUpper(string);

    // Generate and return random password
    // @uses rand() .. srand() must be initialized before
    // @param int length > Password length
    string genPassword(int length);

    // Append/prepend a character(-set) to a given string
    string padString(const string &s, int pLen, string pStr, strPadType pType=PAD_LEFT);

    vector<string> split (string, unsigned char);

    string assemble (vector<string>, string);
    string assemble (map<string,string>, string, string);

	void trim (string&, const char*);

	void trimLeft (string&, const char*);
	void trimRight (string&, const char*);

    string stripTags (string);
    string stripTags (string, string[]);

}; // end namespace core

}; // end namespace evias

#endif
