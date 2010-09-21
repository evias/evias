#include "db_adapter.hpp"

namespace evias {

namespace core {

    pqxx::result dbAdapter::doQuery(string query)
    {
        // compute transaction key
        time_t      timestamp;
        string trxKey;
        stringstream trxSeed;
        unsigned long timeSeed;

        time (&timestamp);
        timeSeed= (unsigned long) timestamp;
        trxSeed << _trxKeyMask
                << timeSeed
                << "__";
        trxKey  = trxSeed.str();

        // output
        pqxx::result        rOut;

        if (! good()) {
            return rOut;
        }

        string errorMsg = "";

        // Psql init
        pqxx::connection    db(_connPolicy);

        if (! db.is_open()) {
            _state = false;
            return rOut;
        }

        // create transaction
        pqxx::transaction<> tr(db, trxKey);

        try { // do query
            rOut = tr.exec( query );

            tr.commit();
        }
        catch (const exception& e) {
            cout << "-- caught an error.. aborting : " << e.what() << endl;
        }

        return rOut;
    }

}; // end namespace core

}; // end namespace evias

