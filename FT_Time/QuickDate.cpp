/**
 * @file QuickDate.cpp
 * @author Johan Simonsson
 * @brief Get the time from QuickDate.php and save into a DS1307
 */

/*
 * Copyright (C) 2013 Johan Simonsson
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

#include <Ethernet.h>
#include "QuickDate.h"


QuickDate::QuickDate(unsigned int doSyncTime)
{
    syncTime = doSyncTime;
}


/**
 * How many seconds since last sync?
 *
 * Read DS1307 0..7 and compare with 10..17
 * and calculate how many seconds past since last sync.
 *
 * @return seconds since last sync, 0 if no valid time
 */
unsigned int QuickDate::timeSinceLastSync()
{
    return 0;
}

/**
 * Do timesync if it is time
 *
 * If more than X seconds has past since last sync,
 * then do another sync.
 *
 * @return positive value if ok, negative if bad.
 */
int QuickDate::doTimeSync()
{
    unsigned int syncDiffTime = timeSinceLastSync();
    if( (syncDiffTime != 0) && (syncDiffTime < syncTime) )
    {
        return 1;
    }

    /// @todo dynamic server name... (same as the MQTT server? FT_EDS?)
    if (client.connect("scruffy", 80))
    {
        //Serial.println("connected");
        // Make a HTTP request:
        client.println("GET /QuickDate/QuickDate.php");
        //client.println("Host: bender");
        client.println("Connection: close");
        client.println();
    }
    else
    {
        // kf you didn't get a connection to the server:
        //Serial.println("connection failed");
        client.flush();
        client.stop();
        return -1;
    }

    //Wait a little for the server to react
    delay(1000);

    char buff[25];
    unsigned int pos = 0;
    while(client.available() &&  pos < 24)
    {
        buff[pos] = client.read();
        pos++;
        //Serial.print(c);
    }
    buff[pos] = '\0';

    //Serial.println(buff);
    //Serial.println(pos);

    if( pos == 22 &&
            buff[4]  == '-' &&
            buff[7]  == '-' &&
            buff[10] == 'T' &&
            buff[13] == ':' &&
            buff[16] == ':' &&
            buff[19] == 'Z' &&
            buff[20] == '_'
      )
    {
        //Serial.println("ok date");
        /// @todo send string into parser that updates the ds1307
    }
    else
    {
        return -2;
    }

    //Serial.println();
    //Serial.println("disconnecting.");
    client.stop();

    return 2;
}

