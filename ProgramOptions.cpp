#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "ProgramOptions.h"
#include "globals.h"

using namespace std;

//Loads a file and sets an objects field to the content
void ProgramOptions::loadFile(char *field, int size, std::string filename)
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

//Initialize members to their default values
ProgramOptions::ProgramOptions(char user[32])
{
    //Memset the fields to spaces
    memset(systemID, 0x20, sizeof(systemID));
    memset(volumeID, 0x20, sizeof(volumeID));
    memset(dataPreparerIdentifier, 0x20, sizeof(dataPreparerIdentifier));
    memset(publisherIdentifier, 0x20, sizeof(publisherIdentifier));
    memset(volumeSetIdentifier, 0x20, sizeof(volumeSetIdentifier));
    memset(applicationIdentifier, 0x20, sizeof(applicationIdentifier));
    memset(copyrightFileIdentifier, 0x20, sizeof(copyrightFileIdentifier));
    memset(abstractFileIdentifier, 0x20, sizeof(abstractFileIdentifier));
    memset(bibliographicFileIdentifier, 0x20, sizeof(bibliographicFileIdentifier));
    
    strncpy(systemID, NO_SYSTEM_ID, sizeof(NO_SYSTEM_ID) - 1);
    strncpy(volumeID, NO_VOLUME_ID, sizeof(NO_VOLUME_ID) - 1);
    if(user[0] != '\0')
    {
        strncpy(dataPreparerIdentifier, user, sizeof(char) * 32);
        strncpy(publisherIdentifier, user, sizeof(char) * 32);
    }
    strncpy(volumeSetIdentifier, NO_VOLUME_SET_ID, sizeof(NO_VOLUME_SET_ID) - 1);
    strncpy(applicationIdentifier, APP_IDENTIFIER, sizeof(APP_IDENTIFIER) -1);
    strncpy(expirationDateAndTime, DEFAULT_DATE, sizeof(DEFAULT_DATE));
}