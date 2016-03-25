#include <stdint.h>
#include <fstream>

#ifndef DIRECTORYRECORD_H
#define DIRECTORYRECORD_H

typedef struct DirectoryRecords {
    uint8_t     length;
    uint8_t     xaLength;
    char        extent[8];
    char        size[8];
    char        recordingTime[7];
    uint8_t     fileFlags;
    uint8_t     fileUnitSize;
    uint8_t     interleaveGap;
    uint32_t    volumeSequenceNumber;
    union {
        uint8_t len;
        char str[1];
    } filename;
} __attribute__((packed)) DirectoryRecordS;

class DirectoryRecord {
public:
    DirectoryRecordS * dr;
    DirectoryRecord(int fileNameSize);
    DirectoryRecord(DirectoryRecordS &dr);
    void write(std::ofstream &image);
};

#endif /* DIRECTORYRECORD_H */

