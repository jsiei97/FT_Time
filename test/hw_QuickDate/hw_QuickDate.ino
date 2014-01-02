#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>

#include "QuickDate.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xEE, 0xFF, 0xBE, 0xEF, 0xFE, 0xED };

char server[] = "bender";

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
QuickDate qd;
char qdBuff[25];


void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    //http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
    wdt_enable(WDTO_8S);

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

    Serial.print("Time: ");
    time = millis();
    //prints time since program started
    Serial.println(time);

    int qdStatus = qd.doTimeSync(qdBuff);
    if(qdStatus > 0)
    {
        Serial.print("ok: ");
        Serial.print(qdBuff);
        Serial.print(" : ");
    }
    else
    {
        Serial.print("fail: ");
    }

    Serial.println(qdStatus);


    for(int i=0; i<2; i++)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
}
