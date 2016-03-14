#ifndef __BOTHENDIANINT__ 
#define __BOTHENDIANINT__
/*
 * Implements the Both endian 32 bit integer format for the CD filesystem.
 *
 * Details from ISO 9660 section 7.3.3
 *
 * Potential bug:  If the basic int type is not 4 bytes this code will 
 *                 fail at converting the eightbytes into a valid value.
 */
class BothEndianInt {
protected:
	int value;
public:
	BothEndianInt(char *eightbytes = "\0\0\0\0\0\0\0\0" );
	int getValue() { return value; }
	char *getBytes();
	void setValue(int value);
};

#endif