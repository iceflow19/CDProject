#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <math.h>

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
int getDirectorySize(ProgramOptions* po);
int createDirectory(ofstream &imgPath, ProgramOptions* po, int &pathTableSize);
void writeRecord(DirectoryRecord &rcd, int &extentSector);

int main(int argc, char** argv) {
    //Get the command line options
    ProgramOptions * po = parse(argc, argv);

    //Open the file to write
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open()) {
        int pathTableSize;
        int lastSector;
        
        //Create the initial LPATH and MPATH records
        pathTableSize += sizeof (PathRecordS) + 2;
        LPathRecord * lpr = new LPathRecord(1, ROOT_RECORD_SECTOR, 1, (char*) "\0");
        MPathRecord * mpr = new MPathRecord(1, ROOT_RECORD_SECTOR, 1, (char*) "\0");
        imgPath.seekp(LPATH_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        lpr->write(imgPath);
        imgPath.seekp(MPATH_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        mpr->write(imgPath);
        
        //Create root directory and children
        DirectoryRecord * rootRecord = new DirectoryRecord(1);
        lastSector = createDirectory(imgPath, po, pathTableSize);
        
        //Create and write Primary Volume Descriptor
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pathTableSize);
        pvd->vd.rootDirectoryRecord = *rootRecord->dr; //Copy the record
        imgPath.seekp(PVD_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        pvd->write(imgPath);
        
        //Create and write TVD
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(TVD_SECTOR * LOGICAL_SECTOR_SIZE, ios_base::beg);
        tvd->write(imgPath);

        //Pad to the end
        imgPath.seekp(lastSector * LOGICAL_SECTOR_SIZE, ios_base::beg);
        char buf[1] = {'\0'};
        imgPath.write(buf, 1);
    }
    imgPath.close();

    return 0;
}

//Initializes the root record

void initRootRecord(DirectoryRecord &rcd, int size) {
    BothEndianInt extent, exsize;
    BothEndianShort sequenceNumber;
    char time[7];

    //Set the bothendians
    extent.setValue(20);
    exsize.setValue(size);
    sequenceNumber.setValue(1);
    getDateTime(time);

    //Fill in the default information
    rcd.dr->xaLength = 0;
    rcd.dr->fileFlags = 2;
    rcd.dr->fileUnitSize = 0;
    rcd.dr->interleaveGap = 0;
    rcd.dr->filename.len = 1;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recordingTime, time, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, exsize.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->volumeSequenceNumber, sequenceNumber.getBytes(),
            sizeof (rcd.dr->volumeSequenceNumber));
}

int getDirectorySize(ProgramOptions * po) {
    DIR *direc;
    struct dirent *direcRent;
    int size = 68;
    int recordSize = 0;
    
    direc = opendir(po->directoryTree.c_str());
    if (direc) {
        while ((direcRent = readdir(direc)) != NULL) {
            if (strcmp(direcRent->d_name, ".") == 0)
                continue;
            if (strcmp(direcRent->d_name, "..") == 0)
                continue;
            recordSize = 33 + strlen(direcRent->d_name) + 2;
            if (size % 2048 + recordSize <= 2048){
                size += recordSize;
            } else {
                size += (2048 - (size % 2048));
            }
        }
        closedir(direc);
    }
    return size;
}

int createDirectory(ofstream &imgPath, ProgramOptions* po, DirectoryRecord * rootRecord, int &pathTableSize) {
    DIR *direc;
    struct dirent *direcRent;
    BothEndianInt extent;
    int size = 0;
    int currentLocation = LOGICAL_SECTOR_SIZE * ROOT_RECORD_SECTOR;
    DirectoryRecord * currentRecord;
    int dirTreeSize = getDirectorySize(po);
    int extentSector = ceil(dirTreeSize / 2048);
    
    //Create Root Record and write it
    initRootRecord(*rootRecord, dirTreeSize);
    imgPath.seekp(currentLocation, ios_base::beg);
    rootRecord->write(imgPath);
    currentLocation += rootRecord->dr->length;
    
    //Create parent record and write it
    DirectoryRecord * parentRecord = new DirectoryRecord(1);
    initRootRecord(*parentRecord, dirTreeSize);
    extent.setValue(PVD_SECTOR);
    memcpy(parentRecord->dr->extent, extent.getBytes(), sizeof (parentRecord->dr->extent));
    imgPath.seekp(currentLocation, ios_base::beg);
    parentRecord->write(imgPath);
    currentLocation += parentRecord->dr->length;
    
    //Iterate over the directory
    direc = opendir(po->directoryTree.c_str());
    if (direc) {
        while ((direcRent = readdir(direc)) != NULL) {
            if (strcmp(direcRent->d_name, ".") == 0)
                continue;
            if (strcmp(direcRent->d_name, "..") == 0)
                continue;
            currentRecord = new DirectoryRecord(strlen(direcRent->d_name) + 2);
            if (!(currentLocation % 2048 + currentRecord->dr->length <= 2048)){
                currentLocation += (2048 - (currentLocation % 2048)); //skip ahead
            }
            imgPath.seekp(currentLocation, ios_base::beg);
            writeRecord(*currentRecord, extentSector);
            currentRecord->write(imgPath);
            currentLocation += currentRecord->dr->length;
            delete currentRecord;
        }
        closedir(direc);
    }
    
    //Add the bullshit files
    
    return extentSector;
}

void writeRecord(DirectoryRecord &rcd, int &extentSector) {
    BothEndianInt extent, size;
    BothEndianShort sequenceNumber;
    char time[7];

    //Set the bothendians
    //extent.setValue(20);
    //size.setValue(34);
    //sequenceNumber.setValue(1);
    //getDateTime(time);

    //Fill in the default information
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