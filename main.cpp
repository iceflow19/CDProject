#include <cstdlib>
#include "OptionParser.h"
#include "VolumeDescriptor.h"
#include "BothEndianInt.h"
#include "Utilities.h"
#include "BothEndianShort.h"
#include "LPathRecord.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    ProgramOptions * po = parse(argc, argv);
    
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        int pathTableSize;
        imgPath.seekp(16*2048, ios_base::beg);
        VolumeDescriptor * pvd = new VolumeDescriptor(po);
        DirectoryRecord * rootRecord = new  DirectoryRecord(1);
        initRootRecord(*rootRecord);
        memcpy(pvd->vd.root_directory_record,*rootRecord,sizeof(DirectoryRecordS));
        pvd->write(imgPath);
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(17*2048, ios_base::beg);
        tvd->write(imgPath);
        pathTableSize += sizeof(PathRecordS) + 1;
        LPathRecord * lpr = new LPathRecord(1, 20, 1, 0, "\0");
        
        
    }
    imgPath.close();
    return 0;
}

void initRootRecord(DirectoryRecord &rcd)
{
    rcd.dr->length = 34;
    rcd.dr->xa_length = 0;
    BothEndianInt extent = BothEndianInt();
    extent.setValue(0);
    rcd.dr->extent = *(extent.getBytes());
    BothEndianInt size = BothEndianInt();
    size.setValue(34);
    rcd.dr->size = *(size.getBytes());
    char[7] time;
    getDateTimeNow(&time)
    strncpy(rcd.dr->recording_time,time,7);
    rcd.dr->file_flags = 2;
    rcd.dr->file_unit_size = 0;
    rcd.dr->interleave_gap = 0;
    BothEndianShort sequenceNbr = BothEndianShort();
    sequenceNbr.setValue(1);
    rcd.dr->volume_sequence_number = *(sequenceNbr.getBytes());
    rcd.dr->filename.len = 1;
}