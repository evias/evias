#include "logger.hpp"

namespace evias {

namespace application {

    ostream& operator<< (ostream& out, Logger& logger)
    {
        if (logger.good()) {
            // only log if possible

            map<Date*,string>::iterator itLogs = logger.logs.begin();
            for (; itLogs != logger.logs.end(); itLogs++) {
                string message = "[ " + ((*itLogs).first)->toString() + " ]";

                message.append (" " + (*itLogs).second);

                out << message << endl;
            }
        }

        return out;
    }

    Logger::Logger ()
    {
        _state = false;
    }

    Logger::~Logger ()
    {

    }

    void Logger::setFilename (string file)
    {
        _state = true;

        ifstream readTest;
        readTest.open(file.c_str(), ios::in);
        if (! readTest.good())
            _state = false;
        readTest.close();

        ofstream writeTest;
        writeTest.open(file.c_str(), ios::app);
        if (! writeTest.good())
            _state = false;
        writeTest.close();

        _filename = file;
    }

    Logger& Logger::log (string msg)
    {
        logs.insert (make_pair (Date::now(), msg));

        return *this;
    }

}; // application

}; // evias

