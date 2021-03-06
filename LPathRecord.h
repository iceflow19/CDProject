#include <stdint.h>
#include <fstream>
#include "PathRecord.h"

#ifndef L_PATH_H
#define L_PATH_H

class LPathRecord {
public:
    PathRecordS * rcd;
    LPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID);
    LPathRecord(PathRecordS &rcd);
    void write(std::ofstream &image);
};

#endif /* L_PATH_H */

