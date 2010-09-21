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

            vector<string> getSections()
            { return _sections; }

            vector<iniSection> getIniSections()
            { return _iniSections; }

            vector<string> getLines()
            { return _lines; }

            int getLinesCount()
            { return _countLines; }

            int getSectionsCount()
            { return _countSections; }

            int getPairsCount()
            { return _countPairs; }

            bool state ()
            { return _state; }

            int changeValue (string, string, string);

            string getValue (string, string);

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

    class debugIni {
        public :

            debugIni (configIni* iniDebugged)
                : _iniDebugged(iniDebugged)
            {

            }

            ~debugIni () { };

            inline void vs_linesDump() {
                cout << endl << endl
                     << "-- [configIni Line Dump]"
                     << endl;

                vector<string> toPrint = _iniDebugged->getLines();
                vector<string>::iterator itSec = toPrint.begin();
                string lineToPrint = "";
                for (int i = 0; itSec != toPrint.end(); itSec++, i++) {

                    lineToPrint = "Index: " + evias::core::intToString(i) + " | " + "Label: " + (*itSec);

                    cout << lineToPrint << endl;
                }

                cout << endl
                     << "-- [configIni End Line Dump]"
                     << endl;
            }

            inline void vs_sectionsDump() {
                cout << endl << endl
                     << "-- [configIni Section Dump]"
                     << endl;

                vector<string> toPrint = _iniDebugged->getSections();
                vector<string>::iterator itSec = toPrint.begin();
                string lineToPrint = "";
                for (int i = 0; itSec != toPrint.end(); itSec++, i++) {

                    lineToPrint = "Index: " + evias::core::intToString(i) + " | " + "Label: " + (*itSec);

                    cout << lineToPrint << endl;
                }

                cout << endl
                     << "-- [configIni End Section Dump]"
                     << endl;
            }

            inline void mss_pairsDump() {
                cout << endl << endl
                     << "-- [configIni Pairs Dump]"
                     << endl;

                vector<iniSection> iniSections = _iniDebugged->getIniSections();
                map<string,string> currentSectionPairs;
                vector<iniSection>::iterator itSec = iniSections.begin();
                map<string,string>::iterator itPairs;
                string lineToPrint = "";
                for (int i = 0; itSec != iniSections.end(); itSec++, i++) {

                    lineToPrint = "-- Index: " + evias::core::intToString(i) + " | Section: " + (*itSec).getLabel();

                    cout << lineToPrint << endl;

                    currentSectionPairs = (*itSec).getPairs();
                    itPairs = currentSectionPairs.begin();
                    for (int j = 0; itPairs != currentSectionPairs.end(); itPairs++, j++) {

                        lineToPrint = "---- Index: " + evias::core::intToString(j) + " | Pair [" + (*itPairs).first + " => " + (*itPairs).second + "]";

                        cout << lineToPrint << endl;
                    }
                }

                cout << endl
                     << "-- [configIni End Pairs Dump]"
                     << endl;
            }

        private :
            configIni* _iniDebugged;
    };

}; // end namespace core

}; // end namespace evias

#endif

