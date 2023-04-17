#include "atmega328p_I2C.h"

void I2C_Error (){
	
}

void I2C_Init (){
	TWSR |= I2C_CLOCK_PRESCALER_1;				/* TWSR – TWI Status Register */
	TWBR = I2C_CLOCK_FREQUENCY_400_kHz;			/* TWBR - TWI Bit Rate Register */
	TWCR |= (1 << TWEN);					    	/* TWCR – TWI Control Register */
}

void I2C_Start(){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != I2C_MT_START_CONDITION_TX)
	{
		I2C_Error ();
	}
}

void I2C_Repeated_Start(){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != I2C_MT_REPEATED_START_CONDITION_TX)
	{
		I2C_Error ();
	}
}

void I2C_Stop(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2C_Write(uint8_t write_data){
	TWDR = write_data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != I2C_MT_SLAVE_WRITE_TX_AND_ACK_RX)
	{
		I2C_Error();
	}

	else if ((TWSR & 0xF8) != I2C_MT_DATA_BYTE_TX_AND_ACK_RX)
	{
		I2C_Error();
	}
}

uint8_t I2C_Read(uint8_t acknowledgment){
	TWCR = (1 << TWINT) | (1 << TWEN) | (acknowledgment << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}