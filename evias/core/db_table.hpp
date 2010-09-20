#ifndef __EVIAS_CORE_DBTABLE__
#define __EVIAS_CORE_DBTABLE__

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

#include <pqxx>

#include "db_adapter.hpp"
#include "db_row.hpp"
#include "sql_queries.hpp"
#include "common_utils.hpp"

namespace evias {

namespace core {

    using namespace std;

    /**
     * @example:
     *
     * - object use :
     *  pSqlAdapter* adapter = new pSqlAdapter (host, user, pass, db);
     *  dbTable<pSqlAdapter>* table = new dbTable<pSqlAdapter>(adapter, "tablename");
     *
     * - static use :
     *  dbTable<pSqlAdapter>::setDefaultAdapter(adapter);
     *  int count = dbTable<pSqlAdapter>::fetchAll("tablename");
     *  for (int i = 0; i < dbTable<pSqlAdapter>::currentRows.size(); i++)
     *      cout << dbTable<pSqlAdapter>::currentRows[i].valueOf["column_name"] << endl;
     */
    template <class __adapterType>
    class dbTable {

        public :

            dbTable ();

            dbTable (__adapterType* adapter, string tableName = "", string schema="public");

            ~dbTable();

            // PUBLIC API

            // getters / setters
            void    setSchema(string schema)
                { _schema = schema; };
            string  schema()
                { return _schema; };

            string tableName()
                { return _schema.append(".").append(_tableName); };

            void setTableName (string tableName, string schema = "public")
                { _tableName = tableName; _schema = schema; }

            vector<string> fieldNames()
                { if (getAdapter() != NULL) _initFieldNames(); return _fieldNames; }

            __adapterType* getAdapter();

            vector<dbRow> fetch (selectQuery*);
            int update (updateQuery*);
            int remove (removeQuery*);
            int insert (insertQuery*);

            // static api

            static void             setDefaultAdapter(__adapterType* db);
            static __adapterType*   defaultAdapter();

            static vector<dbRow>    fetchAll(string tableName, string sqlConditions = "TRUE", string orderBy = "");

            static string getLastQuery ();

        private :

            void _initFieldNames();

            string                              _tableName;
            string                              _schema;
            vector<string>                      _fieldNames;

            static __adapterType*               _adapter;

            static map<string, vector<dbRow> >  _cache;
            static map<Date*, string>           _history;
    };


    // STATIC INIT


    template <class __adapterType>
    map<string, vector<dbRow> > dbTable<__adapterType>::_cache;

    template <class __adapterType>
    map<Date*, string> dbTable<__adapterType>::_history;

    template <class __adapterType>
    __adapterType* dbTable<__adapterType>::_adapter;


    // STATIC API IMPLEMENTATION


    template <class __adapterType>
    void dbTable<__adapterType>::setDefaultAdapter (__adapterType* adapter)
    {
        dbTable<__adapterType>::_adapter = adapter;
    }

    template <class __adapterType>
    __adapterType* dbTable<__adapterType>::defaultAdapter ()
    {
        return dbTable<__adapterType>::_adapter;
    }

    template <class __adapterType>
    string dbTable<__adapterType>::getLastQuery ()
    {
        return (*(_history.rbegin())).second;
    }

    /**
     * @method fetchAll
     * @access public static
     *
     * fetch all entries of a relation
     *
     * @param sqlConditions std::string define specific conditions
     * @param orderBy       std::string define order state (ex.: "field_name DESC")
     *
     * @return void
     */
    template <class __adapterType>
    vector<dbRow> dbTable<__adapterType>::fetchAll (string tableName, string sqlConditions, string orderBy)
    {
        vector<dbRow> rows;

        if (sqlConditions.empty())
            sqlConditions = "TRUE";

        if (tableName.empty() || dbTable<__adapterType>::defaultAdapter() == NULL)
            return rows;

        selectQuery* query = new selectQuery;

        query->select   ("*")
             ->from     (tableName)
             ->where    (sqlConditions);

        if (! orderBy.empty()) {
            query->orderBy (orderBy);
        }

        // get MD5 hash from query
        using evias::core::md5Hash;
        string queryHash = md5Hash (query->toString());

        map<string,vector<dbRow> >::iterator cacheIt = dbTable<__adapterType>::_cache.find (queryHash);

        if ( cacheIt == dbTable<__adapterType>::_cache.end () ) {
            // needs to fetch query

            // execute query
            pqxx::result rawData;

            rawData = dbTable<__adapterType>::defaultAdapter()->doQuery( query->toString() );

            int sizeRaw = (int) rawData.size();
            for (int i = 0; i < sizeRaw ; i++) {
                dbRow row;
                row(rawData[i]);
                rows.push_back(row);
            }

            dbTable<__adapterType>::_cache.insert (make_pair( queryHash, rows ));
        }

        return dbTable<__adapterType>::_cache[queryHash];
    }


