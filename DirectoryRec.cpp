#include <stdio.h>
#include <string.h>
#include "DirectoryRec.h"

DirectoryRecord::DirectoryRecord(int nameSize)
{
    this->dr = (SDirRecord*)malloc(sizeof(SDirRecord) + nameSize);
    this->dr->length = 33 + nameSize;
}

void DirectoryRecord::write(std::ofstream &image)
{
    image.write((char *)(this->dr), sizeof(SDirRecord) + this->dr->filename.len);
}