#include <wiringPiI2C.h> //Include WiringPi I2C Library
#include <stdint.h> //Include for Various Integer Data Types 

//MAX30100 Library for I2C Interfacing

#define INT_STATUS 				0x00 //Interrupt Status
#define INT_ENABLE				0x01 //Interrupt Enable
#define FIFO_WRPTR				0x02 //FIFO Write Pointer
#define FIFO_OFCTR				0x03 //FIFO Overflow Counter
#define FIFO_RDPTR				0x04 //FIFO Read Pointer
#define FIFO_DARG				0x05 //FIFO Data Register
#define MODE_CONF				0x06 //Mode Configuration
#define SPO2_CONF				0x07 //SPO2 Configuration
#define LED_CONF				0x09 //LED Configuration
#define TEMP_INT				0x16 //Temperature Integer
#define TEMP_FRACT				0x17 //Temperature Fraction
#define REV_ID					0xFE //Revision ID
#define PART_ID					0xFF //Part ID

#define SLAVE_ID				0x57 //Device ID

#define interrupt				7    //Wiring Pi Pin 7

typedef enum{
  pw200,    // 200us pulse
  pw400,    // 400us pulse
  pw800,    // 800us pulse
  pw1600    // 1600us pulse
}pulseWidth;

typedef enum{
  sr50,    // 50 samples per second
  sr100,   // 100 samples per second
  sr167,   // 167 samples per second
  sr200,   // 200 samples per second
  sr400,   // 400 samples per second
  sr600,   // 600 samples per second
  sr800,   // 800 samples per second
  sr1000   // 1000 samples per second
}sampleRate;

typedef enum{
  so2,	// SO2 interrupt
  hr,	// Heart-rate interrupt
  temp,	// Temperature interrupt
  full,	// FIFO full interrupt
}interruptSource;

typedef enum{
  i0,    // No current
  i4,    // 4.4mA
  i8,    // 7.6mA
  i11,   // 11.0mA
  i14,   // 14.2mA
  i17,   // 17.4mA
  i21,   // 20.8mA
  i27,   // 27.1mA
  i31,   // 30.6mA
  i34,   // 33.8mA
  i37,   // 37.0mA
  i40,   // 40.2mA
  i44,   // 43.6mA
  i47,   // 46.8mA
  i50    // 50.0mA
}ledCurrent;

class MAX30100{
	public:
		double getTemp(); //Returns Temperature in Celcius
		void extractValue(); //Reads 4 Bytes or a Sample from the Data Register and Stores them in IR and RED
		
		void setInfo(pulseWidth pw, ledCurrent ir, ledCurrent red, sampleRate sr, bool enableRes); //Configure Hardware Settings
		void configureMode(uint8_t mode); //Configure Measuring Mode
		void clearPointers(); //Clear and Reset Read and Write Pointers
		void reset(); //Reset All Configurations and Settings
		
		void enableInterrupt(); //Enable Software Interrupts
		void attachInterrupt(); //Attach an Inerrupt that Pauses Program Execution Until Interrupt Returns True
		
		uint16_t getIR(); //Gets IR
		uint16_t getRED(); //Gets RED
	
		MAX30100(); //Default Constructor that Initalizes WiringPi Libraries

	private:
		int fd; //File Descriptor for Sensor Address
		uint16_t IR = 0; //Last Infrared Data Point
		uint16_t RED = 0; //Last Red Data Point
		
		void writeTo(uint8_t address, uint8_t data); //Writes a byte to register
		uint8_t readFrom(uint8_t address); //Reads a byte from register
	
		bool checkBit(uint8_t byte, int position); //Checks the status of a bit in a byte based on sequence position
		uint16_t combine(uint8_t first, uint8_t second); //Combines two bytes into a 2 byte integer for 16 bit resolution
	
};
