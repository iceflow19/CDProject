#include "ProgOpts.h"
#include "DirectoryRec.h"

#ifndef VOLUMEDESCRIPTOR_H
#define VOLUMEDESCRIPTOR_H

typedef struct sVolDesc {
    uint8_t         type;
    char            id[5];
    uint8_t         version;
    char            pading1[1];
    char            system_id[32];
    char            volume_id[32];
    uint8_t         pading2[8];
    uint64_t        space_size;
    uint8_t         padding3[32];
    uint32_t        set_size;
    uint32_t        seq_num;
    uint32_t        block_size;
    uint64_t        path_table_size;
    uint32_t        type_lpath_table;
    uint32_t        opt_type_lpath_table;
    uint32_t        type_mpath_table;
    uint32_t        opt_type_mpath_table;
    SDirRecord      rootDirRecord;
    char            rootDirFilename;
    char            volume_set_id[128];
    char            publisher_id[128];
    char            preparer_id[128];
    char            application_id[128];
    char            copyright_file_id[37];
    char            abstract_file_id[37];
    char            bibliographic_file_id[37];
    char            creation_date[17];
    char            modification_date[17];
    char            expiration_date[17];
    char            effective_date[17];
    uint8_t         file_struct_ver;
    uint8_t         padding4[1];
    char            app_data[512];
    uint8_t         padding5[653];
} __attribute__((packed)) SVolDesc;

class VolumeDescriptor {
public:
    SVolDesc vd;
    VolumeDescriptor();
    VolumeDescriptor(ProgOpts * po, int ptableSize);
    void write(std::ofstream &image);
};

#endif /* VOLUMEDESCRIPTOR_H */

