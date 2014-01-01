/**
 * @file QuickDate.h
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

#ifndef  __QUICKDATE_H
#define  __QUICKDATE_H

#include <Ethernet.h>

#include "DateTime.h"

class QuickDate
{
    private:
        EthernetClient client; ///< The http client
        unsigned int syncTime; ///< How often shall we sync time in seconds?

        DateTime timeNow;      ///< Current time
        DateTime timeLastSync; ///< Last synced time

    public:
        QuickDate(unsigned int doSyncTime);
        unsigned int timeSinceLastSync();
        int doTimeSync();

};

#endif  // __QUICKDATE_H
