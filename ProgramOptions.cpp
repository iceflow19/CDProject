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
    strncpy(systemID, NO_SYSTEM_ID, sizeof(NO_SYSTEM_ID));
    strncpy(volumeID, NO_VOLUME_ID, sizeof(NO_VOLUME_ID));
    strncpy(dataPreparerIdentifier, user, sizeof(char) * 32);
    strncpy(publisherIdentifier, user, sizeof(char) * 32);
    strncpy(volumeSetIdentifier, NO_VOLUME_SET_ID, sizeof(NO_VOLUME_SET_ID));
    strncpy(applicationIdentifier, APP_IDENTIFIER, sizeof(APP_IDENTIFIER));
    strncpy(copyrightFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(abstractFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(bibliographicFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(expirationDateAndTime, DEFAULT_DATE, sizeof(DEFAULT_DATE));
}