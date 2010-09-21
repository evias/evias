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

