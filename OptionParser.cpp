#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "OptionParser.h"

using namespace std;

void handleOptions(ProgramOptions &op, int opt, int *option_index);

enum opts {
    SYSTEM_ID = 'a',
    VOLUME_ID,
    VOLUME_SEQUENCE_NBR,
    VOLUME_SET_IDENTIFIER,
    VOLUME_SET_IDENTIFIER_FILE,
    PUBLISHER_IDENTIFIER,
    PUBLISHER_IDENTIFIER_FILE,
    DATA_PREPARER_IDENTIFIER,
    DATA_PREPARER_IDENTIFIER_FILE,
    APPLICATION_IDENTIFIER,
    APPLICATION_IDENTIFIER_FILE,
    COPYRIGHT_FILE_IDENTIFIER,
    ABSTRACT_FILE_IDENTIFIER,
    BIBLIOGRAPHIC_FILE_IDENTIFIER,
    APPLICATION_USE,
    APPLICATION_USE_FILE,
    DIRECTORY_TREE,
    EXPIRATION_DATE_AND_TIME
};

static struct option longOptions[] = {
    {"system-id", required_argument, 0, SYSTEM_ID},
    {"volume-id", required_argument, 0, VOLUME_ID},
    {"volume-sequence-nbr", required_argument, 0, VOLUME_SEQUENCE_NBR},
    {"volume-set-identifier", required_argument, 0, VOLUME_SET_IDENTIFIER},
    {"volume-set-identifier-file", required_argument, 0, VOLUME_SET_IDENTIFIER_FILE},
    {"publisher-identifier", required_argument, 0, PUBLISHER_IDENTIFIER},
    {"publisher-identifier-file", required_argument, 0, PUBLISHER_IDENTIFIER_FILE},
    {"data-preparer-identifier", required_argument, 0, DATA_PREPARER_IDENTIFIER},
    {"data-preparer-identifier-file", required_argument, 0, DATA_PREPARER_IDENTIFIER_FILE},
    {"application-identifier", required_argument, 0, APPLICATION_IDENTIFIER},
    {"application-identifier-file", required_argument, 0, APPLICATION_IDENTIFIER_FILE},
    {"copyright-file-identifier", required_argument, 0, COPYRIGHT_FILE_IDENTIFIER},
    {"abstract-file-identifier", required_argument, 0, ABSTRACT_FILE_IDENTIFIER},
    {"bibliographic-file-identifier", required_argument, 0, BIBLIOGRAPHIC_FILE_IDENTIFIER},
    {"expiration-date-and-time", required_argument, 0,  EXPIRATION_DATE_AND_TIME},
    {"application-use", required_argument, 0, APPLICATION_USE},
    {"application-use-file", required_argument, 0, APPLICATION_USE_FILE},
    {"directory-tree", required_argument, 0, DIRECTORY_TREE},
    {0, 0, 0, 0}
};

//Parses argv using getopt_long
ProgramOptions * parse(int argc, char** argv) {
    char user[32] = {0};
    ProgramOptions * po;
    int opt;
    
    getlogin_r(user, sizeof(user));
    po = new ProgramOptions(user);
    while (true)
    {
        int optionIndex = 0;
        opt = getopt_long(argc, argv, "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:q:r",
                longOptions, &optionIndex);
        if (opt == -1)
            break;
        handleOptions(*po, opt, &optionIndex);
    }

    return po;
}

//Dispatches proper action depending on the option
void handleOptions(ProgramOptions &po, int opt, int *option_index)
{
    switch (opt) {
        case SYSTEM_ID:
            memset(po.systemID, 0x20, sizeof(po.systemID));
            strncpy(po.systemID, optarg, strlen(optarg));
            break;
        case VOLUME_ID:
            memset(po.volumeID, 0x20, sizeof(po.volumeID));
            strncpy(po.volumeID, optarg, strlen(optarg));
            break;
        case VOLUME_SEQUENCE_NBR:
            po.volumeSequenceNumber = atoi(optarg);
            break;
        case VOLUME_SET_IDENTIFIER:
            memset(po.volumeSetIdentifier, 0x20, sizeof(po.volumeSetIdentifier));
            strncpy(po.volumeSetIdentifier, optarg, strlen(optarg));
            break;
        case VOLUME_SET_IDENTIFIER_FILE:
            po.volumeSetIdentifierFile = optarg;
            po.loadFile(po.volumeSetIdentifier, sizeof(po.volumeSetIdentifier), po.volumeSetIdentifierFile);
            break;
        case PUBLISHER_IDENTIFIER:
            strncpy(po.publisherIdentifier, optarg, strlen(optarg));
            break;
        case PUBLISHER_IDENTIFIER_FILE:
            po.publisherIdentifierFile = optarg;
            po.loadFile(po.publisherIdentifier, sizeof(po.publisherIdentifier), po.publisherIdentifierFile);
            break;
        case DATA_PREPARER_IDENTIFIER:
            strncpy(po.dataPreparerIdentifier, optarg, strlen(optarg));
            break;
        case DATA_PREPARER_IDENTIFIER_FILE:
            po.dataPreparerIdentifierFile = optarg;
            po.loadFile(po.dataPreparerIdentifier, sizeof(po.dataPreparerIdentifier), po.dataPreparerIdentifierFile.c_str());
            break;
        case APPLICATION_IDENTIFIER:
            memset(po.applicationIdentifier, 0x20, sizeof(po.applicationIdentifier));
            strncpy(po.applicationIdentifier, optarg, strlen(optarg));
            break;
        case APPLICATION_IDENTIFIER_FILE:
            po.applicationIdentifierFile = optarg;
            po.loadFile(po.applicationIdentifier, sizeof(po.applicationIdentifier), po.applicationIdentifierFile.c_str());
            break;
        case COPYRIGHT_FILE_IDENTIFIER:
            strncpy(po.copyrightFileIdentifier, optarg, strlen(optarg));
            break;
        case ABSTRACT_FILE_IDENTIFIER:
            strncpy(po.abstractFileIdentifier, optarg, strlen(optarg));
            break;
        case BIBLIOGRAPHIC_FILE_IDENTIFIER:
            strncpy(po.bibliographicFileIdentifier, optarg, strlen(optarg));
            break;
        case APPLICATION_USE:
            strncpy(po.applicationUse, optarg, sizeof(po.applicationUse));
            break;
        case APPLICATION_USE_FILE:
            po.applicationUseFile = optarg;
            po.loadFile(po.applicationUse, sizeof(po.applicationUse), po.applicationUseFile.c_str());
            break;
        case DIRECTORY_TREE:
            po.directoryTree = optarg;
            break;
        case EXPIRATION_DATE_AND_TIME:
            strncpy(po.expirationDateAndTime, optarg, sizeof(po.expirationDateAndTime));
            break;
        default:
            abort();
    }
}