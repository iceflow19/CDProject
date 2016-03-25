#include "ProgramOptions.h"
#include "DirectoryRecord.h"

#ifndef VOLUMEDESCRIPTOR_H
#define VOLUMEDESCRIPTOR_H

typedef struct VolumeDescriptors {
    uint8_t         type;
    char            id[5];
    uint8_t         version;
    char            pad1[1];
    char            systemID[32];
    char            volumeID[32];
    uint8_t         pad2[8];
    uint64_t        volumeSpaceSize;
    uint8_t         pad3[32];
    uint32_t        volumeSetSize;
    uint32_t        volumeSequenceNumber;
    uint32_t        logicalBlockSize;
    uint64_t        pathTableSize;
    uint32_t        typeLPathTable;
    uint32_t        optTypeLPathTable;
    uint32_t        typeMPathTable;
    uint32_t        optTypeMPathTable;
    DirectoryRecordS rootDirectoryRecord;
    char            rootDirectoryFilename;
    char            volumeSetID[128];
    char            publisherID[128];
    char            preparerID[128];
    char            applicationID[128];
    char            copyrightFileID[37];
    char            abstractFileID[37];
    char            bibliographicFileID[37];
    char            creationDate[17];
    char            modificationDate[17];
    char            expirationDate[17];
    char            effectiveDate[17];
    uint8_t         fileStructureVersion;
    uint8_t         pad4[1];
    char            applicationData[512];
    uint8_t         pad5[653];
} __attribute__((packed)) VolumeDescriptorS;

class VolumeDescriptor {
public:
    VolumeDescriptorS vd;
    VolumeDescriptor();
    VolumeDescriptor(ProgramOptions * po, int pathTableSize);
    VolumeDescriptor(VolumeDescriptorS &vd);
    void write(std::ofstream &image);
};

#endif /* VOLUMEDESCRIPTOR_H */

