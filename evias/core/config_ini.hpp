#ifndef __EVIAS_CORE_NS_CONFIGINI__
#define __EVIAS_CORE_NS_CONFIGINI__

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include "common_utils.hpp"
#include "string_utils.hpp"

namespace evias {

namespace core {

    using namespace std;

    struct iniSection {
        private :
            string _section;
            map<string,string> _pairs;

            bool _hasChanged;

        public :

            map<string,string> getPairs()
                { return _pairs; }

            string getLabel()
                { return _section; }

            void setLabel (string lab)
                { _hasChanged = true; _section = lab; }

            void setPairs (map<string,string> pairs)
                { _hasChanged = true; _pairs = pairs; }

            inline bool hasChanged()
            {
                if (! _hasChanged)
                    // may be not set yet
                    _hasChanged = false;

                return _hasChanged;
            }
    };

    class configIni
    {
        public :

            configIni () {};

            configIni (string);

            ~configIni () {};

            int parse ();

            iniSection          getSection(string);
            vector<string>      getSectionLabels();
            vector<iniSection>  getIniSections();
            vector<string>      getLines();

            int     getLinesCount();
            int     getSectionsCount();
            int     getPairsCount();
            bool    state ();

            int changeValue (string, string, string);
            string getValue (string, string);

            /* retrieve several values of one section */
            map<string,string> getValues(string, vector<string>);

        protected :

            int _parseFile ();
            int _parseSections ();
            int _parsePairs ();

        private :

            string                      _file;

            int                         _countLines;
            int                         _countSections;
            int                         _countPairs;

            vector<string>              _lines;
            vector<string>              _sections;
            map<string,vector<string> > _linesBySection;
            vector<iniSection>          _iniSections;

            bool                        _state;
    };

}; // end namespace core

}; // end namespace evias

#endif

