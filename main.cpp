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

void write1(DirectoryRecord &record, int sz) 
{
    BothEndianInt extent, exsize;
    BothEndianShort sequence;
    char cTime[7];
    
    extent.setValue(20);
    exsize.setValue(sz);
    sequence.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;
    
    record.dr->xa_length = 0;
    record.dr->flags = 2;
    record.dr->unit_size = 0;
    record.dr->gap = 0;
    record.dr->filename.len = 1;
    memcpy(&record.dr->recording_time, cTime, 7);
    memcpy(&record.dr->extent, extent.getBytes(), sizeof (record.dr->extent));
    memcpy(&record.dr->size, exsize.getBytes(), sizeof (record.dr->size));
    memcpy(&record.dr->sequence_number, sequence.getBytes(), sizeof (record.dr->sequence_number));
}

void write2(DirectoryRecord &record, ofstream &img, struct dirent *dirnt, int &extentSect, string path)
{
    struct stat st;
    BothEndianInt extent, size;
    BothEndianShort sequence;
    int status, amt, fildes;
    char buffer[1000], cTime[7];
    
    path.append(dirnt->d_name);
    fildes = open(path.c_str(), O_RDWR);
    status = fstat(fildes, &st);
    extent.setValue(extentSect);
    size.setValue(st.st_size);
    sequence.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;

    record.dr->xa_length = 0;
    record.dr->flags = 0;
    record.dr->unit_size = 0;
    record.dr->gap = 0;
    record.dr->filename.len = strlen(dirnt->d_name) + 2;
    memcpy(&record.dr->recording_time, cTime, 7);
    memcpy(&record.dr->extent, extent.getBytes(), sizeof (record.dr->extent));
    memcpy(&record.dr->size, size.getBytes(), sizeof (record.dr->size));
    memcpy(&record.dr->sequence_number, sequence.getBytes(), sizeof (record.dr->sequence_number));
    char * ptr = (record.dr->filename.str) + sizeof(char);
    memcpy(ptr, dirnt->d_name, sizeof(strlen(dirnt->d_name)));
    ptr += (sizeof(char) * strlen(dirnt->d_name));
    char ver[2] = {';','1'};
    memcpy(ptr, ver, sizeof(ver));
    ifstream recordFile;
    img.seekp(extentSect * 2048, ios_base::beg);
    recordFile.open (path, ifstream::binary);
    
    if (recordFile.is_open())
    {
        while ((amt = recordFile.readsome(buffer,1000)) > 0)
          img.write(buffer,amt);
        recordFile.close();
    }
    extentSect += ceil(st.st_size / 2048.0);
}

void writeExtras(DirectoryRecord &record, ofstream &img, string name, int &extentSect)
{
    struct stat st;
    BothEndianInt extent, size;
    BothEndianShort sequence;
    int status, amt, fildes;
    char buffer[1000], cTime[7];
    
    fildes = open(name.c_str(), O_RDWR);
    status = fstat(fildes, &st);
    extent.setValue(extentSect);
    size.setValue(st.st_size);
    sequence.setValue(1);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    cTime[0] = (uint8_t)tm.tm_year;
    cTime[1] = (uint8_t)tm.tm_mon + 1;
    cTime[2] = (uint8_t)tm.tm_mday;
    cTime[3] = (uint8_t)tm.tm_hour;
    cTime[4] = (uint8_t)tm.tm_min;
    cTime[5] = (uint8_t)tm.tm_sec;
    cTime[6] = 44;

    record.dr->xa_length = 0;
    record.dr->flags = 0;
    record.dr->unit_size = 0;
    record.dr->gap = 0;
    record.dr->filename.len = name.length() + 2;
    memcpy(&record.dr->recording_time, cTime, 7);
    memcpy(&record.dr->extent, extent.getBytes(), sizeof (record.dr->extent));
    memcpy(&record.dr->size, size.getBytes(), sizeof (record.dr->size));
    memcpy(&record.dr->sequence_number, sequence.getBytes(), sizeof (record.dr->sequence_number));
    char * ptr = (record.dr->filename.str) + sizeof(char);
    memcpy(ptr, name.c_str(), name.length());
    ptr += (sizeof(char) * name.length());
    char ver[2] = {';','1'};
    memcpy(ptr, ver, sizeof(ver));
    ifstream recordFile;
    img.seekp(extentSect * 2048, ios_base::beg);
    
    recordFile.open (name, ifstream::binary);
    if (recordFile.is_open())
    {
        while ((amt = recordFile.readsome(buffer,1000)) > 0)
          img.write(buffer,amt);
        recordFile.close();
    }
    
    extentSect += ceil(st.st_size / 2048.0);
}


