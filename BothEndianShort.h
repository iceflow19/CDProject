#ifndef __BOTHENDIANSHORT__ 
#define __BOTHENDIANSHORT__
/*
 * Implements the Both endian 32 bit short format for the CD filesystem.
 *
 * Details from ISO 9660 section 7.3.1
 *
 * Potential bug:  If the basic short type is not 2 bytes this code will 
 *                 fail at converting the fourbytes into a valid value.
 */
class BothEndianShort {
protected:
	short value;
public:
	BothEndianShort(char *fourbytes = "\0\0\0\0" );
	short getValue() { return value; }
	char *getBytes();
	void setValue(short value);
};

#endif

