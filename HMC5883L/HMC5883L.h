/*****************************************
* HMC5883L Driver for Arduino
*****************************************/

#include "Arduino.h"

#ifndef HMC5883L_h
#define HMC5883L_h

#define HMC5883L_ADDR 0x1E

#define HMC5883L_CONFIG_REG_A	0x00
#define HMC5883L_CONFIG_REG_B	0x01
#define HMC5883L_MODE_REG		0x02
#define HMC5883L_OUT_X_L		0x03
#define HMC5883L_OUT_X_H		0x04
#define HMC5883L_OUT_Z_L		0x05
#define HMC5883L_OUT_Z_H		0x06
#define HMC5883L_OUT_Y_L		0x07
#define HMC5883L_OUT_Y_H		0x08
#define HMC5883L_STATUS_REG		0x09
#define HMC5883L_ID_REG_A		0x0A
#define HMC5883L_ID_REG_B		0x0B
#define HMC5883L_ID_REG_C		0x0C

// For Config Register A
#define HMC5883L_NORMAL_MEASURE	1
#define HMC5883L_POSITIVE_BIAS	2
#define HMC5883L_NEGATIVE_BIAS	3
#define HMC5883L_1_SAMPLE	1
#define HMC5883L_2_SAMPLE	2
#define HMC5883L_4_SAMPLE	3
#define HMC5883L_8_SAMPLE	4
#define HMC5883_ODR_0_75	1
#define HMC5883_ODR_1_5		2
#define HMC5883_ODR_3		3
#define HMC5883_ODR_7_5		4
#define HMC5883_ODR_15		5
#define HMC5883_ODR_30		6
#define HMC5883_ODR_75		7

// For Config Register B
#define HMC5883L_GAIN_0_88	0
#define HMC5883L_GAIN_1_3	1
#define HMC5883L_GAIN_1_9	2
#define HMC5883L_GAIN_2_5	3
#define HMC5883L_GAIN_4_0	4
#define HMC5883L_GAIN_4_7	5
#define HMC5883L_GAIN_5_6	6
#define HMC5883L_GAIN_8_1	7

// For Mode Register
#define HMC5883L_CONTINUOUS_MODE	1
#define HMC5883L_SINGLE_MODE	2
#define HMC5883L_SLEEP			3

#define HMC5883L_OK		1	// no error
#define HMC5883L_ERROR	0	// error predent

#define HMC5883L_NO_ERROR	0	// initial state
#define HMC5883L_READ_ERROR	1	// problem reading
#define HMC5883L_BAD_ARG	2	// bad method argument
#define HMC5883L_NO_DEV		3

#define HMC5883L_ID_A		'H'
#define HMC5883L_ID_B		'4'
#define HMC5883L_ID_C		'3'
#define HMC5883L_TO_READ 6

class HMC5883L
{
public:
	bool status;
	byte errorCode;
	
	HMC5883L();
	void Init(int address);
	byte GetDevID();
	void PowerOn();
	void SleepDev();
	void ReadMgn(int* xyz);
	void ReadMgn(int* x, int* y, int* z);
	void WriteTo(byte address, byte val);
	
	void SetMeasureMode(int mode);
	void SetRate(int rate);		// set output data frequency (Hz)
	void SetSampleAverage(int sample);
	void SetGain(int gain);
	void SetOutputMode(int mode);
	
	void PrintAllRegister();

private:
	byte buff[HMC5883L_TO_READ];
	int devAddress;
	
	void ReadFrom(byte address, int num, byte buff[]);
	void SetRegisterBit(byte regAdress, int bitPos, bool state);
	bool GetRegisterBit(byte regAdress, int bitPos);
	void PrintByte(byte val);
};
#endif