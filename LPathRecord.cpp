#include <stdio.h>
#include <string.h>
#include "LPathRecord.h"

//Constructor for an LPath Record
LPathRecord::LPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID)
{
    rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    rcd->length = idLength;
    rcd->xaLength = 0;
    rcd->extent = extentLocation;
    rcd->parentDirectoryNumber = parentDirNum;
    memcpy(rcd->directoryID, dirID, idLength);
}

//Copy Constructor
LPathRecord::LPathRecord(PathRecordS &rcd)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + rcd.length);
    memcpy(this->rcd, &rcd, sizeof(PathRecordS) + rcd.length);
}

//Serialize the backing struct out to the iso file
void LPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(PathRecordS) + this->rcd->length);
}