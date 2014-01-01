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

#include "DateTime.h"

bool DateTime::setTime(char* quickDateString)
{
    int len = strlen(quickDateString);
    if(len < 22)
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
        return true;
    }
    return false;
}

uint8_t DateTime::bcd2uint(uint8_t bcd)
{
    /// @todo Check for non dec num in bcd...
    uint8_t res = (bcd&0x0F);
    bcd = bcd >> 4;
    res += (bcd*10);
    return res;
}

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

uint16_t DateTime::daySince2000()
{
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

uint32_t DateTime::secSince2000()
{
    uint32_t seconds = (uint32_t)daySince2000();
    seconds *= 86400;

    seconds += bcd2uint(hour)*60*60;
    seconds += bcd2uint(min)*60;
    seconds += bcd2uint(sec);

    return seconds;
}
