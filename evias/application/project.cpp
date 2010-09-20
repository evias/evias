#include "project.hpp"

namespace evias {

namespace application {

    Project::Project (string name)
        : _name (name)
    {
        _initDate = Date::now();
    }

    Project::~Project ()
    {
    }

}; // end namespace application

}; // end namespace evias

