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
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    this->rcd->length = idLength;
    this->rcd->xa_length = 0;
    this->rcd->extent = extentLocation;
    this->rcd->parentDirectoryNumber = pdn;
    memcpy(this->rcd->directoryID, id, idLength);
    
    swapEndian(this->rcd->length, sizeof(this->rcd->length));
    swapEndian(this->rcd->extent, sizeof(this->rcd->extent));
    swapEndian(this->rcd->xa_length, sizeof(this->rcd->xa_length));
    swapEndian(this->rcd->parentDirectoryNumber, sizeof(this->rcd->parentDirectoryNumber));
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