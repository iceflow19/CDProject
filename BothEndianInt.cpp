#include "BothEndianInt.h"
#include <iostream>
using namespace std;

BothEndianInt::BothEndianInt(char *eightbytes) {
	unsigned int *ptr;
	char testOrder[] = { 0xaa, 0xbb, 0xcc, 0xdd };

	ptr = (unsigned int *) &testOrder[0];
	if ( (unsigned int)((*ptr) & 0xff) == (unsigned int) 0xaa ) {
		value = *((int *)&(eightbytes[0]));	
	} else if ( (unsigned int)((*ptr) & 0xff) == (unsigned int) 0xdd ) {
		value = *((int *)&(eightbytes[4]));
	}
}

char *BothEndianInt::getBytes() {
	char * result;

	result = new char[8];

	result[0] = result[7] = value & 0xff; // lsb
	result[1] = result[6] = (value >> 8) & 0xff; 
	result[2] = result[5] = (value >> 16) & 0xff;
	result[3] = result[4] = (value >> 24) & 0xff; // msb

	return result;
}

void BothEndianInt::setValue(int value) {
	this->value = value;
}