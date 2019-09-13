#include "defines.h"
#include "String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *Int2String(int n, char *buffer)
{
    //        if (n < 0) { //turns n positive
    //               n = (-1 * n);
    //                u = "-"; //adds '-' on result string
    //        }

    char c;
    int i = 0;

    do
    {
        buffer[i++] = n % 10 + '0';
        n -= n % 10;
    } while ((n /= 10) > 0);

    for (int j = 0; j < i; j++, i--)
    {
        c = buffer[j];
        buffer[j] = buffer[i];
        buffer[i] = c;
    }

    return buffer;
}
