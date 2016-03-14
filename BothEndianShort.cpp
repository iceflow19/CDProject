#include "BothEndianShort.h"
#include <iostream>
using namespace std;

BothEndianShort::BothEndianShort(char *fourbytes) {
	unsigned short *ptr;
	char testOrder[] = { 0xaa, 0xbb};

	ptr = (unsigned short *) &testOrder[0];
	if ( (unsigned short)((*ptr) & 0xff) == (unsigned int) 0xaa ) {
		value = *((short *)&(fourbytes[0]));	
	} else if ( (unsigned short)((*ptr) & 0xff) == (unsigned int) 0xbb ) {
		value = *((short *)&(fourbytes[4]));
	}
}

char *BothEndianShort::getBytes() {
	char * result;

	result = new char[4];

	result[0] = result[3] = value & 0xff; // lsb
	result[1] = result[2] = (value >> 8) & 0xff; //  msb
	return result;
}

void BothEndianShort::setValue(short value) {
	this->value = value;
}