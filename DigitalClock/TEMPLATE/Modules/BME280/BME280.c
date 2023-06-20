#include "BME280.h"

void BMP280_Init(){
	
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	//I2C_Write(0xF2);
	//I2C_Write(0b00000111);
	I2C_Write(BMP280_CONTROL_REG);
	I2C_Write(0b01010111);
	I2C_Write(BMP280_CONFIG_REG);
	I2C_Write(0b00010000);
	I2C_Stop();

	
	
	BMP280_Compensation_Data();
}

void BMP280_Compensation_Data(){
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_COMPENSATION_START_ADDRESS);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);

	Compensation_Data.dig_T1 = I2C_Read(1);
	Compensation_Data.dig_T1 += I2C_Read(1) << 8;
	Compensation_Data.dig_T2 = I2C_Read(1);
	Compensation_Data.dig_T2 += I2C_Read(1) << 8;
	Compensation_Data.dig_T3 = I2C_Read(1);
	Compensation_Data.dig_T3 += I2C_Read(1) << 8;
	Compensation_Data.dig_P1 = I2C_Read(1);
	Compensation_Data.dig_P1 += I2C_Read(1) << 8;
	Compensation_Data.dig_P2 = I2C_Read(1);
	Compensation_Data.dig_P2 += I2C_Read(1) << 8;
	Compensation_Data.dig_P3 = I2C_Read(1);
	Compensation_Data.dig_P3 += I2C_Read(1) << 8;
	Compensation_Data.dig_P4 = I2C_Read(1);
	Compensation_Data.dig_P4 += I2C_Read(1) << 8;
	Compensation_Data.dig_P5 = I2C_Read(1);
	Compensation_Data.dig_P5 += I2C_Read(1) << 8;
	Compensation_Data.dig_P6 = I2C_Read(1);
	Compensation_Data.dig_P6 += I2C_Read(1) << 8;
	Compensation_Data.dig_P7 = I2C_Read(1);
	Compensation_Data.dig_P7 += I2C_Read(1) << 8;
	Compensation_Data.dig_P8 = I2C_Read(1);
	Compensation_Data.dig_P8 += I2C_Read(1) << 8;
	Compensation_Data.dig_P9 = I2C_Read(1);
	Compensation_Data.dig_P9 += I2C_Read(0) << 8;

	I2C_Stop();
	
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_COMPENSATION_H1_ADDRESS);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);
	
	Compensation_Data.dig_H1 = I2C_Read(0);
	
	I2C_Stop();
	
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_COMPENSATION_H2_H6_ADDRESS);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);
	
	Compensation_Data.dig_H2 = I2C_Read(1);
	Compensation_Data.dig_H2 += I2C_Read(1) << 8;
	
	Compensation_Data.dig_H3 = I2C_Read(1);
	
	Compensation_Data.dig_H4 = I2C_Read(1) << 4;
	uint8_t E5_BIT = I2C_Read(1);
	Compensation_Data.dig_H4 += E5_BIT & 0b00001111;
	
	Compensation_Data.dig_H5 = E5_BIT >> 4;
	Compensation_Data.dig_H5 += I2C_Read(1) << 4;
	
	Compensation_Data.dig_H6 = I2C_Read(0);
		
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

int32_t t_fine = 0;

int32_t BMP280_GetTemperature() {
	bmp280_GetRawTemperature();
	int32_t var1, var2, T;
	var1 = ((((myTemperatureData.ADC_T>>3) - ((int32_t)Compensation_Data.dig_T1<<1))) * ((int32_t)Compensation_Data.dig_T2)) >> 11;
	var2 = (((((myTemperatureData.ADC_T>>4) - ((int32_t)Compensation_Data.dig_T1)) * ((myTemperatureData.ADC_T>>4) - ((int32_t)Compensation_Data.dig_T1))) >> 12) * ((int32_t)Compensation_Data.dig_T3)) >> 14;
	t_fine = var1 + var2; T = (t_fine * 5 + 128) >> 8;
	return T;
}
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t BMP280_GetPressure() {
	bmp280_GetRawPressure();
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)Compensation_Data.dig_P6;
	var2 = var2 + ((var1*(int64_t)Compensation_Data.dig_P5)<<17);
	var2 = var2 + (((int64_t)Compensation_Data.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)Compensation_Data.dig_P3)>>8) + ((var1 * (int64_t)Compensation_Data.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)Compensation_Data.dig_P1)>>33;
	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-myPressureData.ADC_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)Compensation_Data.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)Compensation_Data.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)Compensation_Data.dig_P7)<<4);
	return (uint32_t)(p/(int64_t)2560);
}
/*
double t_fine = 0;
double BMP280_GetTemperature(){
	bmp280_GetRawTemperature();
	double var1 = 0.0, var2 = 0.0, myTemperature = 0.0;

	var1 = (((double) myTemperatureData.ADC_T)/16384.0 - ((double) Compensation_Data.dig_T1)/1024.0) * ((double) Compensation_Data.dig_T2);
	var2 = ((((double) myTemperatureData.ADC_T)/131072.0 - ((double) Compensation_Data.dig_T1)/8192.0) * (((double) myTemperatureData.ADC_T)/131072.0 - ((double) Compensation_Data.dig_T1)/8192.0)) * ((double) Compensation_Data.dig_T3);
	t_fine =  (var1 + var2);
	myTemperature = (var1 + var2) / 5120.0;

	return myTemperature;
}*/

