#include <cstring>
#include <fstream>
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "VolumeDescriptor.h"
#include "Utilities.h"
#include "globals.h"

//Constructor for a Terminating Volume Descriptor
VolumeDescriptor::VolumeDescriptor()
{
    vd.type = TVD_TYPE;
    vd.version = VD_VERSION;
    strncpy(vd.id,"CD001",sizeof(vd.id));
}

//Constructor for the PVD, uses a ProgramOptions obj for value initialization
VolumeDescriptor::VolumeDescriptor(ProgramOptions* po, int pathTableSize)
{
    BothEndianShort setSize, sequenceNumber, logicalBlockSize;
    
    //Set values for the BothEndians
    setSize.setValue(1);
    sequenceNumber.setValue(1);
    logicalBlockSize.setValue(LOGICAL_SECTOR_SIZE);
    
    //Set the numerical fields
    vd.type = PVD_TYPE;
    vd.version = VD_VERSION;
    vd.volumeSpaceSize = VOLUME_SPACE_SIZE;
    vd.pathTableSize = pathTableSize;
    vd.typeLPathTable = LPATH_SECTOR;
    vd.optTypeLPathTable = 0;
    vd.optTypeMPathTable = 0;
    vd.fileStructureVersion = FILE_STRUCTURE_VERSION;
    vd.typeMPathTable = MPATH_SECTOR;
    
    //Swap the MPathTable Location to big endian
    swapEndian((char *)&vd.typeMPathTable, sizeof(vd.typeMPathTable));
    
    //Copy the BothEndians into our struct
    memcpy(&vd.volumeSetSize, setSize.getBytes(), sizeof(vd.volumeSetSize));
    memcpy(&vd.volumeSequenceNumber, sequenceNumber.getBytes(), sizeof(vd.volumeSequenceNumber));
    memcpy(&vd.logicalBlockSize, logicalBlockSize.getBytes(), sizeof(vd.logicalBlockSize));
    
    //Memset the text fields
    memset(vd.systemID, 0x20, sizeof(vd.systemID));
    memset(vd.volumeID, 0x20, sizeof(vd.volumeID));
    memset(vd.preparerID, 0x20, sizeof(vd.preparerID));
    memset(vd.publisherID, 0x20, sizeof(vd.publisherID));
    memset(vd.volumeSetID, 0x20, sizeof(vd.volumeSetID));
    memset(vd.applicationID, 0x20, sizeof(vd.copyrightFileID));
    memset(vd.copyrightFileID, 0x20, sizeof(vd.copyrightFileID));
    memset(vd.abstractFileID, 0x20, sizeof(vd.abstractFileID));
    memset(vd.bibliographicFileID, 0x20, sizeof(vd.bibliographicFileID));
    
    //Initialize the remaining char arrays in the struct
    strncpy(vd.id, VD_ID, sizeof(vd.id));
    strncpy(vd.systemID, po->systemID, sizeof(po->systemID));
    strncpy(vd.volumeID, po->volumeID, sizeof(po->volumeID));
    strncpy(vd.preparerID, po->dataPreparerIdentifier, sizeof(po->dataPreparerIdentifier));
    strncpy(vd.publisherID, po->publisherIdentifier, sizeof(po->publisherIdentifier));
    strncpy(vd.volumeSetID, po->volumeSetIdentifier, sizeof(po->volumeSetIdentifier));
    strncpy(vd.applicationID, po->applicationIdentifier, sizeof(po->applicationIdentifier));
    strncpy(vd.copyrightFileID, po->copyrightFileIdentifier, sizeof(po->copyrightFileIdentifier));
    strncpy(vd.abstractFileID, po->abstractFileIdentifier, sizeof(po->abstractFileIdentifier));
    strncpy(vd.bibliographicFileID, po->bibliographicFileIdentifier, sizeof(po->bibliographicFileIdentifier));
    strncpy(vd.creationDate, getASCIIDateTime(), sizeof(vd.creationDate));
    strncpy(vd.modificationDate, getASCIIDateTime(), sizeof(vd.modificationDate));
    strncpy(vd.effectiveDate, getASCIIDateTime(), sizeof(vd.effectiveDate));
    strncpy(vd.expirationDate, po->expirationDateAndTime, sizeof(vd.expirationDate));
}

//Copy Constructor
VolumeDescriptor::VolumeDescriptor(VolumeDescriptorS &vd)
{
    this->vd = vd;
}

//Serializes the backing struct to the file
void VolumeDescriptor::write(std::ofstream &image)
{
    image.write((char *)&this->vd, sizeof(VolumeDescriptorS));
}