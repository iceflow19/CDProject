/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "MPathRecord.h"
#include <stdio.h>
#include <string.h>
#include "Utilities.h"


MPathRecord::MPathRecord(int idLength, int extentLocation, int pdn, char * id)
{
    rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    rcd->length = idLength;
    rcd->xa_length = 0;
    rcd->extent = extentLocation;
    rcd->parentDirectoryNumber = pdn;
    memcpy(rcd->directoryID, id, idLength);
    
    swapEndian((char *)&rcd->length, sizeof(rcd->length));
    swapEndian((char *)&rcd->extent, sizeof(rcd->extent));
    swapEndian((char *)&rcd->xa_length, sizeof(rcd->xa_length));
    swapEndian((char *)&rcd->parentDirectoryNumber, sizeof(rcd->parentDirectoryNumber));
}

MPathRecord::MPathRecord(PathRecordS &rcd)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + rcd.length);
    memcpy(&(this->rcd), &rcd, sizeof(PathRecordS) + rcd.length);
}

void MPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(PathRecordS) + this->rcd->length);
}