#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>
#include <Wire.h>

#include "QuickDate.h"
#include "DateTime.h"
#include "RTC_DS1307.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xEE, 0xFF, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
QuickDate qd;
char qdBuff[25];
char isoDate[25];

DateTime now;  // Current time in RTC
DateTime last; // Last time rtc was updated
RTC_DS1307 rtc;


void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    //http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
    wdt_enable(WDTO_8S);

    Wire.begin();

    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, ip);
    }

    // give the Ethernet shield a second to initialize:
    delay(1000);

    Serial.println();
    Serial.println("*********************");
    Serial.println("*** Setup done... ***");
    Serial.println("*********************");
}

unsigned long time;
void loop()
{
    //Feed the dog.
    wdt_reset();

    // Update now, so we have correct time to work with.
    rtc.getTime(&now, &last);

    //Print:
    // 1. uptime from internal clock
    // 2. current time in RTC
    // 3. last synced time stored in the RTC
    Serial.print("Time: ");
    time = millis();
    Serial.print(time);
    Serial.print(" - ");
    now.isoDateString(isoDate);
    Serial.print(isoDate);
    last.isoDateString(isoDate);
    Serial.print(" (");
    Serial.print(isoDate);
    Serial.println(")");

    // If last time sync is older than Xs (like 120s)
    if(!rtc.isrunning() || (now.secSince2000()-last.secSince2000()>120) )
    {
        int qdStatus = qd.doTimeSync(qdBuff);
        if(qdStatus > 0)
        {
            Serial.print("ok: ");
            Serial.print(qdBuff);
            Serial.print(" : ");
            now.setTime(qdBuff);
            rtc.adjust(&now);
        }
        else
        {
            Serial.print("fail: ");
        }

        Serial.println(qdStatus);
    }

    delay(2000);
}
