#include "string_utils.hpp"

namespace evias {

namespace core {

    int stringToInt(string s) {
        int iOut = atoi(s.c_str());

        if (iOut == 0 && s != "0") {
            return -1;
        }

        return iOut;
    }

    string intToString(int iIn)
    {
        stringstream ss;

        ss	<< iIn;

        return ss.str();
    }

    string strToLower(string s)
    {
        transform( s.begin(), s.end(), s.begin(), ptr_fun(::tolower) );

        return s;
    }

    string strToUpper(string s)
    {
        transform( s.begin(), s.end(), s.begin(), ptr_fun(::toupper) );

        return s;
    }

    string padString(const string &s, int pLen, string pStr, strPadType pType)
    {
        int i = 0, // loop variables
            j = 0,
            k = 0;

        int inSize      = s.size();     // input data size
        int pSize       = pStr.size();  // padding string size
        int roomForAdd  = pLen - inSize;// room left for prepend/append
        string paddedStr; // output data

        if (pLen <= inSize || pSize < 1)
            // nothing to do
            return s;

        paddedStr.reserve(pLen); // one time memory alloc
        if (pType == PAD_LEFT)
        {
            // pad left => first add room count of pad string
            for ( i = 0; i < roomForAdd; ) // loop while enough room
                for ( j = 0; j < pSize && i < roomForAdd; j++,i++ )
                    // loop through pad string
                    paddedStr.push_back( pStr[j] );

            // add input data
            for ( i = 0, k = inSize; i < k; i++ )
                paddedStr.push_back( s[i] );
        }
        else if (pType == PAD_RIGHT)
        {
            // pad right => first insert already known data
            for ( i = 0, k = inSize; i < k; i++)
                paddedStr.push_back( s[i] );

            // pad the string with the pad string (character(s) to add)
            for (i = inSize; i < pLen; )
                for ( j = 0; j < pSize && i < pLen; j++,i++)
                    paddedStr.push_back( pStr[j] );
        }
        else if (pType == PAD_BOTH)
        {
            // roomLeft & roomRight will differ if by example
            // pLen = 9.
            int roomLeft  = roomForAdd / 2;
            int roomRight = roomForAdd - roomLeft;

            // prepend left padding
            for ( i = 0; i < roomLeft; )
                for ( j = 0; j < pSize && i < roomLeft; j++,i++ )
                    paddedStr.push_back( pStr[j] );

            // add input data
            for ( i = 0, k= inSize; i < k; i++ )
                paddedStr.push_back( s[i] );

            // append right padding
            for ( i = 0; i < roomRight; )
                for ( j = 0; j < pSize && i < roomRight; j++,i++ )
                    paddedStr.push_back( pStr[j] );
        }

        return paddedStr;
    }

    vector<string> split( string in_strSource, unsigned char in_ucDelim )
    {
        vector< string > vsTmp;
        string strBefore;
        int i= 0, j= 0;
        bool bBreakAfterThis = false;
        do
        {
            if( ( int ) in_strSource.find( in_ucDelim, j ) == -1 )
                bBreakAfterThis = true;
            else
                bBreakAfterThis = false;

            if( !bBreakAfterThis ) {
                // continue until next delimiter position

                while( in_strSource[ i ] != in_ucDelim ) {
                    strBefore += in_strSource[ i ];
                    ++i;
                }
            }
            else {
                // continue until end of the string to split

                while( i <= ( int ) in_strSource.length( ) ) {
                    strBefore += in_strSource[ i ];
                    ++i;
                }
            }

            vsTmp.push_back( strBefore );
            strBefore = "";

            // continue loop at delim_position + 1
            i = i + 1;

            j = i;

            if( bBreakAfterThis )
                break;
        }
        while( j <= ( int ) in_strSource.length( ) );

        return ( vsTmp );
    }

    string assemble (vector<string> list, string betweenRows=", ")
    {
        string outputStr = "";
        vector<string>::iterator itList = list.begin();
        for (int i = 0; itList != list.end(); itList++, i++) {

            if (i > 0)
                outputStr.append (betweenRows);

            outputStr.append((*itList));
        }
        return outputStr;
    }

    string assemble (map<string,string> keyValuePairs, string betweenKeyValue = " as ", string betweenRows = ", ")
    {
        string outputStr = "";
        map<string,string>::iterator itPairs = keyValuePairs.begin();
        for (int i = 0; itPairs != keyValuePairs.end(); itPairs++) {
            outputStr.append ((*itPairs).first);
            outputStr.append (betweenKeyValue);
            outputStr.append ((*itPairs).second);

            if (i < (keyValuePairs.size()-1)) {
                outputStr.append (betweenRows);
            }
        }
        return outputStr;
    }


