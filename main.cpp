#include <cstdlib>
#include "OptionParser.h"
#include "VolumeDescriptor.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    ProgramOptions * po = parse(argc, argv);
    
    ofstream imgPath;
    imgPath.open("out.iso", ios_base::out | ios_base::binary);
    if (imgPath.is_open())
    {
        imgPath.seekp(16*2048, ios_base::beg);
        VolumeDescriptor * pvd = new VolumeDescriptor(po);
        pvd->write(imgPath);
    }
    imgPath.close();
    return 0;
}

