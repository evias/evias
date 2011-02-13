#include "console.hpp"

namespace evias {

namespace application {

    consoleParser* const consoleParser::parseAll() {
        if (! (this->_argsCnt > 1)) {
            // has only program name argument
            this->_lastReturn = this->_canEmptyCall ? NOTHING_TO_DO : MISSING_REQUIRED;
            return this;
        }

        for (int i = 0; i < this->_argsCnt; i++) {
            this->_vArgs.push_back(this->_argsVal[i]);
        }

        bool validData = this->_validateInput();

        if (! validData) {
            this->_printUsage();
        }

        this->_dispatchData();

        // stop if already broken..
        if (this->_lastReturn != RET_SUCCESS) {
            return this;
        }

        // post dispatch validation
        bool postValidation = this->_validateInput();

        if (! postValidation) {
            this->_lastReturn = MISSING_REQUIRED;
            return this;
        }

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    bool consoleParser::_validateInput() {
        if (this->_argsCnt != this->_vArgs.size()) {
            this->_lastReturn = WRONG_DATA_INPUT;
            return false;
        }

        if (this->_dataDispatched) {
            return this->_hasRequired();
        }

        this->_lastReturn = RET_SUCCESS;
        return true;
    }

    bool consoleParser::_hasRequired()
    {
        if (this->_vRequired.size() == 0) {
            return true;
        }

        s_map::iterator itData = this->_mData.begin(); // data iteration
        s_vec::iterator itFound; // search return
        size_t indexAt;      // search validation
        string keyName = ""; // used for readability
        int countHaving = 0; // used for validation
        int argIndex = -1;   // will be set if not-named arguments

        // until end reached or requirements met
        for (; itData != this->_mData.end() && countHaving != this->_mData.size(); itData++) {
            if ((argIndex = atoi((*itData).first.c_str())) != 0) {
                // not-named arguments
                keyName = this->_mPositions[argIndex];
            }
            else {
                // named arguments
                keyName = (*itData).first;
            }
            // check for requirement of current argument
            itFound = find(this->_vRequired.begin(), this->_vRequired.end(), keyName);
            indexAt = distance(this->_vRequired.begin(), itFound);

            if (indexAt != this->_vRequired.size()) {
                // current argument is required
                countHaving++;
            }
        }

        if (countHaving != this->_vRequired.size()) {
            // did not match all required arguments
            return false;
        }

        return true;
    }

    consoleParser* const consoleParser::_dispatchData() {
        s_vec::iterator   itMixed;      // go through available args
        s_vec::iterator   itBefore;     // be able to work with arg-1
        s_vec::iterator   itFoundArg;   // use for finding current arg in allowed args
        size_t            stIndexFound;  // find current arg
        string keyName; // readability

        if (! (this->_vArgs.size() > 1)) {
            // only got executable name
            this->_lastReturn = this->_canEmptyCall ? NOTHING_TO_DO : MISSING_REQUIRED;
            return this;
        }

        // XXX should not be done like this
        //   => why not use regular expression
        //      to know wether a positioning argumentation
        //      has been used or a named argumentation
        // test purpose (do not loop if any error.)
        if (atoi(this->_vArgs[1].c_str()) != 0) {
            // have only numbers, positions need to be set
            if (! this->_argsPosSet) {
                this->_lastReturn = ARGS_POS_NOT_SET;
                return this;
            }
        }

        // initialize
        itMixed = this->_vArgs.begin();
        itBefore= this->_vArgs.begin();

        for (int i = 0; itMixed != this->_vArgs.end(); itMixed++, i++) {

            if (i == 0) {
                // first argument is program name
                continue;
            }

            if (i > 1) {
                itBefore++; // only increment the second time.. makes current_arg-1
            }

            if ((*itMixed)[0] == '-') {
                // - beginning arg name

                // verify argument is allowed
                itFoundArg = find(this->_vAllowed.begin(), this->_vAllowed.end(), (*itMixed));

                // get distance between beginning and iterator position
                stIndexFound = distance(this->_vAllowed.begin(), itFoundArg);

                if (stIndexFound != this->_vAllowed.size()) {
                    // valid argument, will be added next turn
                    continue;
                }
                else {
                    // not a valid argument
                    this->_lastReturn = ARG_NAME_INVALID;
                    continue;
                }
            }
            else {
                // arg value
                if ((*itBefore)[0] == '-' && this->_lastReturn != ARG_NAME_INVALID) {

                    // fetched valid - beginning argument .. now fetch its value
                    keyName = (*itBefore);
                    this->_mData.insert(pair<string, string>(keyName, (*itMixed)));
                    continue;
                }
                else if (this->_lastReturn == ARG_NAME_INVALID) {
                    // can now pass to next argument if any present ..
                    // mismatched arguments are to be allowed, just they are
                    // not processed
                    this->_lastReturn = RET_SUCCESS;
                    continue;
                }

                // coming here means no arg-name before value
                keyName = this->_mPositions[i];
                this->_mData.insert(pair<string,string>(keyName, (*itMixed)));
            }
        }

        this->_dataDispatched = true;

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::addAllowedArg(string argName) {
        if (argName.empty()) {
            this->_lastReturn = EMPTY_ARG_NAME;
            return this;
        }

        this->_vAllowed.push_back (argName);

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::addAllowedArgs(s_vec argList) {
        if (argList.empty()) {
            this->_lastReturn = EMPTY_ARGS_LIST;
            return this;
        }

        // insert at the end
        s_vec::iterator itEnd = this->_vAllowed.begin() + (this->_vAllowed.size()-1);

        // insert all elements
        this->_vAllowed.insert(itEnd, argList.begin(), argList.end());

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::setAllowedArgs(s_vec argList) {
        if (argList.empty()) {
            this->_lastReturn = EMPTY_ARGS_LIST;
            return this;
        }

        this->_vAllowed.clear();

        // insert all elements
        this->_vAllowed.assign(argList.begin(), argList.end());

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::removeAllowedArg(string argName) {
        if (this->_dataDispatched) {
            // cannot remove after dispatch
            this->_lastReturn = DATA_DISPATCHED;
            return this;
        }

        if (argName.empty()) {
            this->_lastReturn = EMPTY_ARG_NAME;
            return this;
        }

        // look for argument existance
        s_vec::iterator itFound   = find(this->_vAllowed.begin(), this->_vAllowed.end(), argName);
        size_t indexAt = distance(this->_vAllowed.begin(), itFound);

        if (indexAt == this->_vAllowed.size()) {
            this->_lastReturn = ARG_NAME_INVALID;
            return this;
        }

        // remove argument
        this->_vAllowed.erase(itFound);

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::removeAllowedArgs(s_vec argList) {
        if (this->_dataDispatched) {
            this->_lastReturn = DATA_DISPATCHED;
            return this;
        }

        if (argList.empty()) {
            this->_lastReturn = EMPTY_ARGS_LIST;
            return this;
        }

        bool            allValid = true;            // data validation
        s_vec::iterator itArgs   = argList.begin(); // args to remove
        s_vec::iterator itFound;                    // validation
        vector<s_vec::iterator> findingsList;       // positions in _vAllowed
        vector<s_vec::iterator>::iterator itFindings;// positions iteration for erase
        size_t          foundAt;                    // validation

        for( ; itArgs != argList.end(); itArgs++) {
            // look for argument existance
            itFound = find(this->_vAllowed.begin(), this->_vAllowed.end(), (*itArgs));
            foundAt = distance(this->_vAllowed.begin(), itFound);

            if (foundAt == this->_vAllowed.size()) {
                allValid = false;
                break;
            }

            // add position in _vAllowed
            findingsList.push_back(itFound);
        }

        if (! allValid) {
            this->_lastReturn = ARG_NAME_INVALID;
            return this;
        }

        itFindings = findingsList.begin();

        for ( ; itFindings != findingsList.end(); itFindings++) {
            // remove position in _vAllowed
            this->_vAllowed.erase((*itFindings));
        }

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::setArgsPositions(is_map mMap) {
        is_map::iterator itArgs = mMap.begin();
        s_vec::iterator  argValid;
        size_t           argIndex;

        for( ; itArgs != mMap.end(); itArgs++) {
            // validate argument name
            argValid = find(this->_vAllowed.begin(), this->_vAllowed.end(), (*itArgs).second);
            argIndex = distance(this->_vAllowed.begin(), argValid);

            if (argIndex == this->_vAllowed.size()) {
                this->_lastReturn = ARG_NAME_INVALID;
                return this;
            }

            // insert into gateway
            this->_mPositions.insert(pair<int,string>((*itArgs).first, (*itArgs).second));
        }

        this->_argsPosSet = true;

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    consoleParser* const consoleParser::setRequiredArgs(s_vec vReq) {
        // is there any args to set to required
        if (this->_vAllowed.empty()) {
            this->_lastReturn = this->_canEmptyCall ? NOTHING_TO_DO : MISSING_REQUIRED;
            return this;
        }

        if (vReq.size() > this->_vAllowed.size()) {
            // have more required fields to set than actual arguments known
            this->_lastReturn = TOO_MANY_REQUIRED;
            return this;
        }

        bool validArgs = this->_validateArgKeys(vReq);

        if (! validArgs) {
            // .. speaks enough ?! ;]
            this->_lastReturn = ARG_NAME_INVALID;
            return this;
        }

        this->_vRequired.assign(vReq.begin(), vReq.end());

        this->_lastReturn = RET_SUCCESS;
        return this;
    }

    bool consoleParser::_validateArgKeys(s_vec vArgs) {
        s_vec::iterator itInput = vArgs.begin();
        s_vec::iterator itFound;
        size_t indexAt;

        for (; itInput != vArgs.end(); itInput++) {
            itFound = find(this->_vAllowed.begin(), this->_vAllowed.end(), (*itInput));
            indexAt = distance(this->_vAllowed.begin(), itFound);

            if (indexAt == this->_vAllowed.size()) {
                return false;
            }
        }

        return true;
    }

    void consoleParser::_printUsage() {
        cout	<< "--------------------------------------"	<< endl
                << " Usage of " << this->_programName 		<< endl
                << this->_usageMessage						<< endl
                << "--------------------------------------" << endl;
    }

    void consoleParser::printMe()
    {
        cout	<< "------ eVias printMe ------"	<< endl
                << this->_programName << " running"	<< endl
                << "---------------------------"	<< endl;
    }

}; // end namespace application

}; // end namespace evias
