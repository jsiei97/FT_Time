/**
 * @file RTC_DS1307.h
 * @author Johan Simonsson
 * @brief Interface class for the DS1307 RTC
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

#ifndef  __RTC_DS1307_H
#define  __RTC_DS1307_H

#include "DateTime.h"


/**
 * RTC based on the DS1307 chip connected via I2C and the Wire library
 *
 * This code is inspired by:
 * Code by JeeLabs http://news.jeelabs.org/code/
 *
 * And the works done by Adafruit
 * https://github.com/adafruit/RTClib
 */
class RTC_DS1307
{
    private:

    public:
        /// What output state the sqw pin shall have.
        enum SQW_State {
            DS1307_SQW_ON  = 0x80, ///< sqw always on
            DS1307_SQW_OFF = 0x00, ///< sqw always off
            DS1307_SQW_1HZ = 0x10  ///< sqw pulses with 1Hz
        };

        bool isrunning(void);
        void sqw(SQW_State state);

        void adjust(DateTime* dt);
        bool getTime(DateTime* now, DateTime* last);
};

#endif  // __RTC_DS1307_H
