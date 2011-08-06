#ifndef EVIAS_CORE_COMMON_UTILS_HPP
#define EVIAS_CORE_COMMON_UTILS_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <cctype>
#include <ctime>		// time
#include <cstdlib> 		// srand, rand
#include <cstring>      // strcat
#include <cstdio>       // sprintf

#include <sys/stat.h>    // stat() method in fileExists

#include "string_utils.hpp"

namespace evias {

namespace core {

    using namespace std;

    string md5Hash(string);

    // count elements in an array of any type
    template <typename arrType, size_t arrSize>
    size_t countElm(arrType (&array)[arrSize]) {
        return arrSize;
    };

    // compare values to get the smaller one of both
    // value's types need to have operator>
    template <typename T>
    const T& max(const T& x, const T& y)
    {
        return (x > y) ? x : y;
    };

    // compare values to get the smaller one of both
    // value's types need to have operator<
    template <typename T>
    const T& min(const T& x, const T& y) {
        return (x < y) ? x : y;
    };

    void s_mapDump(map<string, string>);
    void s_vecDump(vector<string>);

    void is_mapDump(map<int, string>);

    void s_mapCopy    (map<string,string> rhs, map<string,string>& lhs);
    void s_vectorCopy (vector<string> rhs, vector<string>& lhs);

}; // end namespace core

}; // end namespace evias

#endif
