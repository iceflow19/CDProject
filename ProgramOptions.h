#include <string>

#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

class ProgramOptions {
public:
    char        systemID[32];
    char        volumeID[32];
    int         volumeSequenceNumber;
    char        volumeSetIdentifier[32];
    std::string volumeSetIdentifierFile;
    char        publisherIdentifier[32];
    std::string publisherIdentifierFile;
    char        dataPreparerIdentifier[32];
    std::string dataPreparerIdentifierFile;
    char        applicationIdentifier[32];
    std::string applicationIdentifierFile;
    char        copyrightFileIdentifier[12];
    char        abstractFileIdentifier[12];
    char        bibliographicFileIdentifier[12];
    char        applicationUse[512];
    std::string applicationUseFile;
    std::string directoryTree;
    char        expirationDateAndTime[17];
    
    ProgramOptions(char user[32]);
    
    void loadFile(char *field, int size, std::string filename);
};

#endif /* PROGRAMOPTIONS_H */