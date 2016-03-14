#ifndef __DESCRIPTORDATETIME_H__
#define __DESCRIPTIONDATETIME_H__

class DescriptorDateTime {
protected:
	short year;
	short month;
	short day;
	short hour;
	short minute;
	short second;
	short hundreths;
	short greenwichOffset;
public:
	DescriptorDateTime(char *seventeenBytes="\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");

	short getYear() { return year; }
	short getMonth() { return month; }
	short getDay() { return day; }
	short getHour() { return hour; }
	short getMinute() { return minute; }
	short getSecond() { return second; }
	short getHundreths() { return hundreths; }
	short getGreenwichOffset() { return greenwichOffset; }

	char *getBytes();
};

#endif
