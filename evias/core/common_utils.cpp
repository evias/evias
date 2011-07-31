#include "common_utils.hpp"
#include <openssl/md5.h>

namespace evias {

namespace core {

    // debug method for map<string,string>
    // @todo implement in debug namespace
    void s_mapDump(map<string,string> mMap) {
        map<string,string>::iterator it = mMap.begin();

        cout << "----Map Dump" << endl;
        for( ; it != mMap.end(); it++) {
            cout << "['" << (*it).first << "'] '" << (*it).second << "'" << endl;
        }
        cout << "----End Dump" << endl;
    }

    // debug method for vector<string>
    // @todo implement in debug namespace
    void s_vecDump(vector<string> vVec) {
        vector<string>::iterator it = vVec.begin();

        cout << "----Vector Dump" << endl;
        for(int i = 0 ; it != vVec.end(); it++, i++) {
            cout << "[" << evias::core::intToString(i) << "] = '" << (*it) << "'" << endl;
        }
        cout << "----End Dump" << endl;
    }

    string md5Hash(string src) {
        int   len    = src.size();
        char* compat = new char[len];
        unsigned char* md5d = new unsigned char[256];

        for (int i=0; i<len; i++)
            compat[i] = src[i];
        compat[len-1] = '\0';

        MD5( (unsigned char*) compat, len, md5d );

        // hash human-readability (hexa hash)
        string hash;
        char* hashBuf = new char [34];
        char* hashTmp = new char [10];
        for(int i=0; i < MD5_DIGEST_LENGTH; i++) {
            sprintf(hashTmp, "%02x",md5d[i]);
            strcat(hashBuf, hashTmp);
        }

        hash = hashBuf;
        hash = hash.substr(1, hash.size()-1);

        delete hashBuf;
        delete hashTmp;
        delete md5d;
        delete compat;

        return hash;
    }

    void s_mapCopy (map<string,string> rhs, map<string,string>& lhs)
    {
        lhs.clear();
        map<string,string>::iterator itClone = rhs.begin();
        for (; itClone != rhs.end(); itClone++) {

            lhs.insert (make_pair ((*itClone).first, (*itClone).second));
        }
    }

    void s_vectorCopy (vector<string> rhs, vector<string>& lhs)
    {
        lhs.clear();
        vector<string>::iterator itClone = rhs.begin();
        for (; itClone != rhs.end(); itClone++) {

            lhs.push_back ((*itClone));
        }
    }

}; // end namespace core

}; // end namespace evias
