/**
 * Package: 	iSchool Core
 *
 * Implemented Namespaces:
 * 	- eVias
 *
 * Implemented Classes:
 * 	- Date
 *
 * Implemented Functions:
 * 	public :
 *  - Date* now()                   : get the current timestamp
 *  - bool isLeapYear (int)         : get wether the year is a leap year
 *  - int daysByMonth (int, int)    : get the count of days in a certain month
 *  - int year ()                   : get the year data
 *  - int month ()                  : get the month data
 *  - int mDay ()                   : month day (1-31) data
 *  - int yDay ()                   : year day (1-365) data
 *  - int wDay ()                   : week day (1-7) data
 *  - int hour ()                   : get the hour data
 *  - int min ()                    : get the minutes data
 *  - int sec ()                    : get the seconds data
 *  - time_t timeStamp ()           : get the timestamp data
 *  - struct tm* timeData ()        : get the timeData vector
 *
 *  protected
 *  - Date* const _initTimeData()   : initialize the time data
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */

#ifndef EVIAS_CORE_DATE_HPP
#define EVIAS_CORE_DATE_HPP

#include <string>
#include <ctime> // time
#include <cstdlib> // abs

#include "string_utils.hpp"

namespace evias {

namespace core {

    using namespace std;


    enum weekDays {
        SUNDAY      = 0,
        MONDAY      = 1,
        TUESDAY     = 2,
        WEDNESDAY   = 3,
        THURSDAY    = 4,
        FRIDAY      = 5,
        SATURDAY    = 6
    };

    static string daysLabels_en[] = {
        "Sunday", "Monday", "Tuesday",
        "Wednesday", "Thursday", "Friday",
        "Saturday"
    };

    class Date {

        public :

            Date();
            Date(time_t);
            // @todo fetch data from string
            //Date(string);
            Date(const Date &);

            static Date* now() {
                time_t timeStamp;

                time( &timeStamp );

                Date *returnObj = new Date( timeStamp );

                return returnObj;
            };

            static bool isLeapYear(int year) {
                return ( (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)) );
            };

            string toString();

            string toString(string);

            // initialization data
            time_t timeStamp() { return _timeStamp; };
            struct tm* timeData() { return _timeData; };

            void setTimeData (struct tm* data) {
                _timeData = data;

                // refresh date object
                _initTimeData();
            }

            // working data
            int year () { return _year; };
            int month() { return _month; };
            int mDay () { return _mDay; };
            int wDay () { return _wDay; };
            int yDay () { return _yDay; };
            int hour () { return _hour; };
            int min  () { return _min; };
            int sec  () { return _sec; };

            string day() {
                return evias::core::daysLabels_en[_wDay];
            };

            int dayOfWeek();

            int daysByMonth(int m, int y=0) {
                int year = y;
                if (year == 0)
                    year = _timeData->tm_year;

                if (
                    m == 1 || m == 3 || m == 5 || m == 7 ||
                    m == 8 || m == 10|| m == 12
                   )
                    return 31;
                else if( m == 2 )
                    return (Date::isLeapYear(year) ? 29 : 28);
                else
                    return 30;
            };

            // PUBLIC API

            Date* const addDay();
            Date* const addDay(int);
            Date* const subDay(int);

            Date* const addMonth();
            Date* const addMonth(int);

            Date* const addYear();
            Date* const addYear(int);

        protected :

            Date* const _initTimeData() {
                _year     = _timeData->tm_year + 1900;
                _month    = _timeData->tm_mon + 1;
                _yDay     = _timeData->tm_yday + 1;
                _mDay     = _timeData->tm_mday;
                _wDay     = _timeData->tm_wday + 1;
                _hour     = _timeData->tm_hour;
                _min      = _timeData->tm_min;
                _sec      = _timeData->tm_sec;

                return this;
            };

            Date* const _buildString();

        private :

            time_t      _timeStamp; // only used at initialization
            struct tm*  _timeData;  // same as line above

            int     _hour;
            int     _min;
            int     _sec;
            int     _yDay;
            int     _mDay;
            int     _wDay;
            int     _month;
            int     _year;

    };

}; // end namespace core

}; // end namespace evias

#endif
