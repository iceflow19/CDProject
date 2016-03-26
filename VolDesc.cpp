#include <cstring>
#include <fstream>
#include "BothEndianInt.h"
#include "BothEndianShort.h"
#include "VolDesc.h"

char * getASCIIDT();
void swapper(char * buf, int length);

VolumeDescriptor::VolumeDescriptor()
{
    vd.type = 255;
    vd.version = 1;
    strncpy(vd.id,"CD001",sizeof(vd.id));
}

VolumeDescriptor::VolumeDescriptor(ProgOpts* po, int pathTableSize)
{
    BothEndianShort setSize, sequenceNumber, logicalBlockSize;
    strncpy(vd.id,"CD001",sizeof(vd.id));
    memset(vd.system_id, 0x20, sizeof(vd.system_id));
    memset(vd.volume_id, 0x20, sizeof(vd.volume_id));
    memset(vd.preparer_id, 0x20, sizeof(vd.preparer_id));
    memset(vd.publisher_id, 0x20, sizeof(vd.publisher_id));
    memset(vd.volume_set_id, 0x20, sizeof(vd.volume_set_id));
    memset(vd.application_id, 0x20, sizeof(vd.copyright_file_id));
    memset(vd.copyright_file_id, 0x20, sizeof(vd.copyright_file_id));
    memset(vd.abstract_file_id, 0x20, sizeof(vd.abstract_file_id));
    memset(vd.bibliographic_file_id, 0x20, sizeof(vd.bibliographic_file_id));
    vd.type = 1;
    vd.version = 1;
    vd.space_size = (uint64_t)0xAE000000000000AE;
    vd.path_table_size = pathTableSize;
    vd.type_lpath_table = 18;
    vd.opt_type_lpath_table = 0;
    vd.type_mpath_table = 19;
    swapper((char *)&vd.type_mpath_table, sizeof(vd.type_mpath_table));
    vd.opt_type_mpath_table = 0;
    vd.file_struct_ver = 1;
    setSize.setValue(1);
    memcpy(&vd.set_size, setSize.getBytes(), sizeof(vd.set_size));
    sequenceNumber.setValue(1);
    memcpy(&vd.seq_num, sequenceNumber.getBytes(), sizeof(vd.seq_num));
    logicalBlockSize.setValue(2048);
    memcpy(&vd.block_size, logicalBlockSize.getBytes(), sizeof(vd.block_size));
    strncpy(vd.system_id, po->sysID, sizeof(po->sysID));
    strncpy(vd.volume_id, po->volID, sizeof(po->volID));
    strncpy(vd.preparer_id, po->dataPreparerID, sizeof(po->dataPreparerID));
    strncpy(vd.publisher_id, po->publisherID, sizeof(po->publisherID));
    strncpy(vd.volume_set_id, po->volSetID, sizeof(po->volSetID));
    strncpy(vd.application_id, po->appID, sizeof(po->appID));
    strncpy(vd.copyright_file_id, po->copyrightFileID, sizeof(po->copyrightFileID));
    strncpy(vd.abstract_file_id, po->abstractFileID, sizeof(po->abstractFileID));
    strncpy(vd.bibliographic_file_id, po->bibliographicFileID, sizeof(po->bibliographicFileID));
    strncpy(vd.creation_date, getASCIIDT(), sizeof(vd.creation_date));
    strncpy(vd.modification_date, getASCIIDT(), sizeof(vd.modification_date));
    strncpy(vd.effective_date, getASCIIDT(), sizeof(vd.effective_date));
    strncpy(vd.expiration_date, po->expDateAndTime, sizeof(vd.expiration_date));
}

void VolumeDescriptor::write(std::ofstream &image)
{
    image.write((char *)&this->vd, sizeof(SVolDesc));
}

void swapper(char * buf, int length)
{
    char * a;
    char * b;
    a = buf;
    b = buf + ((length - 1) * sizeof(char));
    for (int len = length / 2; len > 0; --len)
    {
        char temp;
        temp = *a;
        *a = *b;
        *b = temp;
        ++a;
        --b;
    }
}

char * getASCIIDT()
{    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char * asciiTime = new char [17];
    sprintf(&asciiTime[0], "%04d", tm.tm_year);
    sprintf(&asciiTime[4], "%02d", tm.tm_mon);
    sprintf(&asciiTime[6], "%02d", tm.tm_mday + 1);
    sprintf(&asciiTime[8], "%02d", tm.tm_hour);
    sprintf(&asciiTime[10], "%02d", tm.tm_min);
    sprintf(&asciiTime[12], "%02d", tm.tm_sec);
    sprintf(&asciiTime[14], "%02d", 0);
    asciiTime[16] = 44;
    
    return asciiTime;
}