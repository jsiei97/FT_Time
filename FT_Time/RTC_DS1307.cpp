/**
 * @file RTC_DS1307.cpp
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

#include <Wire.h>
#include "RTC_DS1307.h"
#include "DateTime.h"

//#ifdef __AVR__
//#include <avr/pgmspace.h>
//#define WIRE Wire
//#else
//#define PROGMEM
//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
//#define WIRE Wire1
//#endif

#define DS1307_ADDRESS 0x68

#define DS1307_SQW_ON  0x80
#define DS1307_SQW_OFF 0x00
#define DS1307_SQW_1HZ 0x10

#include <Arduino.h>

void RTC_DS1307::sqw(uint8_t state)
{
    //First update the normal clock
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x7);
    Wire.write(state);
    Wire.endTransmission();
}

bool RTC_DS1307::isrunning(void)
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 1);
    uint8_t ss = Wire.read();

    //Check bit7
    if((ss & 0x80) != 0)
    {
        sqw(DS1307_SQW_ON);
        return false;
    }

    return true;
}

void RTC_DS1307::adjust(DateTime* dt)
{
    //First update the normal clock
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);
    Wire.write(dt->sec);
    Wire.write(dt->min);
    Wire.write(dt->hour);
    Wire.write(dt->dow);
    Wire.write(dt->day);
    Wire.write(dt->month);
    Wire.write(dt->year);
    Wire.endTransmission();

    //Then write the mirror clock at 0x10
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x10);
    Wire.write(dt->sec);
    Wire.write(dt->min);
    Wire.write(dt->hour);
    Wire.write(dt->dow);
    Wire.write(dt->day);
    Wire.write(dt->month);
    Wire.write(dt->year);
    Wire.endTransmission();

    //Then start to blink the clock status led since all is fine!
    sqw(DS1307_SQW_1HZ);
}

bool RTC_DS1307::getTime(DateTime* now, DateTime* last)
{
    if(!isrunning())
        return false;

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 7);
    now->sec   = Wire.read();
    now->min   = Wire.read();
    now->hour  = Wire.read();
    now->dow   = Wire.read();
    now->day   = Wire.read();
    now->month = Wire.read();
    now->year  = Wire.read();

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x10);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_ADDRESS, 7);
    last->sec   = Wire.read();
    last->min   = Wire.read();
    last->hour  = Wire.read();
    last->dow   = Wire.read();
    last->day   = Wire.read();
    last->month = Wire.read();
    last->year  = Wire.read();

    return true;
}

