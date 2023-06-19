#ifndef BME280
#define BME280

#include <avr/io.h>

#include "atmega328p_I2C.h"

#define BMP280_WRITE                0b11101100
#define BMP280_READ                 0b11101101
#define BMP280_CONTROL_REG			0xF4
#define BMP280_CONFIG_REG			0xF5
#define BMP280_TEMPERATURE_REG		0xFA
#define BMP280_TEMPERATURE_REG		0xFA
#define BMP280_PRESSURE_REG			0xF7
#define BMP280_MOISTURE_REG			0xFD

#define BMP280_COMPENSATION_START_ADDRESS	0x88
#define BMP280_COMPENSATION_H1_ADDRESS		0xA1
#define BMP280_COMPENSATION_H2_H6_ADDRESS   0xE1
uint32_t readTemp();
void BME280_Init(uint8_t address);
void BME280_CompensationData(uint8_t address);
void bmp280_GetRawTemperature();
double BMP280_GetTemperature();
void bmp280_GetRawPressure();
double BMP280_GetPressure();
void bmp280_GetRawMoisture();
double BMP280_GetMoisture();

struct BMP280_Calib_Data{
	uint16_t dig_T1; // adrese: 0x88 / 0x89
	int16_t dig_T2;  // adrese: 0x8A / 0x8B
	int16_t dig_T3;  // adrese: 0x8C / 0x8D
	uint16_t dig_P1; // adrese: 0x8E / 0x8F
	int16_t dig_P2;  // adrese: 0x90 / 0x91
	int16_t dig_P3;  // adrese: 0x92 / 0x93
	int16_t dig_P4;  // adrese: 0x94 / 0x95
	int16_t dig_P5;  // adrese: 0x96 / 0x97
	int16_t dig_P6;  // adrese: 0x98 / 0x99
	int16_t dig_P7;  // adrese: 0x9A / 0x9B
	int16_t dig_P8;  // adrese: 0x9C / 0x9D
	int16_t dig_P9;  // adrese: 0x9E / 0x9F
	// add for humidity if BME280 is used...
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;
}Compensation_Data;

struct BME280_TemperatureData{
	uint8_t temp_msb;   // temp_msb, kam adreses v?rt?ba: 0xFA
	uint8_t temp_lsb;   // temp_lsb, kam adreses v?rt?ba: 0xFB
	uint8_t temp_xlsb;  // temp_xlsb, kam adreses v?rt?ba: 0xFC
	uint32_t ADC_T;
}myTemperatureData;

struct BME280_PressureData{
	uint8_t press_msb;  // press_msb, kam adreses v?rt?ba: 0xF7
	uint8_t press_lsb;  // press_lsb, kam adreses v?rt?ba: 0xF8
	uint8_t press_xlsb; // press_xlsb, kam adreses v?rt?ba: 0xF9
	uint32_t ADC_P;
}myPressureData;

struct BME280_HumidityData{
	uint8_t hum_msb;  // hum_msb, kam adreses v?rt?ba: 0xFD
	uint8_t hum_lsb;  // hum_lsb, kam adreses v?rt?ba: 0xFE

	uint32_t ADC_H;
}myHumidityData;
#endif