    // PUBLIC API IMPLEMENTATION

    template <class __adapterType>
    dbTable<__adapterType>::dbTable ()
    {
        if (getAdapter() != NULL)
            _initFieldNames();

        _tableName = "information_schema";
        _schema    = "public";
    }

    template <class __adapterType>
    dbTable<__adapterType>::dbTable (__adapterType* adapter, string tableName, string schema)
        :   _tableName(tableName),
            _schema(schema)
    {
        dbTable<__adapterType>::setDefaultAdapter (adapter);

        if (schema.empty())
            // missuse
            _schema = "public";

        if (getAdapter() != NULL)
            _initFieldNames();
    }

    template <class __adapterType>
    dbTable<__adapterType>::~dbTable ()
    {
    }

    template <class __adapterType>
    __adapterType* dbTable<__adapterType>::getAdapter()
    {
        return dbTable<__adapterType>::_adapter;
    }

    /**
     * @method fetch ()
     *
     * fetch row(s) for a query object
     *
     * @param queryObj selectQuery*   select query object
     *
     * @return vector<dbRow>
     */
    template <class __adapterType>
    vector<dbRow> dbTable<__adapterType>::fetch (selectQuery* queryObj)
    {
        string query = queryObj->toString ();
        vector<dbRow> outputRows;

        _history.insert ( make_pair (Date::now (), query) );

        // XXX validate / filter query (injections, etc..)

        if (getAdapter() == NULL)
            return outputRows;

        // get MD5 hash from query
        using evias::core::md5Hash;
        string queryHash = md5Hash (query);

        map<string,vector<dbRow> >::iterator cacheIt = dbTable<__adapterType>::_cache.find (queryHash);

        // execute query only if not data cached
        if (cacheIt == dbTable<__adapterType>::_cache.end()) {
            pqxx::result rawData;

            rawData = getAdapter()->doQuery( query );

            int sizeRaw = (int) rawData.size();

            for (int i = 0; i < sizeRaw ; i++) {
                dbRow row;

                // bind row's data
                row(rawData[i]);
                outputRows.push_back(row);
            }
        }

        return _cache[query];
    }

    /**
     * @method update ()
     *
     * update row(s) by an updateQuery
     *
     * @param queryObj updateQuery*   update query object
     *
     * @return int  count of lines updated
     */
    template <class __adapterType>
    int dbTable<__adapterType>::update (updateQuery* queryObj)
    {
        string query = queryObj->toString ();

        _history.insert ( make_pair (Date::now (), query) );

        if (_tableName.empty() || getAdapter() == NULL)
            return 0;

        int cntRows = 0;

        pqxx::result rawData;

        rawData = getAdapter()->doQuery (query);

        cntRows = rawData.size();

        return cntRows;
    }

    template <class __adapterType>
    int dbTable<__adapterType>::remove (removeQuery* queryObj)
    {
        string query = queryObj->toString ();

        _history.insert (make_pair (Date::now(), query));

        if (getAdapter() == NULL)
            return 0;

        int cntRows = 0;

        pqxx::result rawData;

        rawData = getAdapter ()->doQuery (query);

        cntRows = rawData.size();

        return cntRows;
    }

    template <class __adapterType>
    int dbTable<__adapterType>::insert (insertQuery* queryObj)
    {
        string query = queryObj->toString ();

        _history.insert (make_pair (Date::now(), query));

        if (getAdapter() == NULL)
            return 0;

        getAdapter()->doQuery (query);

        return 1;
    }

    /**
     *
     * PRIVATE API
     *
     */

    template <class __adapterType>
    void dbTable<__adapterType>::_initFieldNames ()
    {
        _fieldNames = getAdapter()->fieldNamesOf (_tableName);
    }

}; // end namespace core

}; // end namespace evias

#endif

