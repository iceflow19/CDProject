#include <stdio.h>
#include <string.h>
#include "LPathRecord.h"


LPathRecord::LPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID)
{
    rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    rcd->length = idLength;
    rcd->xaLength = 0;
    rcd->extent = extentLocation;
    rcd->parentDirectoryNumber = parentDirNum;
    memcpy(rcd->directoryID, dirID, idLength);
}

LPathRecord::LPathRecord(PathRecordS &rcd)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + rcd.length);
    memcpy(this->rcd, &rcd, sizeof(PathRecordS) + rcd.length);
}

void LPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(PathRecordS) + this->rcd->length);
}