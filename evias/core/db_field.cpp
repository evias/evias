#include "db_field.hpp"

namespace evias {

namespace core {

    string dbField::quoteInto ( string data )
    {
        string returnStr = "";
        string workStr   = data;
        string newWorkStr= "";
        int quotePos = workStr.find_first_of("'");
        if (quotePos == string::npos)
            // nothing to escape
            return (returnStr + "'" + workStr + "'");
        else {
            bool finishedQuoting = false;
            int searchOffset = 0;
            do {
                quotePos = workStr.find ("'", searchOffset);

                if (quotePos == string::npos)
                    finishedQuoting = true;
                else {
                    // needs to quote

                    string beforeQuote = workStr.substr (searchOffset, quotePos);
                    string afterQuote  = workStr.substr (quotePos+1);

                    // increment searchOffset for ongoing treatment
                    searchOffset = (int) quotePos + 2;

                    returnStr = returnStr + beforeQuote + "''" + afterQuote;
                }
            }
            while (! finishedQuoting);
        }

        return "'" + returnStr + "'";
    }

}; // core

}; // evias
