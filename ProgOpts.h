#include <string>

#ifndef PROGOPTS_H
#define PROGOPTS_H

class ProgOpts {
public:
    char        sysID[32];
    char        volID[32];
    int         volSequenceNumber;
    char        volSetID[32];
    std::string volSetIDFile;
    char        publisherID[32];
    std::string publisherIDFile;
    char        dataPreparerID[32];
    std::string dataPreparerIDFile;
    char        appID[32];
    std::string appIDFile;
    char        copyrightFileID[12];
    char        abstractFileID[12];
    char        bibliographicFileID[12];
    char        appUse[512];
    std::string appUseFile;
    std::string dirTree;
    char        expDateAndTime[17];
    
    ProgOpts(char user[32]);
    
    void loadFile(char *field, int size, std::string filename);
};

#endif /* PROGOPTS_H */