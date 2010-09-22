#ifndef __EVIAS_NS_LOGGER__
#define __EVIAS_NS_LOGGER__

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "../core/date.hpp"

namespace evias {

namespace application {

    using namespace std;
    using evias::core::Date;

    class Logger;

    ostream& operator<< (ostream&, Logger&);

    class Logger
    {
        public :

            map<Date*, string>  logs;

            Logger ();
            ~Logger();

            Logger& log (string);

            void setFilename (string);

            const char* getFilename ()
                { return _filename.c_str(); };

            bool good ()
                { return _state == true; }

        private :

            string  _filename;
            bool    _state;

    };

}; // application

}; // evias

#endif

