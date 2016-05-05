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

void initRootRecord(DirectoryRecord &rcd);
void writeDirectory(ofstream imgPath);
int getDirectorySize(ProgOpts* po);
int createDirectory(ofstream &imgPath, ProgOpts* po, DirectoryRecord * rootRecord);
void writeRecord(DirectoryRecord &rcd, ofstream &imgPath, struct dirent *direcRent, int &extentSector, string path);
void writeIdentifierFile(DirectoryRecord &rcd,ofstream &imgPath,string filename,int &extentSector);

int main(int argc, char** argv)
{
    ProgOpts * po = parse(argc, argv);
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        int pathTableSize;
        int lastSector;
        
        pathTableSize += sizeof(SPathRcd) + 2;
        LPathRecord * lpr = new LPathRecord(1, 20, 1, (char*)"\0");
        MPathRecord * mpr = new MPathRecord(1, 20, 1, (char*)"\0");
        imgPath.seekp(18 * 2048, ios_base::beg);
        lpr->write(imgPath);
        imgPath.seekp(19 * 2048, ios_base::beg);
        mpr->write(imgPath);
        
        DirectoryRecord * rootRecord = new DirectoryRecord(1);
        lastSector = createDirectory(imgPath, po, rootRecord);
        
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pathTableSize);
        pvd->vd.rootDirRecord = *rootRecord->dr; //Copy the record
        imgPath.seekp(16 * 2048, ios_base::beg);
        pvd->write(imgPath);
        
        VolumeDescriptor * tvd = new VolumeDescriptor();
        imgPath.seekp(17 * 2048, ios_base::beg);
        tvd->write(imgPath);

        imgPath.seekp(lastSector * 2048, ios_base::beg);
        char buf[1] = {'\0'};
        imgPath.write(buf, 1);
    }
    imgPath.close();
    
    return 0;
}

void initRootRecord(DirectoryRecord &rcd, int size) {
    BothEndianInt extent, exsize;
    BothEndianShort sequenceNumber;
    char cTime[7];

    //Set the bothendians
    extent.setValue(20);
    exsize.setValue(size);
    sequenceNumber.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;

    //Fill in the default information
    rcd.dr->xa_length = 0;
    rcd.dr->flags = 2;
    rcd.dr->unit_size = 0;
    rcd.dr->gap = 0;
    rcd.dr->filename.len = 1;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recording_time, cTime, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, exsize.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->sequence_number, sequenceNumber.getBytes(),
            sizeof (rcd.dr->sequence_number));
}

