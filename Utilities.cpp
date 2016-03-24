/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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