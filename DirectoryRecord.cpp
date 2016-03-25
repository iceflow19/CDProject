#include <stdio.h>
#include <string.h>
#include "DirectoryRecord.h"

DirectoryRecord::DirectoryRecord(int fileNameSize)
{
    this->dr = (DirectoryRecordS*)malloc(sizeof(DirectoryRecordS) + fileNameSize);
}

DirectoryRecord::DirectoryRecord(DirectoryRecordS &dr)
{
    this->dr = (DirectoryRecordS*)malloc(sizeof(DirectoryRecordS) + dr.filename.len);
    memcpy(&(this->dr), &dr, sizeof(DirectoryRecordS) + dr.filename.len);
}

void DirectoryRecord::write(std::ofstream &image)
{
    image.write((char *)(this->dr), sizeof(DirectoryRecordS) + this->dr->filename.len);
}