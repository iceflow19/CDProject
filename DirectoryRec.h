#include <stdint.h>
#include <fstream>

#ifndef DIRECTORYRECORD_H
#define DIRECTORYRECORD_H

typedef struct sDirectoryRecord {
    uint8_t     length;
    uint8_t     xa_length;
    char        extent[8];
    char        size[8];
    char        recording_time[7];
    uint8_t     flags;
    uint8_t     unit_size;
    uint8_t     gap;
    uint32_t    sequence_number;
    union {
        uint8_t len;
        char str[1];
    } filename;
} __attribute__((packed)) SDirRecord;

class DirectoryRecord {
public:
    SDirRecord * dr;
    DirectoryRecord(int nameSize);
    void write(std::ofstream &image);
};

#endif /* DIRECTORYRECORD_H */

