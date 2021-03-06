#ifndef __EVIAS_APPLICATION_NS_PROJECT__
#define __EVIAS_APPLICATION_NS_PROJECT__

#include "../application/console.hpp"
#include "../core/config_ini.hpp"
#include "../core/date.hpp"

#include <string>
#include <cstdlib>
#include <ctime>

namespace evias {

namespace application {

    using namespace evias::core;
    using namespace std;

    class Project
    {
        public :
            Project (string);

            ~Project ();

            virtual int bootstrap (int, char**) = 0;
            virtual int execute   () = 0;
            virtual int shutdown  () = 0;

            string getName()
                { return _name; }

            Date* getInitTime ()
                { return _initDate; }

            Date* getExecTime ()
                { return _execDate; }

            consoleParser* getCallParser ()
                { return _consoleParser; }

            configIni* getConfig ()
                { return (_projectConfig != NULL ? _projectConfig : (_projectConfig = new configIni())); }

            void setName(string n)
                { _name = n; }

            void setCallParser(consoleParser* p)
                { _consoleParser = p; } 

            void setConfig(configIni* c)
                { _projectConfig = c; }

        protected :

            string          _name;

            consoleParser*  _consoleParser;
            configIni*      _projectConfig;

            Date*           _initDate;
            Date*           _execDate;

    };

}; // end namespace application

}; // end namespace evias

#endif
