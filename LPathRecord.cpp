/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LPathRecord.h"
#include <stdio.h>
#include <string.h>


LPathRecord::LPathRecord(int idLength, int extentLocation, int pdn, char * id)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + idLength);
    this->rcd->length = idLength;
    this->rcd->xa_length = 0;
    this->rcd->extent = extentLocation;
    this->rcd->parentDirectoryNumber = pdn;
    memcpy(this->rcd->directoryID, id, idLength);
}

LPathRecord::LPathRecord(PathRecordS &rcd)
{
    this->rcd = (PathRecordS*)malloc(sizeof(PathRecordS) + rcd.length);
    memcpy(&(this->rcd), &rcd, sizeof(PathRecordS) + rcd.length);
}

void LPathRecord::write(std::ofstream &image)
{
    image.write((char *)(this->rcd), sizeof(PathRecordS) + this->rcd->length);
}