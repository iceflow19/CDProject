#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

#include "VolDesc.h"
#include "LPathRec.h"
#include "MPathRec.h"
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "OptParse.h"

using namespace std;

void getDT(char * bytes);

int main(int argc, char** argv)
{
    ProgOpts * po = parse(argc, argv);
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        int pathTableSize;
        pathTableSize += sizeof(SPathRcd) + 2;
        LPathRecord * lpr = new LPathRecord(1, 20, 1, (char*)"\0");
        MPathRecord * mpr = new MPathRecord(1, 20, 1, (char*)"\0");
        DirectoryRecord * root = new  DirectoryRecord(1);
        BothEndianInt ex, size;
        BothEndianShort seqNum;
        char time[7];
        getDT(time);
        root->dr->length = 34;
        root->dr->xa_length = 0;
        ex.setValue(20);
        memcpy(&root->dr->extent, ex.getBytes(), sizeof(root->dr->extent));
        size.setValue(34);
        memcpy(&root->dr->size, size.getBytes(), sizeof(root->dr->size));
        seqNum.setValue(1);
        memcpy(&root->dr->sequence_number, seqNum.getBytes(), sizeof(root->dr->sequence_number));
        root->dr->flags = 2;
        root->dr->unit_size = 0;
        root->dr->gap = 0;
        root->dr->filename.len = 1;
        memcpy(&root->dr->recording_time,time,7);
        imgPath.seekp(18 * 2048, ios_base::beg);
        lpr->write(imgPath);
        imgPath.seekp(19 * 2048, ios_base::beg);
        mpr->write(imgPath);
        imgPath.seekp(20 * 2048, ios_base::beg);
        root->write(imgPath);
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pathTableSize);
        pvd->vd.rootDirRecord = *root->dr;
        imgPath.seekp(16 * 2048, ios_base::beg);
        pvd->write(imgPath);
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(17 * 2048, ios_base::beg);
        tvd->write(imgPath);
        imgPath.seekp(21 * 2048, ios_base::beg);
        char buf[1] = {'\0'};
        imgPath.write(buf,1);
    }
    imgPath.close();
    
    return 0;
}

void getDT(char *bytes)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    bytes[0] = (uint8_t)tm.tm_year;
    bytes[1] = (uint8_t)tm.tm_mon + 1;
    bytes[2] = (uint8_t)tm.tm_mday;
    bytes[3] = (uint8_t)tm.tm_hour;
    bytes[4] = (uint8_t)tm.tm_min;
    bytes[5] = (uint8_t)tm.tm_sec;
    bytes[6] = 44;
}