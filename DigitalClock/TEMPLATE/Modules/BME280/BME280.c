#include "BME280.h"

void BME280_Init(uint8_t address){
	I2C_Start();
	I2C_Write(0xEC);
	I2C_Write(0xF4);
	I2C_Write(0x57);
	
	//I2C_Write(0xEC);
	I2C_Write(0xF5);
	I2C_Write(0x10);
	I2C_Stop();
}

void BME280_CompensationData(uint8_t address){
	uint8_t dataByte0 = 0;
	uint8_t dataByte1 = 0;
	
	I2C_Start();
	I2C_Write(0xEC);
	I2C_Write(0x88);
	
	I2C_Repeated_Start();
	I2C_Write(0xED);
	
	dataByte0 = I2C_Read(0);
	//I2C_Read(1);
	
	//I2C_Read(1);
	//I2C_Read(1);
	
	//I2C_Read(1);
	//I2C_Read(0);
	
	I2C_Stop();
}

uint32_t readTemp(){
	uint8_t dataByte0 = 0;
	uint8_t dataByte1 = 0;
	uint8_t dataByte2 = 0;
	uint32_t dataValue = 0;
	
	I2C_Start();
	I2C_Write(0xEC);
	I2C_Write(0xFA);
	I2C_Repeated_Start();
	I2C_Write(0xED);
	dataByte0 = I2C_Read(1);
	dataByte1 = I2C_Read(1);
	dataByte2 = I2C_Read(0);
	I2C_Stop();
	dataValue |= (dataByte0 << 0);
	dataValue |= (dataByte1 << 7);
	dataValue |= (dataByte2 << 15);
	return dataValue;
}

int32_t t_fine;
int32_t BME280_compensate_T_int32(uint32_t adc_T){
	int32_t var1, var2, T;
	var1  = ((((adc_T>>3) - ((uint32_t)bmeFactoryData.dig_T1<<1))) * ((uint32_t)bmeFactoryData.dig_T2)) >> 11;
	var2  = (((((adc_T>>4) - ((uint32_t)bmeFactoryData.dig_T1)) * ((adc_T>>4) - ((uint32_t)bmeFactoryData.dig_T1))) >> 12) * ((uint32_t)bmeFactoryData.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T  = (t_fine * 5 + 128) >> 8;
	return T;
}