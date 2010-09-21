#include "date.hpp"

namespace evias {

namespace core {

    Date::Date() {
        time( &_timeStamp );

        // convert timestamp to (struct tm*)
        _timeData = localtime( &_timeStamp);

        // init variables
        _initTimeData();
    }

    Date::Date(time_t timeRef)
        : _timeStamp( timeRef )
    {
        // convert timestamp to (struct tm*)
        _timeData = localtime( &timeRef );

        this->_initTimeData();
    }

    Date::Date(const Date &ref) {
        _timeStamp = ref._timeStamp;
        _timeData  = ref._timeData;

        // if data has been changed, we need to set
        // the correct working data
        _hour = ref._hour;
        _min  = ref._min;
        _sec  = ref._sec;
        _month= ref._month;
        _year = ref._year;
        _yDay = ref._yDay;
        _mDay = ref._mDay;
        _wDay = ref._wDay;

    }

    string Date::toString() {
        // default format is: yyyy-mm-dd H:M:S
        string sDate;

        sDate.append(intToString(this->year())) .append("-")
             .append(padString(intToString(this->month()), 2, "0")).append("-")
             .append(padString(intToString(this->mDay()), 2, "0")) .append(" ")
             .append(padString(intToString(this->hour()), 2, "0")) .append(":")
             .append(padString(intToString(this->min()), 2, "0"))  .append(":")
             .append(padString(intToString(this->sec()), 2, "0"));

        return sDate;
    }

    string Date::toString(string format) {
        return "";
    }

    int Date::dayOfWeek() {
        struct tm data;

        // set data to working data
        data.tm_mon = this->month() - 1;
        data.tm_mday = this->mDay();
        data.tm_year = this->year() - 1900;
        data.tm_hour = data.tm_min = data.tm_sec = 0;
        data.tm_isdst = -1; // 00:00 would generate day-1 when DST

        // generate unfilled data (week day..)
        int retCode = mktime(&data);

        if (retCode != -1) {
            return data.tm_wday;
        }

        return retCode;
    }

    Date* const Date::addDay() {
        // no need for double-maintening algorithm
        // call to below function is very fast
        return this->addDay(1);
    }

    Date* const Date::addDay(int nDays) {
        int yDay = _yDay;
        int mDay = _mDay;
        int month= _month;
        int year = _year;
        int daysInMonth = this->daysByMonth(month);
        int yearDays = (Date::isLeapYear(year) ? 366 : 365);

        // check negativeness
        if (nDays < 0) {
            return this->subDay(abs(nDays));
        }

        if (nDays <= (daysInMonth - mDay)) {
            // has less days to add than remaining in month
            _mDay += nDays;
            _yDay += nDays;
            // can now recalculate the day of the week
            _wDay = this->dayOfWeek();

            return this;
        }

        if (nDays <= (yearDays - yDay)) {
            // less days than remaining in current year
            _yDay += nDays;

            while (nDays > 0) {
                // there are some more days to add

                if (nDays <= (daysInMonth - mDay)) {
                    // no more days to add than remaining in working month
                    mDay += nDays - 1; // jumped from 31 to 1

                    _mDay  = mDay;
                    _month = month;
                    // can now recalculate the day of the week
                    _wDay = this->dayOfWeek();

                    return this;
                }

                nDays -= (daysInMonth - mDay);
                mDay   = 1;
                month++;
                daysInMonth = this->daysByMonth(month);
            }
        }

        // will finish at least one year
        while (nDays > 0) {
            // some more days to add

            if (nDays <= (daysInMonth - mDay)) {
                // no more days to add than remaining in working month
                mDay += nDays - 1; // jumped from 31 to 1

                _mDay  = mDay;
                _yDay  = yDay;
                _month = month;
                _year  = year;
                // can now recalculate the day of the week
                _wDay = this->dayOfWeek();

                return this;
            }

            // @todo: PERFORMANCE
            // @brief:
            //  - should try and add more month at a time
            //  - use of one more condition (nDays <= (yearDays - yDay))
            // => bigger jumps possible

            nDays -= (daysInMonth - mDay);
            mDay   = 1;
            month++;

            if (month > 12) {
                // end of year
                year++;
                month = 1;
                yDay  = 1;
                yearDays = (Date::isLeapYear(year) ? 366 : 365);
            }

            daysInMonth = this->daysByMonth(month);
        }
    }

    Date* const Date::subDay(int nDays) {
        int yDay = _yDay;
        int mDay = _mDay;
        int month= _month;
        int year = _year;
        int daysInMonth;

        if (nDays < mDay) {
            // not changing month
            _mDay -= nDays;
            _yDay -= nDays;
            _wDay  = this->dayOfWeek();

            return this;
        }

        if (month == 1) {
            daysInMonth = this->daysByMonth(12);
        }
        else {
            daysInMonth = this->daysByMonth(month-1);
        }

        if (nDays < yDay) {
            // less days to substract than passed in current year
            _yDay -= nDays;

            while (nDays > 0) {
                 // there are some more days to substract

                if (nDays < mDay) {
                    // no more days to substract than passed in working month
                    mDay -= nDays - 1; // jumped from 1 to 31

                    _mDay  = mDay;
                    _month = month;
                    // can now recalculate the day of the week
                    _wDay = this->dayOfWeek();

                    return this;
                }

                nDays -= mDay;
                mDay   = daysInMonth;
                month--;
                daysInMonth = this->daysByMonth(month);
            }
        }

        // will go back at least one year
        while (nDays > 0) {
            // some more days to substract

            if (nDays < mDay) {
                // no more days to substract than passed in working month
                mDay -= nDays - 1; // jumped from 1 to 31

                _mDay  = mDay;
                _yDay  = yDay;
                _month = month;
                _year  = year;
                // can now recalculate the day of the week
                _wDay = this->dayOfWeek();

                return this;
            }

            // @todo: PERFORMANCE
            // @brief:
            //  - should try and substract more months at a time

            nDays -= mDay;
            mDay   = daysInMonth;
            month--;

            if (month < 1) {
                // begin of year
                year--;
                month   = 12;
                yDay    = (Date::isLeapYear(year) ? 366 : 365);
            }

            daysInMonth = this->daysByMonth(month);
        }
    }

    Date* const Date::addMonth() {
        int mDay  = _mDay;
        int yDay  = _yDay;
        int month = _month;
        int year  = _year;
        int nextMonth = month + 1;

        if (nextMonth > 12) {
            nextMonth = 1;
            yDay = 1;
            mDay = 1;
            year++;
        }

        int daysInYear      = (Date::isLeapYear(year) ? 366 : 365);
        int daysInMonth     = this->daysByMonth(month); // days to add

        if (year == _year) {
            // only need to add month

            // get how many days to add left after switching month
            _mDay = daysInMonth - (daysInMonth - _mDay);
            _yDay = _yDay + daysInMonth;
            _month= nextMonth;

            _wDay = this->dayOfWeek();

            return this;
        }

        // we have changed year

        // get how many days to add left after switching year
        _mDay = daysInMonth - (daysInYear - _yDay);
        _yDay = yDay;
        _year = year;
        _month= nextMonth;

        _wDay = this->dayOfWeek();
        return this;
    }

    Date* const Date::addMonth(int nMonths) {
        return this;
    }

    Date* const Date::addYear() {
        return this;
    }

    Date* const Date::addYear(int nYears) {
        return this;
    }

}; // end namespace core

}; // end namespace evias
