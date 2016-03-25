#include <stdint.h>
#include <fstream>
#include "PathRecord.h"

#ifndef M_PATH_H
#define M_PATH_H

class MPathRecord {
public:
    PathRecordS * rcd;
    //Normal Constructor
    MPathRecord(int idLength, int extentLocation, int pdn, char * id);
    //Copy Constructor
    MPathRecord(PathRecordS &rcd);
    void write(std::ofstream &image);
};

#endif /* M_PATH_H */

