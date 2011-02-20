#ifndef EVIAS_CORE_STRING_UTILS_HPP
#define EVIAS_CORE_STRING_UTILS_HPP

#include <string> 		// string
#include <algorithm> 	// transform
#include <sstream> 		// stringstream
#include <functional>   // ptr_fun, toupper, tolower
#include <vector>
#include <map>

#include <cstdlib> // atoi

#include "common_utils.hpp"

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

    // XXX get full utf-8 spec listing
    const string __mLowercaseAccented      = "àéèùüöäëï";
    const string __mLowercaseDictionnary   = "abcdefghijklmnopqrstuvwxyz";
    const string __mLowercaseSpecial       = "æŧþßðđŋħĸłµ¢ł";
    const string __mUppercaseDictionnary   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string __mUppercaseAccented      = "ÀÉÈÙÜÖÄËÏ";
    const string __mUppercaseSpecial       = "ÆŦØÞÐŊĦJĸŁ¢Ł";
    const string __mDigitDictionnary       = "0123456789";
    const string __mOtherDictionnary       = "\"\'\\²¬~&#{([-|`_^@)]°+=}$£¤%µ*!§/:;.?,<>€¶←↓→«»” ";

    const string __mAnyDictionnary         = (
        __mDigitDictionnary
        + __mUppercaseDictionnary
        + __mLowercaseDictionnary
        + __mUppercaseAccented
        + __mLowercaseAccented
        + __mUppercaseSpecial
        + __mLowercaseSpecial
        + __mOtherDictionnary
    );

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
    vector<string> splitParts (string, int = 1);

    string assemble (vector<string>, string);
    string assembleSQL (map<string,string>, string = " as ", string = ", ");

	void trim (string&, const char*);

	void trimLeft (string&, const char*);
	void trimRight (string&, const char*);

    bool in_vector(string, vector<string>);

    vector<string> vector_merge(vector<string>,vector<string>);
    vector<string> vector_remove(vector<string>,vector<string>);

    bool isAlpha(string);
    bool isNumeric(string);

    bool greaterThan(string,string);
    bool smallerThan(string,string);

}; // end namespace core

}; // end namespace evias

#endif
