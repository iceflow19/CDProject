/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "VolumeDescriptor.h"
#include <cstring>
#include <fstream>

VolumeDescriptor::VolumeDescriptor(ProgramOptions* po)
{
    //VD
    strncpy(vd.system_id, po->systemID, sizeof(vd.system_id));
    strncpy(vd.volume_id, po->volumeID, sizeof(vd.volume_id));
    strncpy(vd.preparer_id, po->dataPreparerIdentifier, sizeof(vd.preparer_id));
    strncpy(vd.publisher_id, po->publisherIdentifier, sizeof(vd.publisher_id));
    strncpy(vd.volume_set_id, po->volumeSetIdentifier, sizeof(vd.volume_set_id));
    strncpy(vd.application_id, po->applicationIdentifier, sizeof(vd.copyright_file_id));
    strncpy(vd.copyright_file_id, po->copyrightFileIdentifier, sizeof(vd.copyright_file_id));
    strncpy(vd.abstract_file_id, po->abstractFileIdentifier, sizeof(vd.abstract_file_id));
    strncpy(vd.bibliographic_file_id, po->bibliographicFileIdentifier, sizeof(vd.bibliographic_file_id));
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