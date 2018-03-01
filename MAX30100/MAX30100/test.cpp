#include "MAX30100.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

int main()
{
	MAX30100 sensor;
	sensor.reset();
	sensor.configureMode(0b00000010);
	sensor.setInfo(pw1600,i31,i11,sr100,true);
	sensor.enableInterrupt();
	int countPtr = 0;
	while(countPtr<1001)
	{
		countPtr++;
		sensor.clearPointers();
		sensor.writeTo(FIFO_DARG,0x0);
		sensor.attachInterrupt();
		sensor.extractValue();
		std::cout << sensor.IR << std::endl;
		myfile<<sensor.IR<<"\n";
	}
	myfile.close();
	return 0;
}
