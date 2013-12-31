
#include "DateTime.h"
#include <inttypes.h>
#include <string.h>

//DateTime::DateTime (char* quickDateString)
//{
    //setTime(quickDateString);
//}

bool DateTime::setTime(char* quickDateString)
{
    int len = strlen(quickDateString);
    if(len < 22)
    {
        return false;
    }

    if(
            quickDateString[0]  == '2' &&
            quickDateString[1]  == '0' &&
            quickDateString[4]  == '-' &&
            quickDateString[7]  == '-' &&
            quickDateString[10] == 'T' &&
            quickDateString[13] == ':' &&
            quickDateString[16] == ':' &&
            quickDateString[19] == 'Z' &&
            quickDateString[20] == '_'
      )
    {
        year  = str2bcd( quickDateString[2], quickDateString[3] );
        month = str2bcd( quickDateString[5], quickDateString[6] );
        day   = str2bcd( quickDateString[8], quickDateString[9] );

        hour = str2bcd( quickDateString[11], quickDateString[12] );
        min  = str2bcd( quickDateString[14], quickDateString[15] );
        sec  = str2bcd( quickDateString[17], quickDateString[18] );

        dow  = str2bcd( '0', quickDateString[21] );
        return true;
    }
    return false;
}




uint8_t DateTime::bcd2uint(uint8_t bcd)
{
    /// @todo Check for non dec num in bcd...
    uint8_t res = (bcd&0x0F);
    bcd = bcd >> 4;
    res += (bcd*10);
    return res;
}

uint8_t DateTime::str2bcd(char ch0, char ch1)
{
    uint8_t res;

    if(ch0 > '9' || ch0 < '0')
        ch0 = '0';

    if(ch1 > '9' || ch1 < '0')
        ch1 = '0';

    res = ch0-'0';
    res = res << 4;
    res += ch1-'0';

    return res;
}
