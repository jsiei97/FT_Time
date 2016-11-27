#include <stdio.h>
#include "DateTime.h"

/*
 * Prototype code to play with send on change
 * for meter values...
 */


//Every X time, we return a value that is a little bit bigger.
//"simulate" irq inc
unsigned int value(unsigned int val)
{
    static int cnt=0;
    static bool skip = false;

    if(cnt%22==0)
    {
        if(true == skip)
        {
            val++;
            skip = false;
        }
        else
        {
            skip = true;
        }
    }

    cnt++;
    return val;
}


int main()
{
    printf("Test how send on change would look like\n");

    DateTime now;   ///< Current time
    DateTime dtOld; ///< Last time value checked
    DateTime dtSent;///< Last time value was sent

    unsigned int valueNow=10;       ///< Current value
    unsigned int valueSent=valueNow;///< Last value sent

    char str[30];
    char iso[30];

    bool oneMore = true;

    for( int h=0 ; h<6 ; h++ )
    {
        for( int m=0 ; m<59 ; m+=2 )
        {
            snprintf(str, 30, "2016-11-27T%02d:%02d:00Z_6", h, m);
            //printf("%s\n", str);
            now.setTime(str);

            valueNow = value(valueNow);
            if( ( valueSent != valueNow ))
            {
                if(dtOld.isoDateString(iso))
                {
                    //There is a new value, send the last one first.
                    printf("\t%03d %s\n", valueSent, iso );
                }

                //Mark that it is sent
                valueSent=valueNow;
                dtSent.setTime(str);

                //Then send the new one
                dtSent.isoDateString(iso);
                printf("\t%03d %s\n", valueNow, iso );

                oneMore = true;
            }
            else if ( (now.secSince2000()-dtSent.secSince2000()) >= (30*60) )
            {
                // No change for 30min, send anyway
                dtSent.setTime(str);
                dtSent.isoDateString(iso);
                printf("\t%03d %s\n", valueNow, iso );
                printf("\n");

            }
            else
            {
                //Update time since last
                dtOld.setTime(str);

                if(oneMore==true)
                {
                    //We need to send one more direct
                    //so we can calc
                    dtOld.isoDateString(iso);
                    printf("\t%03d %s\n", valueNow, iso );
	                printf("\n");
                    oneMore=false;
                }
            }
        }
    }

    return 0;
}
