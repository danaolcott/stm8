/*
 * BMP280.h
 *
 *  Created on: Sep 16, 2018
 *      Author: danao
 *
 *  Modified June 30, 2020
 *
 * Controller file for the BMP280 Barometer / Temperature Sensor.
 * The purpose of this file is to provide a simple interface for
 * reading temperature and pressure.
 *
 * Note: This controller file has been modified from the BME280
 * version that also read humidity
 *
 */

#ifndef SENSORS_BMP280_H_
#define SENSORS_BMP280_H_



#include <stddef.h>
#include <stdint.h>
#include "register.h"


//datatypes - for use in mfg compensation equations
#define BMP280_S32_t			signed long
#define BMP280_U32_t			unsigned long



#define BMP280_I2C_ADDRESS		(uint8_t)(0x76)     //not upshifted


#define BMP280_REG_TEMP_XLSB	0xFC
#define BMP280_REG_TEMP_LSB		0xFB
#define BMP280_REG_TEMP_MSB		0xFA

#define BMP280_REG_PRESS_XLSB	0xF9
#define BMP280_REG_PRESS_LSB	0xF8
#define BMP280_REG_PRESS_MSB	0xF7

#define BMP280_REG_CONFIG		0xF5
#define BMP280_REG_CTRL_MEAS	0xF4
#define BMP280_REG_STATUS		0xF3

#define BMP280_REG_RESET		0xE0
#define BMP280_REG_ID			0xD0		//0x58 for BMP, 0x60 for BME


typedef struct
{
	signed long uPressure;
	signed long uTemperature;
	signed long cTemperatureF;
	unsigned long cPressurePa;

}BMP280_Data;


//Calibration data presented in Table 16
typedef struct
{
	uint16_t dig_T1;		//temperature
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;		//pressure
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

}BMP280_CalibrationData;


void BMP280_init(void);
void BMP280_reset(void);
uint8_t BMP280_readChipID(void);
void BMP280_read(BMP280_Data *result);

void BMP280_sleep(void);
void BMP280_wakeup(void);


BMP280_S32_t BMP280_compensate_T_int32(BMP280_S32_t adc_T);

BMP280_U32_t BMP280_compensate_P_int32(BMP280_S32_t adc_P);

#endif /* SENSORS_BMP280_H_ */
