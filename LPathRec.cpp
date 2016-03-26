#include <stdio.h>
#include <string.h>
#include "LPathRec.h"

LPathRecord::LPathRecord(int idLen, int extentLoc, int pdn, char * dirID)
{
    rcd = (SPathRcd*)malloc(sizeof(SPathRcd) + idLen);
    rcd->length = idLen;
    rcd->xa_length = 0;
    rcd->extent = extentLoc;
    rcd->parent_dir_num = pdn;
    memcpy(rcd->directory_id, dirID, idLen);
}

void LPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(SPathRcd) + this->rcd->length);
}