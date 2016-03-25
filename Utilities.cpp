#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "Utilities.h"

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

void getDateTime(char *bytes)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    bytes[0] = (uint8_t)tm.tm_year;
    bytes[1] = (uint8_t)tm.tm_mon + 1;
    bytes[2] = (uint8_t)tm.tm_mday;
    bytes[3] = (uint8_t)tm.tm_hour;
    bytes[4] = (uint8_t)tm.tm_min;
    bytes[5] = (uint8_t)tm.tm_sec;
    bytes[6] = 44;
}

char * getASCIIDateTime()
{    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char * asciiTime = new char [17];
    sprintf(&asciiTime[0], "%04d", tm.tm_year);
    sprintf(&asciiTime[4], "%02d", tm.tm_mon);
    sprintf(&asciiTime[6], "%02d", tm.tm_mday + 1);
    sprintf(&asciiTime[8], "%02d", tm.tm_hour);
    sprintf(&asciiTime[10], "%02d", tm.tm_min);
    sprintf(&asciiTime[12], "%02d", tm.tm_sec);
    sprintf(&asciiTime[14], "%02d", 0);
    asciiTime[16] = 44;
    
    return asciiTime;
}