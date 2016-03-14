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
    char        systemID[32]                    = "NO SYSTEM ID";
    char        volumeID[32]                    = "NO VOLUME ID";
    int         volumeSequenceNumber            = 1;
    char        volumeSetIdentifier[32]         = "NO-VOLUME-SET-ID";
    std::string volumeSetIdentifierFile         = "";
    char        publisherIdentifier[32];
    std::string publisherIdentifierFile         = "";
    char        dataPreparerIdentifier[32];
    std::string dataPreparerIdentifierFile;
    char        applicationIdentifier[32]       = "CDProject";
    std::string applicationIdentifierFile       = "";
    char        copyrightFileIdentifier[13]     = "            ";
    char        abstractFileIdentifier[13]      = "            ";
    char        bibliographicFileIdentifier[13] = "            ";
    std::string applicationUse                  = "";
    std::string applicationUseFile              = "";
    std::string directoryTree                   = "";
    char        expirationDateAndTime[18]       = "2100010100000000\22";
    
    ProgramOptions();
    
    void loadFile(char *field, int size, std::string filename);
};

#endif /* PROGRAMOPTIONS_H */