#ifndef BME280
#define BME280

#include <avr/io.h>

#include "atmega328p_I2C.h"

uint32_t readTemp();
void BME280_Init(uint8_t address);
void BME280_CompensationData(uint8_t address);

struct BME280_factoryData{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
}bmeFactoryData;


#endif