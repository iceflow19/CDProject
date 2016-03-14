/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VolumeDescriptor.h
 * Author: cgoodwin
 *
 * Created on March 14, 2016, 3:41 AM
 */

#include "ProgramOptions.h"
#include "DirectoryRecord.h"

#ifndef VOLUMEDESCRIPTOR_H
#define VOLUMEDESCRIPTOR_H

typedef struct VolumeDescriptors {
    uint8_t         type;
    char            id[5];
    uint8_t         version;
    char            unused1[1];
    char            system_id[32];
    char            volume_id[32];
    uint8_t         unused2[8];
    uint64_t        volume_space_size;
    uint8_t         unused3[32];
    uint32_t        volume_set_size;
    uint32_t        volume_sequence_number;
    uint32_t        logical_block_size;
    uint64_t        path_table_size;
    uint32_t        type_l_path_table;
    uint32_t        opt_type_l_path_table;
    uint32_t        type_m_path_table;
    uint32_t        opt_type_m_path_table;
    DirectoryRecordS root_directory_record;
    char            root_directory_filename;
    char            volume_set_id[32];
    char            publisher_id[32];
    char            preparer_id[32];
    char            application_id[32];
    char            copyright_file_id[37];
    char            abstract_file_id[37];
    char            bibliographic_file_id[37];
    char            creation_date[17];
    char            modification_date[17];
    char            expiration_date[17];
    char            effective_date[17];
    uint8_t         file_structure_version;
    uint8_t         unused4[1];
    char            application_data[512];
    uint8_t         unused5[653];
} __attribute__((packed)) VolumeDescriptorS;

class VolumeDescriptor {
public:
    VolumeDescriptorS vd;
    //Normal Constructor
    VolumeDescriptor(ProgramOptions * po);
    //Copy Constructor
    VolumeDescriptor(VolumeDescriptorS &vd);
    void write(std::ofstream &image);
};

#endif /* VOLUMEDESCRIPTOR_H */

