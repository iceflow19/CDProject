#include <stdint.h>
#include <fstream>
#include "PathRecord.h"

#ifndef L_PATH_H
#define L_PATH_H

class LPathRecord {
public:
    PathRecordS * rcd;
    //Normal Constructor
    LPathRecord(int idLength);
    //Copy Constructor
    LPathRecord(PathRecordS &rcd);
    void write(std::ofstream &image);
};

#endif /* L_PATH_H */

