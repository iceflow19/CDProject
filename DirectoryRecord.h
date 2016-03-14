#include <stdint.h>
#include <fstream>

#ifndef DIRECTORYRECORD_H
#define DIRECTORYRECORD_H

typedef struct DirectoryRecords {
    uint8_t     length;
    uint8_t     xa_length;
    uint64_t    extent;
    uint64_t    size;
    char        recording_time[7];
    uint8_t     file_flags;
    uint8_t     file_unit_size;
    uint8_t     interleave_gap;
    uint32_t    volume_sequence_number;
    union {
        uint8_t len;
        char str[1];
    } filename;
} __attribute__((packed)) DirectoryRecordS;

class DirectoryRecord {
public:
    DirectoryRecordS * dr;
    //Normal Constructor
    DirectoryRecord(int fileNameSize);
    //Copy Constructor
    DirectoryRecord(DirectoryRecordS &dr);
    void write(std::ofstream &image);
};

#endif /* DIRECTORYRECORD_H */

