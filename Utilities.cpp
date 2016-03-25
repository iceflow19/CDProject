/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Utilities.h"
#include <time.h>

void swapEndian(char * buf, int length)
{
    char * a;
    char * b;
    
    a = buf;
    b = buf + ((length - 1) * sizeof(char));
    
    for (int len = length / 2; len > 0; --len)
    {
        char temp;
        temp = *a;
        *a = *b;
        *b = temp;
        ++a;
        --b;
    }
}

void getDateTimeNow(char *bytes)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    bytes[0] = (uint8_t)tm.tm_year;
    bytes[1] = (uint8_t)tm.tm_mon;
    bytes[2] = (uint8_t)tm.tm_mday;
    bytes[3] = (uint8_t)tm.tm_hour;
    bytes[4] = (uint8_t)tm.tm_min;
    bytes[5] = (uint8_t)tm.tm_sec;
    bytes[6] = 44;
}