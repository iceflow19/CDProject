#include <stdint.h>
#include <fstream>
#include "PathRec.h"

#ifndef L_PATH_H
#define L_PATH_H

class LPathRecord {
public:
    SPathRcd * rcd;
    LPathRecord(int idLength, int extentLocation, int parentDirNum, char * dirID);
    void write(std::ofstream &image);
};

#endif /* L_PATH_H */

