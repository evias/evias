#ifndef __EVIAS_CORE_selectQuery__
#define __EVIAS_CORE_selectQuery__

/**
 * Package: 	iSchool Core
 *
 * Implemented Namespaces:
 * 	- evias::core
 *
 * Implemented Classes:
 *  - sqlSelect
 *  - sqlFrom
 *  - sqlWhere
 *  - sqlParam
 *  - sqlParams
 *  - selectQuery
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory> // auto_ptr

#include "string_utils.hpp"
#include "db_field.hpp"

namespace evias {

namespace core {

    using evias::core::intToString;

    class sqlSelect
    {
        public :

            sqlSelect () {
                _selectStr = "";
            }

            sqlSelect (string fieldsStr)
                : _selectStr(fieldsStr)
            {
            }

            sqlSelect (vector<string> fields)
                : _fields (fields)
            {
            }

            sqlSelect (map<string,string> fields)
                : _fieldsWithAlias (fields)
            {
            }

            int addField (string field, string alias = "");

            string toString ();

        private :

            string              _selectStr;
            vector<string>      _fields;
            map<string,string>  _fieldsWithAlias;
    };

    class sqlFrom {
        public :

            sqlFrom () {
                _fromStr = "";
            }

            sqlFrom (string tablesStr)
                : _fromStr (tablesStr)
            {
                _fromLines = evias::core::split (_fromStr, ',');
            }

            sqlFrom (vector<string> lines)
                : _fromLines (lines)
            {
                _fromStr = evias::core::assemble (_fromLines, " ");
            }

            sqlFrom* const addLine (string line);

            sqlFrom* const addLines (vector<string> lines);

            // JOINS

            sqlFrom* const leftInnerJoinOn      (string table, string alias = "", string onCondition = "");
            sqlFrom* const leftInnerJoinUsing   (string table, string alias = "", string usingCondition = "");
            sqlFrom* const rightInnerJoinOn     (string table, string alias = "", string onCondition = "");
            sqlFrom* const rightInnerJoinUsing  (string table, string alias = "", string usingCondition = "");
            sqlFrom* const leftOuterJoinOn      (string table, string alias = "", string onCondition = "");
            sqlFrom* const leftOuterJoinUsing   (string table, string alias = "", string usingCondition = "");
            sqlFrom* const rightOuterJoinOn     (string table, string alias = "", string onCondition = "");
            sqlFrom* const rightOuterJoinUsing  (string table, string alias = "", string usingCondition = "");
            sqlFrom* const innerJoinOn          (string table, string alias = "", string onCondition = "");
            sqlFrom* const innerJoinUsing       (string table, string alias = "", string usingCondition = "");
            sqlFrom* const outerJoinOn          (string table, string alias = "", string onCondition = "");
            sqlFrom* const outerJoinUsing       (string table, string alias = "", string usingCondition = "");

            string toString ();

        private :

            string          _fromStr;
            vector<string>  _fromLines;
    };

    class sqlWhere
    {
        public :

            sqlWhere () {
                _whereStr = "";
            }

            sqlWhere (string whereStr)
                : _whereStr (whereStr)
            {
                _whereLines.size();
            }

            sqlWhere (string whereStr, map<string,string> params)
                : _whereStr (whereStr)
            {
                _whereLines.push_back (whereStr);

                setParams (params);
            }

            sqlWhere* const setParams (map<string,string> params)
            {
                map<string,string>::iterator it = params.begin();
                for (; it != params.end (); it++)
                    _params.insert (make_pair ((*it).first, (*it).second));
            }

            map<string,string> getParams ()
                { return _params; }

            sqlWhere* const addCondition (string condition);
            string toString ();

            bool hasConditions () {
                return _whereLines.size() > 0;
            }

        private :

            string          _whereStr;
            vector<string>  _whereLines;

            map<string,string> _params;
    };

    enum __joinType {
        _LEFT_INNER_JOIN_ON_        = 1,
        _LEFT_INNER_JOIN_USING_     = 2,
        _LEFT_OUTER_JOIN_ON_        = 3,
        _LEFT_OUTER_JOIN_USING_     = 4,
        _RIGHT_INNER_JOIN_ON_       = 5,
        _RIGHT_INNER_JOIN_USING_    = 6,
        _RIGHT_OUTER_JOIN_ON_       = 7,
        _RIGHT_OUTER_JOIN_USING_    = 8,
        _INNER_JOIN_ON_             = 9,
        _INNER_JOIN_USING_          = 10,
        _OUTER_JOIN_ON_             = 11,
        _OUTER_JOIN_USING_          = 12
        // @XXX : cross joins
    };

    class selectQuery
    {
        public :

            selectQuery () {
                _selectObj = new sqlSelect();
                _fromObj   = new sqlFrom  ();
                _whereObj  = new sqlWhere ();

                _limit  = 0;
                _offset = 0;
                _groupBy = "";
                _orderBy = "";
                _additionalParams = "";
            }

            selectQuery (sqlSelect* select, sqlFrom* from, sqlWhere* where)
                : _selectObj (select), _fromObj (from), _whereObj (where)
            {
                _limit  = 0;
                _offset = 0;
                _groupBy = "";
                _orderBy = "";
                _additionalParams = "";
            }

            ~selectQuery () {
                if (_selectObj != NULL)
                    delete _selectObj;

                if (_fromObj != NULL)
                    delete _fromObj;

                if (_whereObj != NULL)
                    delete _whereObj;
            }

            string toString ();

            selectQuery* const limit    (int limit);
            selectQuery* const offset   (int offset);
            selectQuery* const concatSql(string sql);
            selectQuery* const orderBy  (string orderBy);
            selectQuery* const groupBy  (string groupBy);
            selectQuery* const select   (string field, string alias = "");
            selectQuery* const from     (string line);
            selectQuery* const where    (string condition);
            selectQuery* const with     (map<string,string>);
            selectQuery* const join     (string table, string alias="", string condition = "", __joinType type = _INNER_JOIN_ON_);

        private :

            string      _queryStr;

            sqlSelect*  _selectObj;
            sqlFrom*    _fromObj;
            sqlWhere*   _whereObj;

            int         _limit;
            int         _offset;
            string      _groupBy;
            string      _orderBy;
            string      _additionalParams;
    };

    class updateQuery
    {
        public :

            updateQuery () {
                _whereObj = new sqlWhere;
                _queryStr = "";
                _tableName= "";
            }

            ~updateQuery () {
                if (_whereObj != NULL)
                    delete _whereObj;
            }

            string toString ();

            updateQuery* const table (string);
            updateQuery* const field (string, string);
            updateQuery* const where (string);

        private :

            string _queryStr;

            string              _tableName;
            map<string, string> _keyValues;

            sqlWhere*   _whereObj;
    };

    class removeQuery
    {
        public :

            removeQuery () {
                _whereObj = new sqlWhere;
                _fromObj  = new sqlFrom;
                _cascade  = false;
                _queryStr = "";
            }

            ~removeQuery () {
                if (_whereObj != NULL)
                    delete _whereObj;

                if (_fromObj != NULL)
                    delete _fromObj;
            }

            string toString ();

            removeQuery* const from  (string);
            removeQuery* const where (string);
            removeQuery* const cascade (bool bCasc = true);

        private :

            string      _queryStr;

            bool        _cascade;

            sqlFrom*    _fromObj;
            sqlWhere*   _whereObj;
    };

    class insertQuery
    {
        public :

            insertQuery () {
                _queryStr  = "";
                _tableName = "";
                _schema    = "public";
            }

            ~insertQuery () {

            }

            string toString ();

            insertQuery* const into (string table, string schema="public");
            insertQuery* const fields (vector<string>);
            insertQuery* const values (vector<string>);
            insertQuery* const values (map<string,string>);

        private :

            string _queryStr;

            string _schema;
            string _tableName;

            map<string,string> _data;
    };

}; // end namespace core

}; // end namespace evias

#endif
