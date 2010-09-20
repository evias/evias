#ifndef __EVIAS_ISCHOOL_MODEL_ABSTRACT__
#define __EVIAS_ISCHOOL_MODEL_ABSTRACT__

#include "../core/date.hpp"
#include "../core/db_adapter.hpp"
#include "../core/db_table.hpp"

namespace evias {

namespace model {

    using evias::core::dbTable;
    using evias::core::pSqlAdapter;
    using evias::core::dbRow;
    using evias::core::dbField;

    class abstract
        : public dbTable<pSqlAdapter>
    {
        public :

            abstract  ();

            ~abstract ();

            virtual abstract* loadById (int) = 0;
    };

}; // model

}; // evias

#endif // __EVIAS_ISCHOOL_MODEL_ABSTRACT__
