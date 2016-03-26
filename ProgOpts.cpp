#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "ProgOpts.h"

using namespace std;

#define NO_SYS_ID           "NO SYSTEM ID"
#define NO_VOL_ID           "NO VOLUME ID"
#define NO_VOL_SET_ID       "NO-VOLUME-SET-ID"
#define APP_ID              "CDProject"
#define DEF_DATE            "2100010100000000\22"

void ProgOpts::loadFile(char *field, int size, std::string filename)
{
    char * buf = new char[size];
    ifstream file;
    file.open(filename.c_str(), ios_base::in | ios_base::binary);
    if (file.is_open())
    {
        file.read(buf, size);
    }
    file.close();
    strncpy(field, buf, size);
    delete buf;
}

ProgOpts::ProgOpts(char user[32])
{
    memset(sysID, 0x20, sizeof(sysID));
    strncpy(sysID, NO_SYS_ID, sizeof(NO_SYS_ID) - 1);
    memset(volID, 0x20, sizeof(volID));
    strncpy(volID, NO_VOL_ID, sizeof(NO_VOL_ID) - 1);
    memset(dataPreparerID, 0x20, sizeof(dataPreparerID));
    memset(publisherID, 0x20, sizeof(publisherID));
    if(user[0] != '\0')
    {
        strncpy(dataPreparerID, user, sizeof(char) * 32);
        strncpy(publisherID, user, sizeof(char) * 32);
    }
    memset(volSetID, 0x20, sizeof(volSetID));
    strncpy(volSetID, NO_VOL_SET_ID, sizeof(NO_VOL_SET_ID) - 1);
    memset(appID, 0x20, sizeof(appID));
    strncpy(appID, APP_ID, sizeof(APP_ID) -1);
    memset(copyrightFileID, 0x20, sizeof(copyrightFileID));
    memset(abstractFileID, 0x20, sizeof(abstractFileID));
    memset(bibliographicFileID, 0x20, sizeof(bibliographicFileID));
    strncpy(expDateAndTime, DEF_DATE, sizeof(DEF_DATE));
}