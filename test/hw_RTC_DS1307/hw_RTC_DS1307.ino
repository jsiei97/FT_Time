#include <Wire.h>

#include "DateTime.h"
#include "RTC_DS1307.h"


DateTime now;
DateTime last;
RTC_DS1307 rtc;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Wire.begin();

    Serial.println();
    Serial.println("*********************");
    Serial.println("*** Setup done... ***");
    Serial.println("*********************");


}

unsigned long time;
void loop()
{
    Serial.print("Time: ");
    time = millis();
    //prints time since program started
    Serial.println(time);

    //Basic test
    //1. Remove battery from the ds1307 (with the ard turned off)
    //2. start this program
    //3. The DS1307 status led will be on for approx 10s
    //4. Then as the time is adjusted the led will start to blink

    if(rtc.isrunning())
    {
        Serial.println("Clock is running");
    }
    else
    {
        Serial.println("Clock is NOT running");
    }


    for(int i=0; i<10; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");


    if(!rtc.isrunning())
    {
        now.setTime("2014-02-10T12:00:00Z_3");
        rtc.adjust(&now);
    }

    rtc.getTime(&now, &last);

    char datestr[25];
    snprintf(datestr, 24, "20%02x-%02x-%02x %02x:%02x:%02x %01x",
            now.year, now.month, now.day,
            now.hour, now.min, now.sec,
            now.dow);
    Serial.println(datestr);

    uint32_t timeSinceSync = now.secSince2000()-last.secSince2000();

    Serial.print("Last sync: ");
    Serial.println(timeSinceSync);

}
