#include <stdint.h>
#include <fstream>

#ifndef PATH_RECORD_H
#define PATH_RECORD_H

typedef struct PathRecords {
    uint8_t     length;
    uint8_t     xa_length;
    uint64_t    extent;
    uint64_t    parentDirectoryNumber;
    union {
        uint8_t len;
        char str[1];
    } directoryID;
} __attribute__((packed)) PathRecordS;

#endif /* PATH_RECORD_H */