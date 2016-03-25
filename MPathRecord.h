#include <stdint.h>
#include <fstream>
#include "PathRecord.h"

#ifndef M_PATH_H
#define M_PATH_H

class MPathRecord {
public:
    PathRecordS * rcd;
    MPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID);
    MPathRecord(PathRecordS &rcd);
    void write(std::ofstream &image);
};

#endif /* M_PATH_H */

