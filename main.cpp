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

void initRecord(int size,int location,int flags,int fileNameLength);
int getDirectorySize(ProgramOptions* po);
int createDirectory(ofstream &imgPath, ProgramOptions* po,
        DirectoryRecord * rootRecord);
void writeFileExtent(DirectoryRecord &rcd, ofstream &imgPath, string filename,
        int &extentSector,string path = "");
int writeIdentifier(string file, ofstream &imgPath, int currentLocation,
        int &extentSector);

int main(int argc, char** argv)
{
    //Get the command line options
    ProgramOptions * po = parse(argc, argv);

    //Open the file to write
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        int pathTableSize;
        int lastSector;
        
        //Create the initial LPATH and MPATH records
        pathTableSize += sizeof (PathRecordS) + 2;
        LPathRecord * lpr = new LPathRecord(1, ROOT_RECORD_SECTOR, 1, NULL_CHAR_STR);
        MPathRecord * mpr = new MPathRecord(1, ROOT_RECORD_SECTOR, 1, NULL_CHAR_STR);
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
void initRecord(
    DirectoryRecord &rcd,   // The directory record
    int size,               // Extent size
    int location,           // Extent location
    int flags,              // Record flags 0 or 2
    int fileNameLength      // Length of the file name (usually 1)
){
    BothEndianInt extent, exsize;
    BothEndianShort sequenceNumber;
    char time[7];

    //Set the bothendians
    extent.setValue(location);
    exsize.setValue(size);
    sequenceNumber.setValue(1);
    getDateTime(time);

    //Fill in the default information
    rcd.dr->xaLength = 0;
    rcd.dr->fileFlags = flags;
    rcd.dr->fileUnitSize = 0;
    rcd.dr->interleaveGap = 0;
    rcd.dr->filename.len = fileNameLength;

    //We have to memcpy the byte arrays
    memcpy(&rcd.dr->recordingTime, time, 7);
    memcpy(&rcd.dr->extent, extent.getBytes(), sizeof (rcd.dr->extent));
    memcpy(&rcd.dr->size, exsize.getBytes(), sizeof (rcd.dr->size));
    memcpy(&rcd.dr->volumeSequenceNumber, sequenceNumber.getBytes(),
            sizeof (rcd.dr->volumeSequenceNumber));
}

//Gets the future size of the directory including any skips to the next sector
int getDirectorySize(ProgramOptions * po) {
    DIR *direc;
    struct dirent *direcRent;
    int size = 68;
    int recordSize = 0;
    
    if (po->directoryTree != ""){
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
    }
    return size;
}

//Creates an a one level directory
int createDirectory(
    ofstream &imgPath,              // The handle to the output image
    ProgramOptions* po,             // Program options from console
    DirectoryRecord * rootRecord    // The root directory record
){
    DIR *direc;
    struct dirent *dirEntry;
    BothEndianInt extent;
    int currentLocation, dirTreeSize, extentSector;
    DirectoryRecord * currentRecord;
    string idFile;
    int idSize = 0;
    
    currentLocation = LOGICAL_SECTOR_SIZE * ROOT_RECORD_SECTOR;
    dirTreeSize = getDirectorySize(po);
    extentSector = ROOT_RECORD_SECTOR + ceil(dirTreeSize / LOGICAL_SECTOR_SIZE_D);
    
    //Create Root Record
    initRecord(*rootRecord, dirTreeSize, ROOT_RECORD_SECTOR, DIR_FLAGS, BLNK_RCD_FN_SIZE);
    currentLocation += rootRecord->dr->length;
    
    //Create parent record and write it
    DirectoryRecord * parentRecord = new DirectoryRecord(1);
    initRecord(*parentRecord, dirTreeSize, PVD_SECTOR, DIR_FLAGS, BLNK_RCD_FN_SIZE);
    extent.setValue(PVD_SECTOR);
    memcpy(parentRecord->dr->extent, extent.getBytes(), sizeof(parentRecord->dr->extent));
    imgPath.seekp(currentLocation, ios_base::beg);
    parentRecord->write(imgPath);
    currentLocation += parentRecord->dr->length;
    
    //Iterate over the directory
    if (po->directoryTree != ""){
        direc = opendir(po->directoryTree.c_str());
        if (direc)
        {
            while ((dirEntry = readdir(direc)) != NULL)
            {
                //Ignore self
                if (strcmp(dirEntry->d_name, ".") == 0)
                    continue;
                
                //Ignore parent
                if (strcmp(dirEntry->d_name, "..") == 0)
                    continue;
                currentRecord = new DirectoryRecord(strlen(dirEntry->d_name)+2);
                
                //Do we have enough room?
                if (!(currentLocation % LOGICAL_SECTOR_SIZE +
                    currentRecord->dr->length <= LOGICAL_SECTOR_SIZE))
                {
                    //Skip ahead
                    currentLocation += (LOGICAL_SECTOR_SIZE -
                            (currentLocation % LOGICAL_SECTOR_SIZE));
                }
                
                //Write the extent and init the record
                writeFileExtent(*currentRecord, imgPath, string(dirEntry->d_name),
                        extentSector, po->directoryTree);
                
                //Seek to the end of the directory and write the record
                imgPath.seekp(currentLocation, ios_base::beg);
                currentRecord->write(imgPath);
                
                //Increment the location by the record size
                currentLocation += currentRecord->dr->length;
                delete currentRecord;
            }
            closedir(direc);
        }
    }
    
    //Add the identifier files if they exist
    BothEndianInt rootSize(rootRecord->dr->size);
    int currentRootSize = rootSize.getValue();
    idFile = string(po->copyrightFileIdentifier,IDENTIFIER_SIZE);
    idSize = writeIdentifier(idFile, imgPath, currentLocation, extentSector);
    currentLocation += idSize;
    currentRootSize += idSize;
    idFile = string(po->abstractFileIdentifier,IDENTIFIER_SIZE);
    idSize = writeIdentifier(idFile, imgPath, currentLocation, extentSector);
    currentLocation += idSize;
    currentRootSize += idSize;
    idFile = string(po->bibliographicFileIdentifier,IDENTIFIER_SIZE);
    idSize = writeIdentifier(idFile, imgPath, currentLocation, extentSector);
    currentLocation += idSize;
    currentRootSize += idSize;
    
    //Adjust the root records size
    rootSize.setValue(currentRootSize);
    memcpy(rootRecord->dr->size, rootSize.getBytes(), sizeof (rootRecord->dr->size));
    
    //Write the root record
    imgPath.seekp(LOGICAL_SECTOR_SIZE * ROOT_RECORD_SECTOR, ios_base::beg);
    rootRecord->write(imgPath);
    
    return extentSector;
}

//Write the file extent
void writeFileExtent(
    DirectoryRecord &rcd,   // The directory record
    ofstream &imgPath,      // Handle to the output image
    string filename,        // The name of the file
    int &extentSector,      // The current farthest sector for extents
    string path             // The path to the file (fstat is picky)
){
    ifstream recordFile;
    const int BUFFER_SIZE = 1000;
    char ver[VER_SUFFIX_SIZE] = VER_SUFFIX ;
    int status, amt, fildes;
    char buffer[BUFFER_SIZE];
    struct stat st;
    char * ptr;
    
    //Get the stats of the file
    path.append(filename);
    fildes = open(path.c_str(), O_RDWR);
    status = fstat(fildes, &st);
    
    //Init the record
    initRecord(rcd, st.st_size, extentSector, FILE_FLAGS,
            filename.length() + VER_SUFFIX_SIZE);
    
    //Fancy pointer math to write the filename into the malloc'ed
    //region outside of the directory record struct
    ptr = (rcd.dr->filename.str) + sizeof(char);
    memcpy(ptr, filename.c_str(), filename.length());
    ptr += (sizeof(char) * filename.length());
    memcpy(ptr, ver, sizeof(ver));
    
    //Seek to the extent and write the file
    imgPath.seekp(extentSector * LOGICAL_SECTOR_SIZE, ios_base::beg);
    recordFile.open (path, ifstream::binary);
    if (recordFile.is_open())
    {
        while ((amt = recordFile.readsome(buffer, BUFFER_SIZE)) > 0)
        {
          imgPath.write(buffer,amt);
        }
        recordFile.close();
    }
    
    //Extend the farthest sector for extents
    extentSector += ceil(st.st_size / LOGICAL_SECTOR_SIZE_D);
}

//Writes and identifier file
int writeIdentifier(
    string file,            // The file
    ofstream &imgPath,      // The image
    int currentLocation,    // The current location
    int &extentSector       // The farthest extent
){
    int trimEnd;
    int size = 0;
    DirectoryRecord * currentRecord;
    if (!(file == BLANK_FILE_IDENTIFIER))
    {
        trimEnd = file.find_last_not_of(" ") + 1;
        file = file.substr(0, trimEnd);
        currentRecord = new DirectoryRecord(file.length()+ VER_SUFFIX_SIZE);
        writeFileExtent(*currentRecord,imgPath,file,extentSector);
        imgPath.seekp(currentLocation, ios_base::beg);
        currentRecord->write(imgPath);
        size = currentRecord->dr->length;
        delete currentRecord;
    }
    return size;
}