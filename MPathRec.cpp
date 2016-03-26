#include <stdio.h>
#include <string.h>
#include "MPathRec.h"

void swap(char * buf, int length);

MPathRecord::MPathRecord(int idLen, int extentLoc, int pdn, char * dirID)
{
    rcd = (SPathRcd*)malloc(sizeof(SPathRcd) + idLen);
    rcd->length = idLen;
    rcd->xa_length = 0;
    rcd->extent = extentLoc;
    rcd->parent_dir_num = pdn;
    memcpy(rcd->directory_id, dirID, idLen);
    
    swap((char *)&rcd->length, sizeof(rcd->length));
    swap((char *)&rcd->extent, sizeof(rcd->extent));
    swap((char *)&rcd->xa_length, sizeof(rcd->xa_length));
    swap((char *)&rcd->parent_dir_num, sizeof(rcd->parent_dir_num));
}

void MPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(SPathRcd) + this->rcd->length);
}

void swap(char * buf, int length)
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