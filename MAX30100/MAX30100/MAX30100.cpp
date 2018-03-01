#include "MAX30100.h"
#include  <unistd.h>

MAX30100::MAX30100(){
	fd = wiringPiI2CSetup(SLAVE_ID);
	wiringPiSetup();
}

double MAX30100::getTemp(){
	writeTo(MODE_CONF,0b00001000); //Enable Temperature Only
	usleep(1000);
	double temp_int = (double)readFrom(TEMP_INT);
	double temp_fract = (double)readFrom(TEMP_FRACT)/(double)16;
	return temp_int+temp_fract;
}

bool MAX30100::checkBit(uint8_t byte, int position){
	uint8_t temp  = byte & (0b10000000>>(position-1));
	if(temp != 0)
	{
		return true;
	}
	return false;
}

void MAX30100::writeTo(uint8_t address, uint8_t data){
	wiringPiI2CWriteReg8(fd, address, data);
}

uint8_t MAX30100::readFrom(uint8_t address){
	return wiringPiI2CReadReg8(fd,address);
}

void MAX30100::extractValue(){
	uint8_t storage[4];
	storage[0] = readFrom(FIFO_DARG);
	storage[1] = readFrom(FIFO_DARG);
	storage[2] = readFrom(FIFO_DARG);
	storage[3] = readFrom(FIFO_DARG);
	//std::cout << "Debug " << storage[0] << std::endl;
	IR = combine(storage[0],storage[1]);
	RED = combine(storage[2],storage[3]);
}

uint16_t MAX30100::combine(uint8_t first, uint8_t second){
	uint16_t combined = (first<<8) | second;
	return combined;
}

//pw 1600, samples 100, current 942 IDD micro, 9.766 LED milli
void MAX30100::setInfo(pulseWidth pw, ledCurrent ir, ledCurrent red, sampleRate sr, bool enableRes){
	uint8_t highRes = 0;
	if(enableRes)
	{
		highRes = 0b01000000;
	}
	writeTo(SPO2_CONF,highRes|(sr<<2)|pw); //Need to Consider High Resolution
	writeTo(LED_CONF, (red<<4)|ir);
}

void MAX30100::reset(){
	writeTo(MODE_CONF,0b01000000);
}

void MAX30100::configureMode(uint8_t mode){
	writeTo(MODE_CONF,mode);
}

void MAX30100::attachInterrupt(){
	while(checkBit(readFrom(INT_STATUS),3)==0){}
}

void MAX30100::clearPointers(){
	writeTo(FIFO_WRPTR,0x0);
	writeTo(FIFO_RDPTR,0x0);
}

void MAX30100::enableInterrupt(){
	writeTo(INT_ENABLE,0b00100000);
}

uint16_t MAX30100::getIR(){
	return IR;
}

uint16_t MAX30100::getRED(){
	return RED;
}
