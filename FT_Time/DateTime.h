/**
 * @file DateTime.h
 * @author Johan Simonsson
 * @brief A date time class where data is stored as bcd data
 */

/*
 * Copyright (C) 2014 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef  __DATETIME_H
#define  __DATETIME_H

#include <inttypes.h>

/**
 * A date time class where data is stored as bcd data
 */
class DateTime
{
    public:
        DateTime();
        bool setTime(char* quickDateString);
        uint8_t bcd2uint(uint8_t bcd);
        uint8_t str2bcd(char, char);

        uint16_t daySince2000();
        uint32_t secSince2000();

        bool isoDateString(char* str);
    public:
        uint8_t year;  ///< 2 digit year 2000..2099 is 00..99 in BCD
        uint8_t month; ///< Month in BCD
        uint8_t day;   ///< Day in month in BCD
        uint8_t hour;  ///< Hour in BCD
        uint8_t min;   ///< Minute in BCD
        uint8_t sec;   ///< Second in BCD
        uint8_t dow;   ///< Day of week, 1..7 where 1 is monday in BCD
};

#endif  // __DATETIME_H
