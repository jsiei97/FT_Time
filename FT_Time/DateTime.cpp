/**
 * @file DateTime.cpp
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

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "DateTime.h"

DateTime::DateTime()
{
        year  = 0;
        month = 0;
        day   = 0;
        hour  = 0;
        min   = 0;
        sec   = 0;
        dow   = 0;

        isClockSet = false;
}

/**
 * Update with a new time
 *
 * String shall be in format "YYYY-MM-DDThh:mm:ssZ_w"
 *
 * Where YYYY is 4 digit year.
 * MM is 2 digit month with leading zero.
 * DD is 2 digit day with leading zero.
 * T is the char T.
 * hh is 2 digit hour with leading zero, 24h mode.
 * mm is 2 digit minute with leading zero.
 * ss is 2 digit second with leading zero.
 * Z_ is the chars Z and _.
 * w is 1 digit day of week.
 *
 * Example: 2014-01-06T09:05:03Z_1
 *
 * @param quickDateString the date string
 * @return true if ok
 */
bool DateTime::setTime(char* quickDateString)
{
    int len = strlen(quickDateString);
    if(len != 22)
    {
        return false;
    }

    if(
            quickDateString[0]  == '2' &&
            quickDateString[1]  == '0' &&
            quickDateString[4]  == '-' &&
            quickDateString[7]  == '-' &&
            quickDateString[10] == 'T' &&
            quickDateString[13] == ':' &&
            quickDateString[16] == ':' &&
            quickDateString[19] == 'Z' &&
            quickDateString[20] == '_'
      )
    {
        year  = str2bcd( quickDateString[2], quickDateString[3] );
        month = str2bcd( quickDateString[5], quickDateString[6] );
        day   = str2bcd( quickDateString[8], quickDateString[9] );

        hour = str2bcd( quickDateString[11], quickDateString[12] );
        min  = str2bcd( quickDateString[14], quickDateString[15] );
        sec  = str2bcd( quickDateString[17], quickDateString[18] );

        dow  = str2bcd( '0', quickDateString[21] );

        isClockSet = true;
        return true;
    }
    return false;
}

/**
 * Convert a bcd (binary code decimal) into a normal number.
 *
 * If bcd is 0x43 the number becomes 43.
 *
 * @param bcd number in bcd format
 * @return uint8_t with normal data
 */
uint8_t DateTime::bcd2uint(uint8_t bcd)
{
    /// @todo Check for non dec num in bcd...
    uint8_t res = (bcd&0x0F);
    bcd = bcd >> 4;
    res += (bcd*10);
    return res;
}

/**
 * Convert a 2 char string into bcd (binary code decimal).
 *
 * If ch0='3' and ch1='6' the the result is 0x36.
 *
 * @param ch0 the first char in the string
 * @param ch1 the second char in the string
 * @return uint8_t with data in bcd.
 */
uint8_t DateTime::str2bcd(char ch0, char ch1)
{
    uint8_t res;

    if(ch0 > '9' || ch0 < '0')
        ch0 = '0';

    if(ch1 > '9' || ch1 < '0')
        ch1 = '0';

    res = ch0-'0';
    res = res << 4;
    res += ch1-'0';

    return res;
}

/**
 * How many days since 2000-01-01 00:00 UTC.
 *
 * @return uint16_t days since 2000.
 */
uint16_t DateTime::daySince2000()
{
    if(!isClockSet) 
        return 0;

    uint16_t days = 0;

    //First some years.
    for( unsigned int y = 0 ; y < bcd2uint(year) ; y++ )
    {
        //Leap year has 366 days (and 2000 was a leap year)
        if(y%4==0)
            days+=366;
        else
            days+=365;
    }

    //The length of the months is a strange thing...!
    for( unsigned int m = 1 ; m < bcd2uint(month) ; m++ )
    {
        if(m<=7)
        {
            if(m==2)
            {
                if( (bcd2uint(year)%4)==0 )
                {
                    days+=29;
                }
                else
                {
                    days+=28;
                }
            }
            else if(m%2==0)
            {
                days+=30;
            }
            else
            {
                days+=31;
            }
        }
        else
        {
            if(m%2==0)
            {
                days+=31;
            }
            else
            {
                days+=30;
            }
        }
    }

    days+=(bcd2uint(day)-1);

    return days;
}

/**
 * How many seconds since 2000-01-01 00:00 UTC.
 *
 * @return uint32_t seconds since 2000.
 */
uint32_t DateTime::secSince2000()
{
    if(!isClockSet) 
        return 0;

    uint32_t seconds = (uint32_t)daySince2000();
    seconds *= 86400;

    seconds += bcd2uint(hour)*60*60;
    seconds += bcd2uint(min)*60;
    seconds += bcd2uint(sec);

    return seconds;
}


/**
 * get a ISO 8601 string
 *
 * @param [out] the date string
 * @return true is ok
 */
bool DateTime::isoDateString(char* str)
{
    snprintf(str, 21, "20%02x-%02x-%02xT%02x:%02x:%02xZ",
            year, month, day,
            hour, min, sec);
    str[20] = '\0';

    //If init is missing
    if(year == 0)
        return false;

    return true;
}