    // Generate and return random password
    // @uses rand() .. srand() must be initialized before
    // @param int length > Password length
    // @return std::string
    string genPassword(int length)
    {
        string password = "";
        int randBoolInt = -1; // 0 or 1
        int secBoolInt  = -1; // 0 or 1
        char *randPChr 	= new char[2]; // to string conversion

        for (int i = 0; i < length; i++)
        {
            randBoolInt = rand() % 2; // [0,1]
            secBoolInt  = rand() % 2; // [0,1]

            if (randBoolInt == 1) {
                if (secBoolInt == 1)
                    randPChr[0] = (char) ((rand() % 26) + 97); // [a-z]
                else
                    randPChr[0] = (char) ((rand() % 26) + 65); // [A-Z]
            }
            else {
                randPChr[0] = (char) ((rand() % 10) + 48); // [0-9]
            }

            randPChr[1] = '\0';

            password.append(randPChr);
        }
        delete randPChr;

        return password;
    }

	void trim(string& str, const char* chars2remove) {
		trimLeft(str, chars2remove);
		trimRight(str, chars2remove);
	}

	void trimLeft(string& str, const char* chars2remove) {
        string chars = chars2remove;

        bool hasCharToKeep = false;

        string::iterator itStr = str.begin();
        for (int iPos = 0; ! hasCharToKeep && itStr != str.end(); itStr++, iPos++) {
            bool isToRemove = false;

            // check if current char is a char to remove
            string::iterator itSzCp = chars.begin();
            for (int jPos = 0; ! isToRemove && itSzCp != chars.end(); itSzCp++, jPos++) {
                if ((*itSzCp) == '\\' && jPos < chars.size() - 1) {
                    // is escaped character

                    itSzCp++;
                    jPos++;
                }
                if ((*itSzCp) == (*itStr))
                    isToRemove = true;
            }

            if (isToRemove) {
                str = str.substr (iPos);
            }
            else {
                hasCharToKeep = true;
            }
        }
	}

	void trimRight(string& str, const char* chars2remove) {
        string chars = chars2remove;

        bool hasCharToKeep = false;

        string::reverse_iterator itStr = str.rbegin();
        for ( ; ! hasCharToKeep && itStr != str.rend(); ) {
            bool isToRemove = false;

            // check if current char is a char to remove
            string::iterator itSzCp = chars.begin();
            for (int jPos = 0; ! isToRemove && itSzCp != chars.end(); itSzCp++, jPos++) {
                if ((*itSzCp) == '\\' && jPos < chars.size() - 1) {
                    // is escaped character

                    itSzCp++;
                    jPos++;
                }
                if ((*itSzCp) == (*itStr))
                    isToRemove = true;
            }

            if (isToRemove) {
                str = str.substr (0, str.size() - 1);
            }
            else {
                hasCharToKeep = true;
            }

            itStr = str.rbegin();
        }
	}

    bool in_vector(string needle, vector<string> haystack)
    {
        for (int i = 0, max = haystack.size() ; i < max; i++) {
            if (haystack.at(i) == needle)
                return true;
        }

        return false;
    }

    bool isAlpha(string value)
    {
        string alphaDictionnary = (__mLowercaseDictionnary + __mUppercaseDictionnary);
        size_t firstNotAlpha = value.find_first_not_of(alphaDictionnary);

        return firstNotAlpha == string::npos;
    }

    bool isNumeric(string value)
    {
        string digitDictionnary = (__mDigitDictionnary + ".,"); // allow both, point and comma
        size_t firstNotDigit = value.find_first_not_of(digitDictionnary);

        return firstNotDigit == string::npos;
    }

    bool greaterThan(string first, string second)
    {
        if (second.size() < first.size())
            return true;

        for (string::iterator itFst = first.begin(), itSnd = second.begin(); itFst != first.end() && itSnd != second.end(); itFst++, itSnd++) {
            if (*itFst > *itSnd)
                return true;
            if (*itFst < *itSnd)
                return false;
        }

        return false;
    }

    bool smallerThan(string first, string second)
    {
        if (first.size() < second.size())
            return true;

        for (string::iterator itFst = first.begin(), itSnd = second.begin(); itFst != first.end() && itSnd != second.end(); itFst++, itSnd++) {
            if (*itFst < *itSnd)
                return true;
            if (*itFst > *itSnd)
                return false;
        }

        return false;
    }


}; // end namespace core

}; // end namespace evias
