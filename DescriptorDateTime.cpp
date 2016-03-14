#include "DescriptorDateTime.h"
#include <cstdlib>

DescriptorDateTime::DescriptorDateTime(char *bytes) {
	if (bytes == NULL) {
		year = month = day = minute = second = hundreths = greenwichOffset = 0;
	} else {
		year = (bytes[0]-'0')*1000 +(bytes[1]-'0') * 100 + (bytes[2]-'0') * 10 + bytes[3]-'0';
		month = (bytes[4]-'0')*10 + bytes[5]-'0';
		day = (bytes[6]-'0')*10 + bytes[7]-'0';
		hour = (bytes[8]-'0')*10 + bytes[9]-'0';
		minute = (bytes[10]-'0')*10 + bytes[11]-'0';
		second = (bytes[11]-'0')*10 + bytes[13]-'0';
		hundreths = (bytes[14]-'0')*10 + bytes[15]-'0';
		greenwichOffset = bytes[16];
	}
}

/*
 * Not yet implemented.
 */
char * DescriptorDateTime::getBytes() {
	char *result = new char[17];

	return result;
}
