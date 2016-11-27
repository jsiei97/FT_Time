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

    if(cnt%7==0)
    {
        val++;
    }

    cnt++;
    return val;
}


int main()
{
    printf("Test how send on change would look like\n");

    DateTime dtOld;
    DateTime dtNow;

    unsigned int valueNow=10;
    unsigned int valueOld=valueNow;

    char str[30];
    char iso[30];

    bool oneMore = true;

    for( int h=1 ; h<6 ; h++ )
    {
        for( int m=0 ; m<59 ; m+=10 )
        {
            snprintf(str, 30, "2016-11-27T%02d:%02d:00Z_6", h, m);
            //printf("%s\n", str);
            //dtNow.setTime(str);

            valueNow = value(valueNow);
            if(valueOld != valueNow)
            {
                //There is a new value, send the last one first.
                dtOld.isoDateString(iso);
                printf("\t%03d %s\n", valueOld, iso );

                //Mark that it is sent
                valueOld=valueNow;
                dtOld.setTime(str);

                //Then send the new one
                dtOld.isoDateString(iso);
                printf("\t%03d %s\n", valueNow, iso );

                oneMore = true;
            }
            else
            {
                //Check time since last
                dtOld.setTime(str);

                if(oneMore==true)
                {
                    //We need to send one more direct
                    //so we can calc
                    dtOld.isoDateString(iso);
                    printf("\t%03d %s\n", valueNow, iso );
                    oneMore=false;
                }
            }
        }
    }

    return 0;
}
