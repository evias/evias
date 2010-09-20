/**
 * Package: 	iSchool Core
 *
 * Implemented Namespaces:
 * 	- eVias
 *
 * Implemented Classes:
 * 	- dbAdapter
 * 	- pSqlAdapter
 *
 * Implemented Functions:
 * 	- dbAdapter::dbAdapter(string, string, string, string) 	: construct a database adapter
 * 	- pSqlAdapter::fieldNamesOf(string, string) 		    : get the field names of a table in a PostgreSQL database
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */

#ifndef __EVIAS_NS_DBADAPTER__
#define __EVIAS_NS_DBADAPTER__

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <ctime>

#include <pqxx/pqxx>
#include "db_field.hpp"

using namespace std;

namespace evias {

namespace core {

    enum adapterType {
        __ADAPTER_POSTGRESQL__  = 10,
        __ADAPTER_MYSQL__       = 11
    };

    class dbAdapter {

        public :
            // fast transactions may want to erase all traces
            virtual ~dbAdapter() {};

            virtual vector<string> fieldNamesOf(string) {};

            // XXX implement 'args' feature
            pqxx::result doQuery(string);

            bool good() {
                return _state;
            };

            string lastError () {
                return _lastErrorMessage;
            }

        protected :
            // called only by child constructors
            dbAdapter(string dbName, string dbUser, string dbPass, string dbHost="localhost")
                : _dbName(dbName), _user(dbUser), _pass(dbPass), _host(dbHost)
            {
                string connPolicy = "dbname=";
                connPolicy.append(_dbName)
                          .append(" user=")
                          .append(_user)
                          .append(" password=")
                          .append(_pass)
                          .append(" host=")
                          .append(_host);

                _connPolicy = connPolicy;

                try {
                    // connection test at creation
                    pqxx::connection db( _connPolicy );

                    _state = db.is_open();
                }
                catch (std::exception& e) {
                    _state = false;
                    _lastErrorMessage = e.what();
                }
                catch (...) {
                    _state = false;
                }
            }
            // XXX copy constructor


        private :
            string  _dbName;
            string  _user;
            string  _pass;
            string  _host;

            string _connPolicy;

            // has to be redefined !
            string  _trxKeyMask;

            string _lastErrorMessage;

            bool _state;
    };

    class pSqlAdapter : public dbAdapter {
        public :

            pSqlAdapter(string dbName, string dbUser, string dbPass, string dbHost = "localhost")
                : dbAdapter(dbName, dbUser, dbPass, dbHost)
            {
                _trxKeyMask = "__eVias_Psql_Adapter_Transaction_";
            }

            pSqlAdapter(const pSqlAdapter& db)
                : dbAdapter(db._dbName, db._user, db._pass, db._host)
            {
                _trxKeyMask = "__eVias_Psql_Adapter_Transaction_";
            }

            virtual ~pSqlAdapter () {};

            vector<string> fieldNamesOf(string table, string schema="public") {
                string query = "SELECT column_name FROM information_schema.columns WHERE table_schema = ";
                query.append(dbField::quoteInto((schema.empty() ? "public" : schema)));
                query.append(" AND table_name = ");
                query.append(dbField::quoteInto(table));

                vector<string> outVec;

                pqxx::result rFields = doQuery(query);

                unsigned int cntFields = rFields.size();
                for (unsigned int i = 0; i < cntFields; i++)
                    outVec.push_back (rFields[i]["column_name"].as<string>());

                return outVec;
            }

        private :
            string  _dbName;
            string  _user;
            string  _pass;
            string  _host;

            // has to be redefined !
            string  _trxKeyMask;
    };

}; // end namespace core

}; // end namespace evias

#endif