int getDirectorySize(ProgOpts * po) {
    DIR *direc;
    struct dirent *direcRent;
    int size = 68;
    int recordSize = 0;
    
    direc = opendir(po->dirTree.c_str());
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
    ProgOpts* po,
    DirectoryRecord * rootRecord
){
    DIR *direc;
    struct dirent *direcRent;
    BothEndianInt extent;
    int currentLocation = 2048 * 20;
    DirectoryRecord * currentRecord;
    int dirTreeSize = getDirectorySize(po);
    int extentSector = 20 + ceil(dirTreeSize / 2048.0);
    
    //Create Root Record
    initRootRecord(*rootRecord, dirTreeSize);
    currentLocation += rootRecord->dr->length;
    
    //Create parent record and write it
    DirectoryRecord * parentRecord = new DirectoryRecord(1);
    initRootRecord(*parentRecord, dirTreeSize);
    extent.setValue(16);
    memcpy(parentRecord->dr->extent, extent.getBytes(), sizeof (parentRecord->dr->extent));
    imgPath.seekp(currentLocation, ios_base::beg);
    parentRecord->write(imgPath);
    currentLocation += parentRecord->dr->length;
    
    //Iterate over the directory
    direc = opendir(po->dirTree.c_str());
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
            writeRecord(*currentRecord, imgPath, direcRent, extentSector, po->dirTree);
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
    idFile = string(po->copyrightFileID,12);
    if (!(idFile == "            ")){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeIdentifierFile(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    idFile = string(po->abstractFileID,12);
    if (!(idFile == "            ")){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeIdentifierFile(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    idFile = string(po->bibliographicFileID,12);
    if (!(idFile == "            ")){
        trimEnd = idFile.find_last_not_of(" ") + 1;
        idFile = idFile.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(idFile.length()+2);
        writeIdentifierFile(*currentRecord,imgPath,idFile,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        currentLocation += currentRecord->dr->length;
        currentRootSize += currentRecord->dr->length;
        delete currentRecord;
    }
    rootSize.setValue(currentRootSize);
    memcpy(rootRecord->dr->size, rootSize.getBytes(), sizeof (rootRecord->dr->size));
    
    //Write the root record
    imgPath.seekp(2048 * 20, ios_base::beg);
    rootRecord->write(imgPath);
    
    return extentSector;
}

void writeRecord(
    DirectoryRecord &rcd,
    ofstream &imgPath,
    struct dirent *direcRent,
    int &extentSector,
    string path
){
    BothEndianInt extent, size;
    BothEndianShort sequenceNumber;
    char cTime[7];
    struct stat st;
    int status, amt;
    char buffer[1000];
    int fildes;
    
    path.append(direcRent->d_name);
    fildes = open(path.c_str(), O_RDWR);
    status = fstat(fildes, &st);

    //Set the bothendians
    extent.setValue(extentSector);
    size.setValue(st.st_size);
    sequenceNumber.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;

    //Fill in the default information
    rcd.dr->xa_length = 0;
    rcd.dr->flags = 0;
    rcd.dr->unit_size = 0;
    rcd.dr->gap = 0;
    rcd.dr->filename.len = strlen(direcRent->d_name) + 2;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recording_time, cTime, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, size.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->sequence_number, sequenceNumber.getBytes(),
            sizeof (rcd.dr->sequence_number));
    
    char * ptr = (rcd.dr->filename.str) + sizeof(char);
    memcpy(ptr, direcRent->d_name, sizeof(strlen(direcRent->d_name)));
    ptr += (sizeof(char) * strlen(direcRent->d_name));
    char ver[2] = {';','1'};
    memcpy(ptr, ver, sizeof(ver));
    
    ifstream recordFile;
    imgPath.seekp(extentSector * 2048, ios_base::beg);
    
    recordFile.open (path, ifstream::binary);
    if (recordFile.is_open()){
        while ((amt = recordFile.readsome(buffer,1000)) > 0){
          imgPath.write(buffer,amt);
        }
        recordFile.close();
    }
    
    extentSector += ceil(st.st_size / 2048.0);
}

void writeIdentifierFile(
    DirectoryRecord &rcd,
    ofstream &imgPath,
    string filename,
    int &extentSector
){
    BothEndianInt extent, size;
    BothEndianShort sequenceNumber;
    char cTime[7];
    struct stat st;
    int status, amt;
    char buffer[1000];
    int fildes;
    
    fildes = open(filename.c_str(), O_RDWR);
    status = fstat(fildes, &st);

    //Set the bothendians
    extent.setValue(extentSector);
    size.setValue(st.st_size);
    sequenceNumber.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;

    //Fill in the default information
    rcd.dr->xa_length = 0;
    rcd.dr->flags = 0;
    rcd.dr->unit_size = 0;
    rcd.dr->gap = 0;
    rcd.dr->filename.len = filename.length() + 2;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recording_time, cTime, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, size.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->sequence_number, sequenceNumber.getBytes(),
            sizeof (rcd.dr->sequence_number));
    
    char * ptr = (rcd.dr->filename.str) + sizeof(char);
    memcpy(ptr, filename.c_str(), filename.length());
    ptr += (sizeof(char) * filename.length());
    char ver[2] = {';','1'};
    memcpy(ptr, ver, sizeof(ver));
    
    ifstream recordFile;
    imgPath.seekp(extentSector * 2048, ios_base::beg);
    
    recordFile.open (filename, ifstream::binary);
    if (recordFile.is_open()){
        while ((amt = recordFile.readsome(buffer,1000)) > 0){
          imgPath.write(buffer,amt);
        }
        recordFile.close();
    }
    
    extentSector += ceil(st.st_size / 2048.0);
}