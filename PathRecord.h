#include <stdint.h>
#include <fstream>

#ifndef PATH_RECORD_H
#define PATH_RECORD_H

typedef struct PathRecords {
    uint8_t     length;
    uint8_t     xa_length;
    uint32_t    extent;
    uint16_t    parentDirectoryNumber;
    char        directoryID[1];
} __attribute__((packed)) PathRecordS;

#endif /* PATH_RECORD_H */