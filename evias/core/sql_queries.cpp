#include "sql_queries.hpp"

namespace evias {

namespace core {

    /**
     *
     * sqlSelect CLASS
     *
     */

    int sqlSelect::addField (string field, string alias )
    {
        if (alias != "") {
            _fieldsWithAlias.insert (pair<string,string>(field, alias));
        }
        else {
            _fields.push_back (field);
        }

        return 0;
    }

    string sqlSelect::toString ()
    {
        _selectStr = "";

        if (! _fields.empty()) {
            _selectStr = evias::core::assemble (_fields, ", ");
        }

        if (! _fieldsWithAlias.empty()) {
            _selectStr = evias::core::assemble (_fieldsWithAlias, " as ", ", ");
        }

        return string("SELECT " + _selectStr + " ");
    }

    /**
     *
     * sqlFrom CLASS
     *
     */

    sqlFrom* const sqlFrom::addLine (string line)
    {
        _fromLines.push_back (line);
        _fromStr = evias::core::assemble (_fromLines, " ");

        return this;
    }

    sqlFrom* const sqlFrom::addLines (vector<string> lines)
    {
        vector<string>::iterator it = lines.begin();
        for (; it != lines.end(); it++) {
            _fromLines.push_back (*it);
        }

        _fromStr = evias::core::assemble (_fromLines, " ");

        return this;
    }

