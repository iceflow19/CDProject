#include "OptionParser.h"
#include <cstring>

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

static struct option long_options[] = {
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

ProgramOptions * parse(int argc, char** argv) {
    char user[32] = "troyko"; //TODO
    ProgramOptions * po = new ProgramOptions();

    int opt;
    while (true)
    {
        int option_index = 0;
        opt = getopt_long(argc, argv, "a:b:c:d:e:f:g:h:i:j:k:l:m:n:o:p:q:r",
                long_options, &option_index);

        if (opt == -1)
            break;
        handleOptions(*po, opt, &option_index);
    }

    return po;
}

void handleOptions(ProgramOptions &po, int opt, int *option_index)
{
    switch (opt) {
        case SYSTEM_ID:
            strncpy(po.systemID, optarg, sizeof(po.systemID));
            break;
        case VOLUME_ID:
            strncpy(po.volumeID, optarg, sizeof(po.volumeID));
            break;
        case VOLUME_SEQUENCE_NBR:
            //strncpy(po.volumeSequenceNumber, optarg, sizeof(po.volumeSequenceNumber));
            break;
        case VOLUME_SET_IDENTIFIER:
            strncpy(po.volumeSetIdentifier, optarg, sizeof(po.volumeSetIdentifier));
            break;
        case VOLUME_SET_IDENTIFIER_FILE:
            po.volumeSetIdentifierFile = optarg;
            po.loadFile(po.volumeSetIdentifier, 0, po.volumeSetIdentifierFile);
            break;
        case PUBLISHER_IDENTIFIER:
            strncpy(po.publisherIdentifier, optarg, sizeof(po.publisherIdentifier));
            break;
        case PUBLISHER_IDENTIFIER_FILE:
            po.publisherIdentifierFile = optarg;
            po.loadFile(po.publisherIdentifier, 0, po.publisherIdentifierFile);
            break;
        case DATA_PREPARER_IDENTIFIER:
            po.dataPreparerIdentifier = optarg;
            break;
        case DATA_PREPARER_IDENTIFIER_FILE:
            po.dataPreparerIdentifierFile = optarg;
            po.loadFile(po.applicationUse, 0, po.applicationUseFile.c_str());
            break;
        case APPLICATION_IDENTIFIER:
            po.applicationIdentifier = optarg;
            break;
        case APPLICATION_IDENTIFIER_FILE:
            po.applicationIdentifierFile = optarg;
            po.loadFile(po.applicationUse, 0, po.applicationUseFile.c_str());
            break;
        case COPYRIGHT_FILE_IDENTIFIER:
            po.copyrightFileIdentifier = optarg;
            break;
        case ABSTRACT_FILE_IDENTIFIER:
            po.abstractFileIdentifier = optarg;
            break;
        case BIBLIOGRAPHIC_FILE_IDENTIFIER:
            po.bibliographicFileIdentifier = optarg;
            break;
        case APPLICATION_USE:
            po.applicationUse = optarg;
            break;
        case APPLICATION_USE_FILE:
            po.applicationUseFile = optarg;
            po.loadFile(po.applicationUse, 0, po.applicationUseFile.c_str());
            break;
        case DIRECTORY_TREE:
            po.directoryTree = optarg;
            break;
        case EXPIRATION_DATE_AND_TIME:
            po.expirationDateAndTime = optarg;
            break;
        default:
            abort();
    }
}