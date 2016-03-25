#include <stdio.h>
#include <string.h>
#include "MPathRecord.h"
#include "Utilities.h"


MPathRecord::MPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID)
{
    rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    rcd->length = idLength;
    rcd->xaLength = 0;
    rcd->extent = extentLocation;
    rcd->parentDirectoryNumber = parentDirNum;
    memcpy(rcd->directoryID, dirID, idLength);
    
    swapEndian((char *)&rcd->length, sizeof(rcd->length));
    swapEndian((char *)&rcd->extent, sizeof(rcd->extent));
    swapEndian((char *)&rcd->xaLength, sizeof(rcd->xaLength));
    swapEndian((char *)&rcd->parentDirectoryNumber, sizeof(rcd->parentDirectoryNumber));
}

MPathRecord::MPathRecord(PathRecordS &rcd)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + rcd.length);
    memcpy(this->rcd, &rcd, sizeof(PathRecordS) + rcd.length);
}

void MPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(PathRecordS) + this->rcd->length);
}