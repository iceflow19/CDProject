#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <dirent.h>

#include "VolumeDescriptor.h"
#include "LPathRecord.h"
#include "MPathRecord.h"
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "OptionParser.h"
#include "Utilities.h"
#include "globals.h"

using namespace std;

void initRootRecord(DirectoryRecord &rcd);
void writeDirectory(ofstream imgPath);

int main(int argc, char** argv) {
    //Get the command line options
    ProgramOptions * po = parse(argc, argv);

    //Open the file to write
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open()) {
        int pathTableSize;

        //Create the initial LPATH and MPATH records
        pathTableSize += sizeof (PathRecordS) + 2;
        LPathRecord * lpr = new LPathRecord(1, ROOT_RECORD_SECTOR, 1, (char*) "\0");
        MPathRecord * mpr = new MPathRecord(1, ROOT_RECORD_SECTOR, 1, (char*) "\0");

        //Create the Root Directory Record
        DirectoryRecord * rootRecord = new DirectoryRecord(1);

        //Create and write PVD
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pathTableSize);
        initRootRecord(*rootRecord);
        pvd->vd.rootDirectoryRecord = *rootRecord->dr; //Copy the record
        imgPath.seekp(PVD_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        pvd->write(imgPath);

        //Create and write TVD
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(TVD_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        tvd->write(imgPath);

        //Write LPATH
        imgPath.seekp(LPATH_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        lpr->write(imgPath);

        //Write MPATH
        imgPath.seekp(MPATH_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        mpr->write(imgPath);

        //Write the Root Directory Record
        imgPath.seekp(ROOT_RECORD_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        rootRecord->write(imgPath);

        //Pad to the end
        imgPath.seekp(PADDING_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        char buf[1] = {'\0'};
        imgPath.write(buf, 1);
    }
    imgPath.close();

    return 0;
}

//Initializes the root record

void initRootRecord(DirectoryRecord &rcd) {
    BothEndianInt extent, size;
    BothEndianShort sequenceNumber;
    char time[7];

    //Set the bothendians
    extent.setValue(20);
    size.setValue(34);
    sequenceNumber.setValue(1);
    getDateTime(time);

    //Fill in the default information
    rcd.dr->length = 34;
    rcd.dr->xaLength = 0;
    rcd.dr->fileFlags = 2;
    rcd.dr->fileUnitSize = 0;
    rcd.dr->interleaveGap = 0;
    rcd.dr->filename.len = 1;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recordingTime, time, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, size.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->volumeSequenceNumber, sequenceNumber.getBytes(),
            sizeof (rcd.dr->volumeSequenceNumber));
}

int getDirectorySize(ProgramOptions* po) {
    DIR *direc;
    struct dirent *direcRent;
    int size = 34;
    int tempSize = 0;
    
    direc = opendir(po->directoryTree.c_str());
    if (direc) {
        while ((direcRent = readdir(direc)) != NULL) {
            if (strcmp(direcRent->d_name, ".") == 0)
                continue;
            if (strcmp(direcRent->d_name, "..") == 0)
                continue;
            tempSize = 33 + strlen(direcRent->d_name) + 2;
            if (size % 2048 + tempSize <= 2048){
                size += tempSize;
            } else {
                size += (2048 - (size % 2048));
            }
        }
        closedir(direc);
    }
    return size;
}