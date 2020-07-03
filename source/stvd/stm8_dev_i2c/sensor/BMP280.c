/*
 * BMP280.c
 *
 *  Created on: Sep 16, 2018
 *      Author: danao
 *
 * Controller file for the BMP280 Barometer / Temperature Sensor.
 * the purpose of this file is to provide a simple interface for
 * reading temperature and pressure, and humidity.
 
    Modified July 3, 2020
    - Updated the temperature and pressure compensation
    equations for 32 bit value as shown in datasheet
    - Updated datatypes
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "BMP280.h"
#include "i2c.h"

/////////////////////////////////////////////////////////
static void BMP280_dummyDelay(uint32_t delay);
static uint8_t BMP280_readReg(uint8_t reg);
void BMP280_readRegArray(uint8_t startAddress, uint8_t* buffer, uint8_t len);
static void BMP280_writeReg(uint8_t reg, uint8_t value);
static void BMP280_readCalibrationValues(void);

//////////////////////////////////////////////////////////
//Structure to hold all of the calibration data
static volatile BMP280_CalibrationData mCalibrationData;

//used in temperature and pressure correction functions
BMP280_S32_t t_fine;

//////////////////////////////////////////////////////////
//Configure for reading temperature and pressure.
//Follows Table 9 in the datasheet for best performance
//for use with indoor navigation.  Minimize noise in
//the pressure reading.
//
void BMP280_init(void)
{
	t_fine = 0x00;				//global used in compensation equations.

	//wait a bit
	BMP280_dummyDelay(20000);
	BMP280_reset();
	BMP280_dummyDelay(20000);

	//Based on the datasheet, the least amount of noise in the
	//pressure sensor is with 16x oversampling on pressure, 2x OS on
	//temperature, and IR filter coeff. of 16.
	//
	//#define BMP280_REG_CTRL_MEAS	0xF4 - temp and pressure options.
	//bits 7-5 - osrs_t - temp 010 - 2x sampling
	//bits 4-2 - osrs_p - pressure 101 - 16x sampling
	//bits 1-0 - mode.  set to 11 for normal mode., set to 00 for sleep mode
	//result = 01010100  = 0x54 - sleep
	BMP280_writeReg(BMP280_REG_CTRL_MEAS, 0x54);		//enable and sleep

	//#define BMP280_REG_CONFIG		0xF5
	//readings should be while in sleep mode or
	//else they are ignored.
	//standby 0.5ms - 000
	//filter settings - on - 16x -  100
	//disable SPI - 0
	//result - 0001 0000 = 0x10
	BMP280_writeReg(BMP280_REG_CONFIG, 0x10);

	//wakeup - bits 0 and 1 high for reg BMP280_REG_CTRL_MEAS
	BMP280_writeReg(BMP280_REG_CTRL_MEAS, 0x57);

	//Load Calibration data into ram - loads mCalibrationData
	BMP280_readCalibrationValues();
}

/////////////////////////////////////////////////////
//Software reset - write 0xB6 to reset register
//readback is always 0x00
void BMP280_reset(void)
{
	BMP280_writeReg(BMP280_REG_RESET, 0xB6);
}

/////////////////////////////////////////////////////
//Chip ID = 0x60
uint8_t BMP280_readChipID(void)
{
	uint8_t id = BMP280_readReg(BMP280_REG_ID);
	return id;
}

/////////////////////////////////////////////////////
//Read all sensors - 8 bytes starting at address
//0xF7.  Combine read bytes into values, apply correction
//factors.
void BMP280_read(BMP280_Data *result)
{
	uint8_t data[6] = {0x00};
	uint32_t msb, lsb, xlsb = 0x00;
	uint32_t uPressure, uTemperature = 0x00;
    BMP280_S32_t cTemperature = 0x00;
    BMP280_U32_t cPressure = 0x00;
    BMP280_S32_t cTemperatureF = 0x00;

	BMP280_readRegArray(BMP280_REG_PRESS_MSB, data, 6);

	//Pressure - 20bits
	msb = ((uint32_t)data[0]) & 0xFF;
	lsb = ((uint32_t)data[1]) & 0xFF;
	xlsb = ((uint32_t)(data[2] >> 4)) & 0x0F;
	uPressure = (signed long)((msb << 12) | (lsb << 4) | (xlsb));

	//temperature - 20bits
	msb = ((uint32_t)data[3]) & 0xFF;
	lsb = ((uint32_t)data[4]) & 0xFF;
	xlsb = ((uint32_t)(data[5] >> 4)) & 0x0F;
	uTemperature = (signed long)((msb << 12) | (lsb << 4) | (xlsb));

	result->uPressure = uPressure;
	result->uTemperature = uTemperature;

	//compute the corrected values using the raw data
	//and calibration values as described in the datasheet

	//compute the compensated temp
	cTemperature = BMP280_compensate_T_int32(uTemperature);	//div 100 to get int, % 100 for frac
	cPressure = BMP280_compensate_P_int32(uPressure);		//divide 256 for integer

	//integer and fractional - celcius
    cTemperatureF = cTemperature * 9 / 5;
	cTemperatureF += 3200;

	result->cTemperatureF = cTemperatureF / 100;
    result->cPressurePa = cPressure;
}




////////////////////////////////////////////////
//Put the sensor into sleep mode
void BMP280_sleep(void)
{
	uint8_t value = BMP280_readReg(BMP280_REG_CTRL_MEAS);
	value &=~ 0x03;				//bits 0 and 1 low

	BMP280_writeReg(BMP280_REG_CTRL_MEAS, value);
}

///////////////////////////////////////////////////
//Wake up the sensor and put into continuous
//reading mode.  CTRL_MEAS register, bits 0:1, 11
void BMP280_wakeup(void)
{
	uint8_t value = BMP280_readReg(BMP280_REG_CTRL_MEAS);
	value |= 0x03;				//bits 0 and 1 high

	BMP280_writeReg(BMP280_REG_CTRL_MEAS, value);
}


/////////////////////////////////////////////////////
//dummy delay
void BMP280_dummyDelay(uint32_t delay)
{
	volatile uint32_t temp = delay;
	while (temp > 0)
		temp--;
}


////////////////////////////////////////////////////
//Standard read I2C function
uint8_t BMP280_readReg(uint8_t reg)
{
	uint8_t result = 0x00;
	result = I2C_readReg(reg);
	return result;
}

//////////////////////////////////////////////////
//Standard write I2C function
void BMP280_writeReg(uint8_t reg, uint8_t value)
{
    I2C_writeReg(reg, value);
}

//////////////////////////////////////////////////////
//Read continuous address range, startign at address,
//into buffer, len bytes
void BMP280_readRegArray(uint8_t startAddress, uint8_t* buffer, uint8_t len)
{
    I2C_readArray(startAddress, buffer, len);
}

//////////////////////////////////////////////////////
//Clears all previous readings, reads registers containing
//calibration data for the ic, computes the signed/unsigned
//calibration data.  For use in computing the compensated
//humidity, temp, and pressure.
//
void BMP280_readCalibrationValues(void)
{
	//read calibration registers
	uint8_t reg[24] = {0x00};

	BMP280_readRegArray(0x88, reg, 24);			//read 24 bytes of cal data

	mCalibrationData.dig_T1 = ((uint16_t)reg[0]) + (((uint16_t)reg[1]) * 256);
	mCalibrationData.dig_T2 = ((int16_t)reg[2]) + (((int16_t)reg[3]) * 256);
	mCalibrationData.dig_T3 = ((int16_t)reg[4]) + (((int16_t)reg[5]) * 256);

	mCalibrationData.dig_P1 = ((uint16_t)reg[6]) + (((uint16_t)reg[7]) * 256);
	mCalibrationData.dig_P2 = ((int16_t)reg[8]) + (((int16_t)reg[9]) * 256);
	mCalibrationData.dig_P3 = ((int16_t)reg[10]) + (((int16_t)reg[11]) * 256);
	mCalibrationData.dig_P4 = ((int16_t)reg[12]) + (((int16_t)reg[13]) * 256);
	mCalibrationData.dig_P5 = ((int16_t)reg[14]) + (((int16_t)reg[15]) * 256);
	mCalibrationData.dig_P6 = ((int16_t)reg[16]) + (((int16_t)reg[17]) * 256);
	mCalibrationData.dig_P7 = ((int16_t)reg[18]) + (((int16_t)reg[19]) * 256);
	mCalibrationData.dig_P8 = ((int16_t)reg[20]) + (((int16_t)reg[21]) * 256);
	mCalibrationData.dig_P9 = ((int16_t)reg[22]) + (((int16_t)reg[23]) * 256);

}

/////////////////////////////////////////////////////////////////////////////////
//Function Obtained Directly from the Datasheet
// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
//BMP280_S32_t t_fine;
//t_fine is defined above - global used for pressure calcs as well.
//
BMP280_S32_t BMP280_compensate_T_int32(BMP280_S32_t adc_T)
{
	BMP280_S32_t var1, var2, T;
    
	var1 = ((((adc_T>>3) - ((BMP280_S32_t)mCalibrationData.dig_T1 << 1))) * ((BMP280_S32_t)mCalibrationData.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((BMP280_S32_t)mCalibrationData.dig_T1)) * ((adc_T >> 4) - ((BMP280_S32_t)mCalibrationData.dig_T1))) >> 12) * ((BMP280_S32_t)mCalibrationData.dig_T3)) >> 14;

	t_fine = var1 + var2;
	T  = (t_fine * 5 + 128) >> 8;
	return T;
}

/////////////////////////////////////////////////////////////////////////////////
// Function Obtained Directly from the Datasheet
// Returns pressure in Pa as unsigned 32 bit integer.  Output of 96386 is
// 96386 pa.  
BMP280_U32_t BMP280_compensate_P_int32(BMP280_S32_t adc_P)
{
	BMP280_S32_t var1 = 0;
	BMP280_S32_t var2 = 0;
	BMP280_U32_t p = 0;
    
	var1 = (((BMP280_S32_t)t_fine) >> 1) - (BMP280_S32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((BMP280_S32_t)mCalibrationData.dig_P6);    
    var2 = var2 + ((var1 * ((BMP280_S32_t)mCalibrationData.dig_P5))<<1);    
    var2 = (var2>>2) + (((BMP280_S32_t)mCalibrationData.dig_P4)<<16);    
    var1 = (((mCalibrationData.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BMP280_S32_t)mCalibrationData.dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((BMP280_S32_t)mCalibrationData.dig_P1))>>15);
    
    if (var1 == 0)
    {
        return 0;
    }
    
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576) - adc_P) - (var2>>12)))*3125;
    
    if (p < 0x80000000)
    {
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else
    {
        p = (p / (BMP280_U32_t)var1) * 2;
    }
    
    
	var1 = (((BMP280_S32_t)mCalibrationData.dig_P9) * ((BMP280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2)) * ((BMP280_S32_t)mCalibrationData.dig_P8))>>13;
    p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + mCalibrationData.dig_P7) >> 4));
    
    return p;
}



