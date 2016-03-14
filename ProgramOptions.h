/* 
 * File:   ProgramOptions.h
 * Author: Thomas Royko
 *
 * Created on February 28, 2016, 10:09 PM
 */

#include <string>

#ifndef PROGRAMOPTIONS_H
#define PROGRAMOPTIONS_H

class ProgramOptions {
public:
    char        systemID[32]                    = {0};
    char        volumeID[32]                    = {0};
    int         volumeSequenceNumber            = 1;
    char        volumeSetIdentifier[32]         = {0};
    std::string volumeSetIdentifierFile;
    char        publisherIdentifier[32]         = {0};
    std::string publisherIdentifierFile;
    char        dataPreparerIdentifier[32]      = {0};
    std::string dataPreparerIdentifierFile;
    char        applicationIdentifier[32]       = {0};
    std::string applicationIdentifierFile;
    char        copyrightFileIdentifier[12]     = {0};
    char        abstractFileIdentifier[12]      = {0};
    char        bibliographicFileIdentifier[12] = {0};
    char        applicationUse[512]             = {0};
    std::string applicationUseFile;
    std::string directoryTree;
    char        expirationDateAndTime[17]       = {0};
    
    ProgramOptions(char user[32]);
    
    void loadFile(char *field, int size, std::string filename);
};

#endif /* PROGRAMOPTIONS_H */