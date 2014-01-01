#ifndef  __DATETIME_H
#define  __DATETIME_H

#include <inttypes.h>

class DateTime
{
    public:
        bool setTime(char* quickDateString);
        uint8_t bcd2uint(uint8_t bcd);
        uint8_t str2bcd(char, char);

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
