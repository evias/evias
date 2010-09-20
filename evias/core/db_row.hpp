/**
 * Package: 	iSchool Core
 *
 * Implemented Namespaces: 	
 * 	- eVias
 *
 * Implemented Classes:
 * 	- dbRow
 *
 * Implemented Functions:
 *  - void operator()(pqxx::result::tuple)  : initialize a row with a PQxx pair
 *  - string operator[](int)                : get a field's value by its index
 *  - string operator[](string)             : get a field's value by its name
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */


#ifndef __EVIAS_NS_DBROW_HPP__
#define __EVIAS_NS_DBROW_HPP__

#include <string>
#include <map>
#include <vector>

#include <pqxx>

using namespace std;

namespace evias {

namespace core {

    class dbRow {
        public :
            // nothing to do
            dbRow() {
                _errorStr = "__col_name_error__";
                _errorIdx = "__col_index_error__";
            };

            // copy construction
            dbRow(const dbRow &row) {
                fields   = row.fields;
                valueOf  = row.valueOf;
                valueAt  = row.valueAt;
            };

            // nothing to do
             ~dbRow() {};

            // initialize with result row (libpqxx)
            void operator()(pqxx::result::tuple row) {
                string tmpFieldName = "";
                string tmpValue = "";
                unsigned int cntFields = (unsigned int) row.size();

                for (unsigned int i = 0; i < cntFields; i++) {
                    tmpFieldName= "";
                    tmpValue    = "";

                    tmpFieldName.append((char*) row[i].name());
                    if (row[i].is_null())
                        tmpValue = "null";
                    else
                        tmpValue.append(row[i].as<string>());

                    fields.push_back(tmpFieldName);
                    valueOf.insert(pair<string,string>(tmpFieldName, tmpValue));
                    valueAt.insert(pair<unsigned int,string>(i, tmpValue));
                }
            };

            string operator[](int iAt) {
                if (_validIndex(iAt))
                    return valueAt[iAt];

                return _errorIdx;
            };

            string operator[](string sOf) {
                if (_validIndex(sOf))
                    return valueOf[sOf];

                return _errorStr;
            };

            vector<string>              fields;
            map<string,string>          valueOf;
            map<unsigned int,string>    valueAt;

        protected :

            bool _validIndex (int iAt) {
                // index must exist
                return iAt < valueAt.size();
            }

            bool _validIndex (string sOf) {
                return ! valueOf[sOf].empty();
            }

        private :

            string  _errorStr;
            string  _errorIdx;
    };

}; // end namespace core

}; // end namespace evias

#endif