    sqlFrom* const sqlFrom::leftInnerJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" LEFT INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::leftInnerJoinUsing (string table, string alias, string usingCondition)
    {
        string line = "";
        line.append (" LEFT INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::rightInnerJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" RIGHT INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::rightInnerJoinUsing (string table, string alias, string usingCondition)
    {
        string line = "";
        line.append (" RIGHT INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::leftOuterJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" LEFT OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::leftOuterJoinUsing (string table, string alias, string usingCondition)
    {
        string line = "";
        line.append (" LEFT OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::rightOuterJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" RIGHT OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::rightOuterJoinUsing (string table, string alias, string usingCondition)
    {
        string line = "";
        line.append (" RIGHT OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::innerJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::innerJoinUsing (string table, string alias, string usingCondition)
    {
        string line = "";
        line.append (" INNER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::outerJoinOn (string table, string alias, string onCondition)
    {
        string line = "";
        line.append (" OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! onCondition.empty())
            line.append (" ON (").append (onCondition).append(")");

        _fromLines.push_back (line);

        return this;
    }

    sqlFrom* const sqlFrom::outerJoinUsing (string table, string alias,string usingCondition)
    {
        string line = "";
        line.append (" OUTER JOIN ").append (table);

        if (alias != "") {
            line.append ( " AS " ).append (alias);
        }

        if (! usingCondition.empty())
            line.append (" USING (").append (usingCondition).append (")");

        _fromLines.push_back (line);

        return this;
    }

    string sqlFrom::toString ()
    {
        _fromStr = evias::core::assemble (_fromLines, " ");

        return string (" FROM " + _fromStr + " ");
    }

    /**
     *
     * sqlWhere CLASS
     * XXX add parameters (use sqlParams)
     *
     */

    sqlWhere* const sqlWhere::addCondition (string condition)
    {
        _whereLines.push_back (condition);

        return 0;
    }

    string sqlWhere::toString ()
    {
        _whereStr = evias::core::assemble (_whereLines, " AND ");

        if (! _params.empty()) {
            // needs to iterate the where conditions to replace the parameter keys
            // by their respective values.

            map<string,string> params                   = getParams ();
            map<string,string>::iterator currentParam   = params.begin ();

            for ( ; currentParam != params.end (); currentParam++) {
                // try and replace the parameter's key in the query,
                // by its actual value

                string paramKey = (*currentParam).first;
                string paramVal = (*currentParam).second;

                cout << endl << "key: " << paramKey << " | val: " << paramVal << endl;

                // find key until end
                string::size_type findKey;
                do {
                    findKey = _whereStr.find (paramKey);

                    if (findKey != string::npos) {
                        // replace by value

                        _whereStr.replace ( findKey, paramKey.size(), dbField::quoteInto (paramVal) );
                    }
                }
                while (findKey != string::npos);
            }
        }

        cout << endl << "will return: " << _whereStr;

        return string (" WHERE " + _whereStr + " ");
    }

    /**
     *
     * selectQuery CLASS
     *
     */

    string selectQuery::toString ()
    {
        _queryStr = _selectObj->toString() + _fromObj->toString() + _whereObj->toString();

        if (! _orderBy.empty()) {
            string::size_type findDesc = _orderBy.find("DESC");
            string::size_type findAsc = _orderBy.find("ASC");

            string direction = "ASC";
            if (findDesc != string::npos || findAsc != string::npos) {
                int endParse = 0;

                if (findDesc != string::npos) {
                    direction = "DESC";
                    endParse = (int) findDesc;
                }
                else if (findAsc != string::npos) {
                    endParse = (int) findAsc;
                }

                _orderBy = _orderBy.substr (0, endParse);
            }

            _queryStr = _queryStr + " ORDER BY " + _orderBy + " " + direction;
        }

        if (! _groupBy.empty ()) {
            _queryStr = _queryStr + " GROUP BY " + _groupBy;
        }

        if (_limit > 0)
            _queryStr = _queryStr + " LIMIT " + intToString(_limit);

        if (_offset > 0)
            _queryStr = _queryStr + " OFFSET " + intToString(_offset);

        return _queryStr + ";";
    }

    selectQuery* const selectQuery::limit (int limit) {
        _limit = limit ;

        return this;
    }

    selectQuery* const selectQuery::offset (int offset) {
        _offset = offset;

        return this;
    }

    selectQuery* const selectQuery::concatSql (string sql) {
        _additionalParams = sql;

        return this;
    }

    selectQuery* const selectQuery::orderBy (string orderBy) {
        _orderBy = orderBy;

        return this;
    }

    selectQuery* const selectQuery::groupBy (string groupBy) {
        _groupBy = groupBy;

        return this;
    }

    selectQuery* const selectQuery::select (string field, string alias) {
        _selectObj->addField (field, alias);

        return this;
    }

    selectQuery* const selectQuery::from (string line) {
        _fromObj->addLine (line);

        return this;
    }

    selectQuery* const selectQuery::where (string condition) {
        _whereObj->addCondition (condition);

        return this;
    }

    selectQuery* const selectQuery::with (map<string,string> params) {
        _whereObj->setParams (params);

        return this;
    }

    selectQuery* const selectQuery::join (string table, string alias, string condition, __joinType type) {
        switch (type) {
            case _INNER_JOIN_ON_ :
                _fromObj->innerJoinOn (table, alias, condition);
                break;

            case _INNER_JOIN_USING_ :
                _fromObj->innerJoinUsing (table, alias, condition);
                break;

            case _OUTER_JOIN_ON_ :
                _fromObj->outerJoinOn (table, alias, condition);
                break;

            case _OUTER_JOIN_USING_ :
                _fromObj->outerJoinUsing (table, alias, condition);
                break;

            case _LEFT_INNER_JOIN_ON_ :
                _fromObj->leftInnerJoinOn (table, alias, condition);
                break;

            case _LEFT_INNER_JOIN_USING_ :
                _fromObj->leftInnerJoinUsing (table, alias, condition);
                break;

            case _LEFT_OUTER_JOIN_ON_ :
                _fromObj->leftOuterJoinOn (table, alias, condition);
                break;

            case _LEFT_OUTER_JOIN_USING_ :
                _fromObj->leftOuterJoinUsing (table, alias, condition);
                break;

            case _RIGHT_INNER_JOIN_ON_ :
                _fromObj->rightInnerJoinOn (table, alias, condition);
                break;

            case _RIGHT_INNER_JOIN_USING_ :
                _fromObj->rightInnerJoinUsing (table, alias, condition);
                break;

            case _RIGHT_OUTER_JOIN_ON_ :
                _fromObj->rightOuterJoinOn (table, alias, condition);
                break;

            case _RIGHT_OUTER_JOIN_USING_ :
                _fromObj->rightOuterJoinUsing (table, alias, condition);
                break;
        }

        return this;
    }

    /**
     *
     * updateQuery CLASS
     *
     */

    updateQuery* const updateQuery::table (string tableName)
    {
        _tableName = tableName;

        return this;
    }

    updateQuery* const updateQuery::field (string field, string value)
    {
        _keyValues.insert (make_pair (field, value));

        return this;
    }

    updateQuery* const updateQuery::where (string condition)
    {
        _whereObj->addCondition(condition);

        return this;
    }

    string updateQuery::toString ()
    {
        _queryStr = "UPDATE " + _tableName + " set ";

        string fieldStr = "";
        if (! _keyValues.empty ()) {
            map<string,string>::iterator it = _keyValues.begin();
            for (int i = 0; it != _keyValues.end (); it++, i++) {

                fieldStr = fieldStr + (i > 0 ?  ", " : "") + (*it).first + "=" + dbField::quoteInto((*it).second);
            }
        }

        _queryStr = _queryStr + fieldStr;

        if (! _whereObj->toString().empty()) {
            _queryStr = _queryStr + " " + _whereObj->toString ();
        }

        return _queryStr;
    }

    removeQuery* const removeQuery::from (string table)
    {
        _fromObj->addLine (table);

        return this;
    }

    removeQuery* const removeQuery::where (string condition)
    {
        _whereObj->addCondition (condition);

        return this;
    }

    removeQuery* const removeQuery::cascade (bool bCasc)
    {
        _cascade = bCasc;

        return this;
    }

    string removeQuery::toString ()
    {
        _queryStr = "DELETE" + _fromObj->toString () + _whereObj->toString ();

        if (_cascade)
            _queryStr = _queryStr + " CASCADE";

        return _queryStr + ";";
    }

    insertQuery* const insertQuery::into (string table, string schema)
    {
        _tableName = table;
        _schema    = schema;

        return this;
    }

    insertQuery* const insertQuery::fields (vector<string> fields)
    {
        vector<string>::iterator it = fields.begin ();
        for (; it != fields.end (); it++)
            _data.insert (make_pair ((*it), ""));
    }

    insertQuery* const insertQuery::values (vector<string> val_vec)
    {
        vector<string>::iterator it = val_vec.begin();
        map<string,string>::iterator itData = _data.begin ();

        for (; it != val_vec.end () && itData != _data.end(); it++, itData++)
            (*itData).second = (*it);
    }

    insertQuery* const insertQuery::values (map<string,string> val_map)
    {
        map<string,string>::iterator it = val_map.begin ();
        for (; it != val_map.end (); it++)
            _data.insert (make_pair ((*it).first, (*it).second));
    }

    // XXX INSERT FROM
    // XXX INSERT INTO table (SUBQUERY)
    string insertQuery::toString ()
    {
        if (_data.empty() || _tableName.empty())
            return "";

        _queryStr = "INSERT INTO " + _schema + "." + _tableName;

        map<string,string>::iterator it = _data.begin();
        string fieldStr = " (";
        string valuesStr= " VALUES (";

        for (int i = 0; it != _data.end (); it++, i++) {
            fieldStr = fieldStr + (i > 0 ? ", " : "") + (*it).first;
            valuesStr= valuesStr+ (i > 0 ? ", " : "") + dbField::quoteInto((*it).second);
        }
        fieldStr = fieldStr + ") ";
        valuesStr = valuesStr + ") ";

        _queryStr = _queryStr + fieldStr + valuesStr;

        return _queryStr + ";";
     }

}; // core

}; // evias

