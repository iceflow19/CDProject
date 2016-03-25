#include <cstdlib>
#include "OptionParser.h"
#include "VolumeDescriptor.h"
#include "BothEndianInt.h"
#include "Utilities.h"
#include "BothEndianShort.h"
#include "LPathRecord.h"
#include "MPathRecord.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>

using namespace std;

void initRootRecord(DirectoryRecord &rcd);

int main(int argc, char** argv)
{
    ProgramOptions * po = parse(argc, argv);
    
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        int pathTableSize;
        imgPath.seekp(16*2048, ios_base::beg);
        pathTableSize += sizeof(PathRecordS) + 2;
        LPathRecord * lpr = new LPathRecord(1, 20, 1, (char*)"\0");
        MPathRecord * mpr = new MPathRecord(1, 20, 1, (char*)"\0");
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pathTableSize);
        DirectoryRecord * rootRecord = new  DirectoryRecord(1);
        initRootRecord(*rootRecord);
        pvd->vd.root_directory_record = *rootRecord->dr;
        pvd->write(imgPath);
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(17*2048, ios_base::beg);
        tvd->write(imgPath);
        imgPath.seekp(18*2048, ios_base::beg);
        lpr->write(imgPath);
        imgPath.seekp(19*2048, ios_base::beg);
        mpr->write(imgPath);
        imgPath.seekp(20*2048, ios_base::beg);
        rootRecord->write(imgPath);
        //Pad to the end
        imgPath.seekp(21*2048, ios_base::beg);
        char buf[1] = {'\0'};
        imgPath.write(buf,1);
    }
    imgPath.close();
    return 0;
}

void initRootRecord(DirectoryRecord &rcd)
{
    rcd.dr->length = 34;
    rcd.dr->xa_length = 0;
    BothEndianInt extent;
    extent.setValue(20);
    memcpy(rcd.dr->extent, extent.getBytes(), sizeof(rcd.dr->extent));
    BothEndianInt size;
    size.setValue(34);
    memcpy(rcd.dr->size, size.getBytes(), sizeof(rcd.dr->size));
    char time[7];
    getDateTimeNow(time);
    memcpy(rcd.dr->recording_time,time,7);
    rcd.dr->file_flags = 2;
    rcd.dr->file_unit_size = 0;
    rcd.dr->interleave_gap = 0;
    BothEndianShort sequenceNbr;
    sequenceNbr.setValue(1);
    memcpy(&rcd.dr->volume_sequence_number, sequenceNbr.getBytes(), sizeof(rcd.dr->volume_sequence_number));
    rcd.dr->filename.len = 1;
}