/*
double BMP280_GetPressure(){
	bmp280_GetRawPressure();
	double var1, var2, p;
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double) Compensation_Data.dig_P6) / 32768.0;
	var2 = var2 + var1 + ((double) Compensation_Data.dig_P5) * 2.0;
	var2 = (var2/4.0) + (((double) Compensation_Data.dig_P4) * 65536.0);
	var1 = (((double) Compensation_Data.dig_P3) * var1 * var1 / 524288.0 + ((double) Compensation_Data.dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double) Compensation_Data.dig_P1);
	if (var1 == 0.0){
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double) myPressureData.ADC_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double) Compensation_Data.dig_P9) * p* p / 2147483648.0;
	var2 = p * ((double) Compensation_Data.dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double) Compensation_Data.dig_P7)) / 16.0;
	return p;
}*/

int32_t BMP280_GetHumidity() {
	bmp280_GetRawHumidity();
	int32_t h;
	h = (t_fine - ((int32_t)76800));
	h = (((((myHumidityData.ADC_H << 14) - (((int32_t)Compensation_Data.dig_H4) << 20) - (((int32_t)Compensation_Data.dig_H5) * h)) + ((int32_t)16384)) >> 15) * (((((((h * ((int32_t)Compensation_Data.dig_H6)) >> 10) * (((h * ((int32_t)Compensation_Data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)Compensation_Data.dig_H2) + 8192) >> 14));
	h = (h - (((((h >> 15) * (h >> 15)) >> 7) * ((int32_t)Compensation_Data.dig_H1)) >> 4));
	h = (h < 0 ? 0 : h);
	h = (h > 419430400 ? 419430400 : h);
	h = (int32_t)(h>>12);
	return h;
}

/*
double BMP280_GetHumidity() {
	bmp280_GetRawHumidity();
	double var_H; var_H = (((double)t_fine) - 76800.0);
	var_H = (myHumidityData.ADC_H - (((double)Compensation_Data.dig_H4) * 64.0 + ((double)Compensation_Data.dig_H5) / 16384.0 * var_H)) * (((double)Compensation_Data.dig_H2) / 65536.0 * (1.0 + ((double)Compensation_Data.dig_H6) / 67108864.0 * var_H * (1.0 + ((double)Compensation_Data.dig_H3) / 67108864.0 * var_H)));
	var_H = var_H * (1.0 - ((double)Compensation_Data.dig_H1) * var_H / 524288.0);
	if (var_H > 100.0)
		var_H = 100.0;
	else if (var_H < 0.0)
		var_H = 0.0;
	return (double) var_H;
}*/

void bmp280_GetRawTemperature(){
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_TEMPERATURE_REG);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);

	myTemperatureData.temp_msb = I2C_Read(1);
	myTemperatureData.temp_lsb = I2C_Read(1);
	myTemperatureData.temp_xlsb = I2C_Read(0);

	I2C_Stop();

	myTemperatureData.ADC_T = ((uint32_t)myTemperatureData.temp_msb << 12)
	| ((uint32_t)myTemperatureData.temp_lsb << 4)
	| (uint32_t)(myTemperatureData.temp_xlsb >> 4);
}

void bmp280_GetRawPressure(){
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_PRESSURE_REG);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);

	myPressureData.press_msb = I2C_Read(1);
	myPressureData.press_lsb = I2C_Read(1);
	myPressureData.press_xlsb = I2C_Read(0);

	I2C_Stop();

	myPressureData.ADC_P = ((uint32_t)myPressureData.press_msb << 12)
	| ((uint32_t)myPressureData.press_lsb << 4)
	| (uint32_t)(myPressureData.press_xlsb >> 4);
}

void bmp280_GetRawHumidity(){
	I2C_Start();
	I2C_Write(BMP280_WRITE);
	I2C_Write(BMP280_MOISTURE_REG);
	I2C_Repeated_Start();
	I2C_Write(BMP280_READ);

	myHumidityData.hum_msb = I2C_Read(1);
	myHumidityData.hum_lsb = I2C_Read(0);

	I2C_Stop();

	myHumidityData.ADC_H = ((uint32_t)myHumidityData.hum_msb << 12)
	| ((uint32_t)myHumidityData.hum_lsb << 4);
}