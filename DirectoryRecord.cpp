#include <stdio.h>
#include <string.h>
#include "DirectoryRecord.h"

//Allocate a new DirectoryRecord given the file name size
DirectoryRecord::DirectoryRecord(int fileNameSize)
{
    this->dr = (DirectoryRecordS*)malloc(sizeof(DirectoryRecordS) + fileNameSize);
}

//Copy Constructor
DirectoryRecord::DirectoryRecord(DirectoryRecordS &dr)
{
    this->dr = (DirectoryRecordS*)malloc(sizeof(DirectoryRecordS) + dr.filename.len);
    memcpy(&(this->dr), &dr, sizeof(DirectoryRecordS) + dr.filename.len);
}

//Serialize the backing struct out to the file
void DirectoryRecord::write(std::ofstream &image)
{
    image.write((char *)(this->dr), sizeof(DirectoryRecordS) + this->dr->filename.len);
}