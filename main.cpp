#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

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
int createDirectory(ofstream &imgPath, ProgramOptions* po, DirectoryRecord * rootRecord);
void writeFileExtent(DirectoryRecord &rcd, ofstream &imgPath, string filename,
        int &extentSector,string path = "");

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
        lastSector = createDirectory(imgPath, po, rootRecord);
        
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

int createDirectory(
    ofstream &imgPath,
    ProgramOptions* po,
    DirectoryRecord * rootRecord
){
    DIR *direc;
    struct dirent *direcRent;
    BothEndianInt extent;
    int currentLocation = LOGICAL_SECTOR_SIZE * ROOT_RECORD_SECTOR;
    DirectoryRecord * currentRecord;
    int dirTreeSize = getDirectorySize(po);
    int extentSector = ROOT_RECORD_SECTOR + ceil(dirTreeSize / 2048.0);
    
    //Create Root Record
    initRootRecord(*rootRecord, dirTreeSize);
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
            writeFileExtent(*currentRecord, imgPath,string(direcRent->d_name),
                    extentSector, po->directoryTree);
            imgPath.seekp(currentLocation, ios_base::beg);
            currentRecord->write(imgPath);
            currentLocation += currentRecord->dr->length;
            delete currentRecord;
        }
        closedir(direc);
    }
    
    //Add the identifier files
    string idFile;
    int trimEnd;
    BothEndianInt rootSize(rootRecord->dr->size);
    int currentRootSize = rootSize.getValue();
    idFile = string(po->copyrightFileIdentifier,12);
    if (!(idFile == BLANK_FILE_IDENTIFIER)){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeFileExtent(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    idFile = string(po->abstractFileIdentifier,12);
    if (!(idFile == BLANK_FILE_IDENTIFIER)){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeFileExtent(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    idFile = string(po->bibliographicFileIdentifier,12);
    if (!(idFile == BLANK_FILE_IDENTIFIER)){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeFileExtent(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    rootSize.setValue(currentRootSize);
    memcpy(rootRecord->dr->size, rootSize.getBytes(), sizeof (rootRecord->dr->size));
    
    //Write the root record
    imgPath.seekp(LOGICAL_SECTOR_SIZE * ROOT_RECORD_SECTOR, ios_base::beg);
    rootRecord->write(imgPath);
    
    return extentSector;
}

void writeFileExtent(
    DirectoryRecord &rcd,
    ofstream &imgPath,
    string filename,
    int &extentSector,
    string path
){
    BothEndianInt extent, size;
    BothEndianShort sequenceNumber;
    char time[7];
    struct stat st;
    int status, amt;
    char buffer[1000];
    int fildes;
    
    path.append(filename);
    fildes = open(path.c_str(), O_RDWR);
    status = fstat(fildes, &st);

    //Set the bothendians
    extent.setValue(extentSector);
    size.setValue(st.st_size);
    sequenceNumber.setValue(1);
    getDateTime(time);

    //Fill in the default information
    rcd.dr->xaLength = 0;
    rcd.dr->fileFlags = 0;
    rcd.dr->fileUnitSize = 0;
    rcd.dr->interleaveGap = 0;
    rcd.dr->filename.len = filename.length() + 2;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recordingTime, time, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, size.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->volumeSequenceNumber, sequenceNumber.getBytes(),
            sizeof (rcd.dr->volumeSequenceNumber));
    
    char * ptr = (rcd.dr->filename.str) + sizeof(char);
    memcpy(ptr, filename.c_str(), filename.length());
    ptr += (sizeof(char) * filename.length());
    char ver[2] = {';','1'};
    memcpy(ptr, ver, sizeof(ver));
    
    ifstream recordFile;
    imgPath.seekp(extentSector * LOGICAL_SECTOR_SIZE, ios_base::beg);
    
    recordFile.open (filename, ifstream::binary);
    if (recordFile.is_open()){
        while ((amt = recordFile.readsome(buffer,1000)) > 0){
          imgPath.write(buffer,amt);
        }
        recordFile.close();
    }
    
    extentSector += ceil(st.st_size / 2048.0);
}