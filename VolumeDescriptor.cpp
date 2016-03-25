/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "VolumeDescriptor.h"
#include <cstring>
#include <fstream>
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "Utilities.h"

VolumeDescriptor::VolumeDescriptor()
{
    //VD
    vd.type = 255;
    strncpy(vd.id,"CD001",5);
    vd.version = 1;
}

VolumeDescriptor::VolumeDescriptor(ProgramOptions* po, int pathTableSize)
{
    //VD
    vd.type = 1;
    strncpy(vd.id,"CD001",5);
    vd.version = 1;
    vd.volume_space_size = (uint64_t)0xAE000000000000AE;
    BothEndianShort setSize;
    setSize.setValue(1);
    memcpy(&vd.volume_set_size, setSize.getBytes(), sizeof(vd.volume_set_size));
    BothEndianShort sequenceNumber;
    sequenceNumber.setValue(1);
    memcpy(&vd.volume_sequence_number, sequenceNumber.getBytes(), sizeof(vd.volume_sequence_number));
    BothEndianShort logicalBlockSize;
    logicalBlockSize.setValue(2048);
    memcpy(&vd.logical_block_size, logicalBlockSize.getBytes(), sizeof(vd.logical_block_size));
    vd.path_table_size = pathTableSize;
    vd.type_l_path_table = 18;
    vd.opt_type_l_path_table = 0;
    uint32_t mPathTable = 19;
    swapEndian((char *)&mPathTable, sizeof(uint32_t));
    vd.type_m_path_table = mPathTable;
    vd.opt_type_m_path_table = 0;
    vd.file_structure_version = 1;
       
    strncpy(vd.system_id, po->systemID, sizeof(vd.system_id));
    strncpy(vd.volume_id, po->volumeID, sizeof(vd.volume_id));
    strncpy(vd.preparer_id, po->dataPreparerIdentifier, sizeof(vd.preparer_id));
    strncpy(vd.publisher_id, po->publisherIdentifier, sizeof(vd.publisher_id));
    strncpy(vd.volume_set_id, po->volumeSetIdentifier, sizeof(vd.volume_set_id));
    strncpy(vd.application_id, po->applicationIdentifier, sizeof(vd.copyright_file_id));
    strncpy(vd.copyright_file_id, po->copyrightFileIdentifier, sizeof(vd.copyright_file_id));
    strncpy(vd.abstract_file_id, po->abstractFileIdentifier, sizeof(vd.abstract_file_id));
    strncpy(vd.bibliographic_file_id, po->bibliographicFileIdentifier, sizeof(vd.bibliographic_file_id));
    strncpy(vd.creation_date, getASCIIDateTime(), sizeof(vd.creation_date));
    strncpy(vd.modification_date, getASCIIDateTime(), sizeof(vd.modification_date));
    strncpy(vd.effective_date, getASCIIDateTime(), sizeof(vd.effective_date));
    strncpy(vd.expiration_date, po->expirationDateAndTime, sizeof(vd.expiration_date));

}

VolumeDescriptor::VolumeDescriptor(VolumeDescriptorS &vd)
{
    this->vd = vd;
}

void VolumeDescriptor::write(std::ofstream &image)
{
    image.write((char *) &this->vd, sizeof(VolumeDescriptorS));
}