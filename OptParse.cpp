#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "OptParse.h"

using namespace std;

void handleOptions(ProgOpts &op, int opt, int *option_index);

static struct option longOptions[] = {
    {"system-id", required_argument, 0, 'a'},
    {"volume-id", required_argument, 0, 'b'},
    {"volume-sequence-nbr", required_argument, 0, 'c'},
    {"volume-set-identifier", required_argument, 0, 'd'},
    {"volume-set-identifier-file", required_argument, 0, 'e'},
    {"publisher-identifier", required_argument, 0, 'f'},
    {"publisher-identifier-file", required_argument, 0, 'g'},
    {"data-preparer-identifier", required_argument, 0, 'h'},
    {"data-preparer-identifier-file", required_argument, 0, 'i'},
    {"application-identifier", required_argument, 0, 'j'},
    {"application-identifier-file", required_argument, 0, 'k'},
    {"copyright-file-identifier", required_argument, 0, 'l'},
    {"abstract-file-identifier", required_argument, 0, 'm'},
    {"bibliographic-file-identifier", required_argument, 0, 'n'},
    {"expiration-date-and-time", required_argument, 0,  'o'},
    {"application-use", required_argument, 0, 'p'},
    {"application-use-file", required_argument, 0, 'q'},
    {"directory-tree", required_argument, 0, 'r'},
    {0, 0, 0, 0}
};

ProgOpts * parse(int argc, char** argv) {
    char user[32] = {0};
    ProgOpts * po;
    int opt;
    getlogin_r(user, sizeof(user));
    po = new ProgOpts(user);
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

void handleOptions(ProgOpts &po, int opt, int *option_index)
{
    switch (opt) {
        case 'a':
            memset(po.sysID, 0x20, sizeof(po.sysID));
            strncpy(po.sysID, optarg, strlen(optarg));
            break;
        case 'b':
            memset(po.volID, 0x20, sizeof(po.volID));
            strncpy(po.volID, optarg, strlen(optarg));
            break;
        case 'c':
            po.volSequenceNumber = atoi(optarg);
            break;
        case 'd':
            memset(po.volSetID, 0x20, sizeof(po.volSetID));
            strncpy(po.volSetID, optarg, strlen(optarg));
            break;
        case 'e':
            po.volSetIDFile = optarg;
            po.loadFile(po.volSetID, sizeof(po.volSetID), po.volSetIDFile);
            break;
        case 'f':
            strncpy(po.publisherID, optarg, strlen(optarg));
            break;
        case 'g':
            po.publisherIDFile = optarg;
            po.loadFile(po.publisherID, sizeof(po.publisherID), po.publisherIDFile);
            break;
        case 'h':
            strncpy(po.dataPreparerID, optarg, strlen(optarg));
            break;
        case 'i':
            po.dataPreparerIDFile = optarg;
            po.loadFile(po.dataPreparerID, sizeof(po.dataPreparerID), po.dataPreparerIDFile.c_str());
            break;
        case 'j':
            memset(po.appID, 0x20, sizeof(po.appID));
            strncpy(po.appID, optarg, strlen(optarg));
            break;
        case 'k':
            po.appIDFile = optarg;
            po.loadFile(po.appID, sizeof(po.appID), po.appIDFile.c_str());
            break;
        case 'l':
            strncpy(po.copyrightFileID, optarg, strlen(optarg));
            break;
        case 'm':
            strncpy(po.abstractFileID, optarg, strlen(optarg));
            break;
        case 'n':
            strncpy(po.bibliographicFileID, optarg, strlen(optarg));
            break;
        case 'p':
            strncpy(po.appUse, optarg, sizeof(po.appUse));
            break;
        case 'q':
            po.appUseFile = optarg;
            po.loadFile(po.appUse, sizeof(po.appUse), po.appUseFile.c_str());
            break;
        case 'r':
            po.dirTree = optarg;
            break;
        case 'o':
            strncpy(po.expDateAndTime, optarg, sizeof(po.expDateAndTime));
            break;
        default:
            abort();
    }
}