int main(int argc, char** argv)
{
    ProgOpts * po = parse(argc, argv);
    ofstream img;
    string idFile;
    int trimEnd;
        
    img.open("out.iso", ios_base::out | ios_base::binary);
    if (img.is_open())
    {
        int pTbSize, finalSec;
        pTbSize += sizeof(SPathRcd) + 2;
        LPathRecord * lpr = new LPathRecord(1, 20, 1, (char*)"\0");
        MPathRecord * mpr = new MPathRecord(1, 20, 1, (char*)"\0");
        img.seekp(18 * 2048, ios_base::beg);
        lpr->write(img);
        img.seekp(19 * 2048, ios_base::beg);
        mpr->write(img);
        DirectoryRecord * rootRecord = new DirectoryRecord(1);
        
        // Create the directory
        DIR *dir, *direc;
        struct dirent *dirnt, *direcRent;
        BothEndianInt extent;
        int cLoc = 2048 * 20;
        DirectoryRecord * cRec;
        int dtSize;
        int size = 68;
        int recordSize = 0;

        // Get the directory tree's size
        direc = opendir(po->dirTree.c_str());
        if (direc) 
        {
            while ((direcRent = readdir(direc)) != NULL) 
            {
                if (strcmp(direcRent->d_name, ".") == 0)
                {
                    continue;
                }
                else if (strcmp(direcRent->d_name, "..") == 0)
                {
                    continue;
                }
                
                recordSize = 33 + strlen(direcRent->d_name) + 2;
                
                if (size % 2048 + recordSize <= 2048)
                    size += recordSize;
                else
                    size += (2048 - (size % 2048));
            }
            closedir(direc);
        }
        dtSize = size;
        int extentSect = ceil(dtSize / 2048.0) + 20;
        write1(*rootRecord, dtSize);
        cLoc += rootRecord->dr->length;
        DirectoryRecord * parentRecord = new DirectoryRecord(1);
        write1(*parentRecord, dtSize);
        extent.setValue(16);
        memcpy(parentRecord->dr->extent, extent.getBytes(), sizeof (parentRecord->dr->extent));
        img.seekp(cLoc, ios_base::beg);
        parentRecord->write(img);
        cLoc += parentRecord->dr->length;
        dir = opendir(po->dirTree.c_str());
        if (dir) 
        {
            while ((dirnt = readdir(dir)) != NULL) 
            {
                if (strcmp(dirnt->d_name, ".") == 0)
                {
                    continue;
                }
                else if (strcmp(dirnt->d_name, "..") == 0)
                {
                    continue;
                }
                
                cRec = new DirectoryRecord(strlen(dirnt->d_name) + 2);
                
                if (!(cLoc % 2048 + cRec->dr->length <= 2048))
                    cLoc += (2048 - (cLoc % 2048));
                
                write2(*cRec, img, dirnt, extentSect, po->dirTree);
                img.seekp(cLoc, ios_base::beg);
                cRec->write(img);
                cLoc += cRec->dr->length;
                delete cRec;
            }
            closedir(dir);
        }
        
        BothEndianInt rootSize(rootRecord->dr->size);
        int currentRootSize = rootSize.getValue();
        
        idFile = string(po->copyrightFileID,12);
        
        if (!(idFile == "            "))
        {
            trimEnd = idFile.find_last_not_of(" ") + 1;
            idFile = idFile.substr(0, trimEnd);
            cRec = new DirectoryRecord(idFile.length()+2);
            writeExtras(*cRec,img,idFile,extentSect);
            img.seekp(cLoc, ios_base::beg);
            cRec->write(img);
            cLoc += cRec->dr->length;
            currentRootSize += cRec->dr->length;
            delete cRec;
        }
        
        idFile = string(po->abstractFileID,12);
        
        if (!(idFile == "            "))
        {
            trimEnd = idFile.find_last_not_of(" ") + 1;
            idFile = idFile.substr(0, trimEnd);
            cRec = new DirectoryRecord(idFile.length()+2);
            writeExtras(*cRec,img,idFile,extentSect);
            img.seekp(cLoc, ios_base::beg);
            cRec->write(img);
            cLoc += cRec->dr->length;
            currentRootSize += cRec->dr->length;
            delete cRec;
        }
        
        idFile = string(po->bibliographicFileID,12);
        
        if (!(idFile == "            "))
        {
            trimEnd = idFile.find_last_not_of(" ") + 1;
            idFile = idFile.substr(0, trimEnd);
            cRec = new DirectoryRecord(idFile.length()+2);
            writeExtras(*cRec,img,idFile,extentSect);
            img.seekp(cLoc, ios_base::beg);
            cRec->write(img);
            cLoc += cRec->dr->length;
            currentRootSize += cRec->dr->length;
            delete cRec;
        }
        
        rootSize.setValue(currentRootSize);
        memcpy(rootRecord->dr->size, rootSize.getBytes(), sizeof (rootRecord->dr->size));
        img.seekp(2048 * 20, ios_base::beg);
        rootRecord->write(img);
        finalSec = extentSect;
        
        //Finish Out
        VolumeDescriptor * pvd = new VolumeDescriptor(po, pTbSize);
        pvd->vd.rootDirRecord = *rootRecord->dr;
        img.seekp(16 * 2048, ios_base::beg);
        pvd->write(img);
        VolumeDescriptor * tvd = new VolumeDescriptor();
        img.seekp(17 * 2048, ios_base::beg);
        tvd->write(img);
        img.seekp(finalSec * 2048, ios_base::beg);
        char buf[1] = {'\0'};
        img.write(buf, 1);
    }
    
    img.close();
    
    return 0;
}