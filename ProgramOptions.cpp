#include "ProgramOptions.h"
#include <cstring>
#define BLANK_FILE_IDENTIFIER   "            "
#define NO_SYSTEM_ID            "NO SYSTEM ID"
#define NO_VOLUME_ID            "NO VOLUME ID"
#define NO_VOLUME_SET_ID        "NO-VOLUME-SET-ID"
#define APP_IDENTIFIER          "CDProject"
#define DEFAULT_DATE            "2100010100000000\22"

using namespace std;

void ProgramOptions::loadFile(char *field, int size, std::string filename)
{
    
}

ProgramOptions::ProgramOptions()
{
    strncpy(systemID, NO_SYSTEM_ID, sizeof(NO_SYSTEM_ID));
    strncpy(volumeID, NO_VOLUME_ID, sizeof(NO_VOLUME_ID));
    strncpy(volumeSetIdentifier, NO_VOLUME_SET_ID, sizeof(NO_VOLUME_SET_ID));
    strncpy(applicationIdentifier, APP_IDENTIFIER, sizeof(APP_IDENTIFIER));
    strncpy(copyrightFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(abstractFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(bibliographicFileIdentifier, BLANK_FILE_IDENTIFIER, sizeof(BLANK_FILE_IDENTIFIER));
    strncpy(expirationDateAndTime, DEFAULT_DATE, sizeof(DEFAULT_DATE));
}