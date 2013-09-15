/*****************************************
* HMC5883L Driver for Arduino
*****************************************/

#include "HMC5883L.h"
#include <Wire.h>

HMC5883L::HMC5883L()
{
	status = HMC5883L_OK;
	errorCode = HMC5883L_NO_ERROR;
}

void HMC5883L::Init(int address)
{
	devAddress = address;
	GetDevID();
	if (errorCode == HMC5883L_NO_ERROR)
		PowerOn();
	else 
		status = HMC5883L_ERROR;
}

byte HMC5883L::GetDevID()
{
	byte name[3];
	ReadFrom(HMC5883L_ID_REG_A, 3, name);
	
	if (name[0] == HMC5883L_ID_A && name[1] == HMC5883L_ID_B && name[2] == HMC5883L_ID_C)
	{
		errorCode = HMC5883L_NO_ERROR;
		return name[0];
	}
	else
	{
		errorCode = HMC5883L_NO_DEV;
		return 0x00;
	}
}

void HMC5883L::PowerOn()
{
	SetRegisterBit(HMC5883L_CONFIG_REG_A, 7, 0);
	SetRegisterBit(HMC5883L_MODE_REG, 7, 0);
}

void HMC5883L::SleepDev()
{
	SetOutputMode(HMC5883L_SLEEP);
}

void HMC5883L::SetMeasureMode(int mode)
{
	switch (mode)
	{
		case HMC5883L_NORMAL_MEASURE:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 1, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 0, 0);
			break;
		case HMC5883L_POSITIVE_BIAS:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 1, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 0, 1);
			break;
		case HMC5883L_NEGATIVE_BIAS:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 1, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 0, 0);
			break;
		default:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 1, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 0, 0);
			break;
	}
}

void HMC5883L::SetRate(int rate)
{
	switch (rate)
	{
		case HMC5883_ODR_0_75:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 0);
			break;
		case HMC5883_ODR_1_5:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 1);
			break;
		case HMC5883_ODR_3:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 0);
			break;
		case HMC5883_ODR_7_5:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 1);
			break;
		case HMC5883_ODR_15:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 0);
			break;
		case HMC5883_ODR_30:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 1);
			break;
		case HMC5883_ODR_75:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 0);
			break;
		default:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 4, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 3, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 2, 0);
			break;
	}
}

void HMC5883L::SetSampleAverage(int sample)
{
	switch (sample)
	{
		case HMC5883L_1_SAMPLE:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 6, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 5, 0);
			break;
		case HMC5883L_2_SAMPLE:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 6, 0);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 5, 1);
			break;
		case HMC5883L_4_SAMPLE:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 6, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 5, 0);
			break;
		case HMC5883L_8_SAMPLE:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 6, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 5, 1);
			break;
		default:
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 6, 1);
			SetRegisterBit(HMC5883L_CONFIG_REG_A, 5, 1);
			break;
	}
}

void HMC5883L::SetGain(int gain)
{
	if (gain > 7)
		return;
	else
		WriteTo(HMC5883L_CONFIG_REG_B, gain << 5);
}

void HMC5883L::SetOutputMode(int mode)
{
	switch (mode)
	{
		case HMC5883L_CONTINUOUS_MODE:
			SetRegisterBit(HMC5883L_MODE_REG, 1, 0);
			SetRegisterBit(HMC5883L_MODE_REG, 0, 0);
			break;
		case HMC5883L_SINGLE_MODE:
			SetRegisterBit(HMC5883L_MODE_REG, 1, 0);
			SetRegisterBit(HMC5883L_MODE_REG, 0, 1);
			break;
		case HMC5883L_SLEEP:
			SetRegisterBit(HMC5883L_MODE_REG, 1, 1);
			SetRegisterBit(HMC5883L_MODE_REG, 0, 0);
			break;
		default:
			SetRegisterBit(HMC5883L_MODE_REG, 1, 0);
			SetRegisterBit(HMC5883L_MODE_REG, 0, 0);
			break;			
	}
}

void HMC5883L::ReadMgn(int* xyz)
{
	ReadMgn(xyz, xyz + 1, xyz + 2);
}

void HMC5883L::ReadMgn(int* x, int* y, int* z)
{
	ReadFrom(HMC5883L_OUT_X_L, HMC5883L_TO_READ, buff);
	
	*x = (((int)buff[0]) << 8) | buff[1];  
	*z = (((int)buff[2]) << 8) | buff[3];
	*y = (((int)buff[4]) << 8) | buff[5]; 
}

void HMC5883L::WriteTo(byte address, byte val)
{
	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.write(address);             // send register address
	Wire.write(val);                 // send value to write
	Wire.endTransmission();         // end transmission
}

void HMC5883L::ReadFrom(byte address, int num, byte buff[])
{
	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.write(address);       	      // sends address to read from
	Wire.endTransmission();         // end transmission

	Wire.beginTransmission(devAddress); // start transmission to device
	Wire.requestFrom(devAddress, num);    // request 6 bytes from device

	int i = 0;
	while(Wire.available())         // device may send less than requested (abnormal)
	{
		buff[i] = Wire.read();    // receive a byte
		i++;
	}
	if(i != num)
	{
		status = HMC5883L_ERROR;
		errorCode = HMC5883L_READ_ERROR;
	}
	Wire.endTransmission();         // end transmission
}

void HMC5883L::SetRegisterBit(byte regAdress, int bitPos, bool state)
{
	byte _b;
	ReadFrom(regAdress, 1, &_b);
	if (state)
	{
		_b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
	}
	else
	{
		_b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
	}
	WriteTo(regAdress, _b);
}

bool HMC5883L::GetRegisterBit(byte regAdress, int bitPos)
{
	byte _b;
	ReadFrom(regAdress, 1, &_b);
	return ((_b >> bitPos) & 1);
}

// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void HMC5883L::PrintAllRegister()
{
	byte _b;
	int i;
	for (i=0; i<=12; i++)
	{
		Serial.print("0x");
		Serial.print(i, HEX);
		Serial.print(": ");
		ReadFrom(i, 1, &_b);
		PrintByte(_b);
		Serial.println();    
	}
}

void HMC5883L::PrintByte(byte val)
{
	int i;
	Serial.print("B");
	for(i=7; i>=0; i--)
	{
		Serial.print(val >> i & 1, BIN);
	}
}