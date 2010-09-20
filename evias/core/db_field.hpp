/**
 * Package: 	iSchool Core
 *
 * Implemented Namespaces:
 * 	- eVias
 *
 * Implemented Classes:
 * 	- dbField
 * 	- pSqlAdapter
 *
 * Implemented Functions:
 *  - static string quoteInto (string data)     : quote a value into an SQL valid value
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */


#ifndef __EVIAS_NS_DBFIELD_HPP__
#define __EVIAS_NS_DBFIELD_HPP__

#include <string>

using namespace std;

namespace evias {

namespace core {

    class dbField {
        public :

            dbField() {
                _outputData = "";
                _fieldName  = "";
            }

            dbField(const dbField& f)
                : _outputData (f._outputData), _fieldName(f._fieldName)
            {
            }

            ~dbField();

            // PUBLIC API

            void    setFieldName(string n) { _fieldName = n; };
            string  fieldName   () { return _fieldName; };

            // STATIC API
            static string quoteInto(string data);

        private :

            string      _outputData;
            string      _fieldName;

    };

}; // end namespace core

}; // end namespace evias

#endif

