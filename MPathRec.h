#include <stdint.h>
#include <fstream>
#include "PathRec.h"

#ifndef M_PATH_H
#define M_PATH_H

class MPathRecord {
public:
    SPathRcd * rcd;
    MPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID);
    void write(std::ofstream &image);
};

#endif /* M_PATH_H */

