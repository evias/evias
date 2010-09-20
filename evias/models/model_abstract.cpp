#include "model_abstract.hpp"

namespace evias {

namespace model {

    abstract::abstract ()
    {
        pSqlAdapter* db = new pSqlAdapter ("ischool", "dev", "opendev", "dev.evias.be");

        dbTable<pSqlAdapter>::setDefaultAdapter (db);

        dbTable<pSqlAdapter>();
    }

    abstract::~abstract ()
    {

    }

}; // model

}; // evias

