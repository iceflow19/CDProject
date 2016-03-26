#include <stdint.h>
#include <fstream>

#ifndef PATH_RECORD_H
#define PATH_RECORD_H

typedef struct sPathRcd {
    uint8_t     length;
    uint8_t     xa_length;
    uint32_t    extent;
    uint16_t    parent_dir_num;
    char        directory_id[1];
} __attribute__((packed)) SPathRcd;

#endif /* PATH_RECORD_H */