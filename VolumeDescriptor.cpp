#include <cstring>
#include <fstream>
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "VolumeDescriptor.h"
#include "Utilities.h"
#include "globals.h"

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
    
    //Initialize the remaining char arrays in the struct
    strncpy(vd.id, VD_ID, sizeof(vd.id));
    strncpy(vd.systemID, po->systemID, sizeof(vd.systemID));
    strncpy(vd.volumeID, po->volumeID, sizeof(vd.volumeID));
    strncpy(vd.preparerID, po->dataPreparerIdentifier, sizeof(vd.preparerID));
    strncpy(vd.publisherID, po->publisherIdentifier, sizeof(vd.publisherID));
    strncpy(vd.volumeSetID, po->volumeSetIdentifier, sizeof(vd.volumeSetID));
    strncpy(vd.applicationID, po->applicationIdentifier, sizeof(vd.copyrightFileID));
    strncpy(vd.copyrightFileID, po->copyrightFileIdentifier, sizeof(vd.copyrightFileID));
    strncpy(vd.abstractFileID, po->abstractFileIdentifier, sizeof(vd.abstractFileID));
    strncpy(vd.bibliographicFileID, po->bibliographicFileIdentifier, sizeof(vd.bibliographicFileID));
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