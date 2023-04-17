#ifndef ATMEGA328P_I2C
#define ATMEGA328P_I2C

#include <avr/io.h>

//P5.1.1.
#define I2C_CLOCK_FREQUENCY_400_kHz             0x0C    // MCU running @ 16MHz
#define I2C_MT_START_CONDITION_TX               0x08	//	A START condition has been transmitted
#define I2C_MT_REPEATED_START_CONDITION_TX	    0x10	//	A repeated START condition has been transmitted
#define I2C_MT_SLAVE_WRITE_TX_AND_ACK_RX        0x18	//	SLA+W has been transmitted; ACK has been received
#define I2C_MT_SLAVE_WRITE_TX_AND_NACK_RX		0x20	//	SLA+W has been transmitted; NOT ACK has been received
#define I2C_MT_DATA_BYTE_TX_AND_ACK_RX			0x28	//	Data byte has been transmitted; ACK has been received
#define I2C_MT_DATA_BYTE_TX_AND_NACK_RX			0x30	//	Data byte has been transmitted; NOT ACK has been received
#define I2C_MT_ARBITRATION_LOST					0x38	//	Arbitration lost in SLA+W or data bytes
#define I2C_CLOCK_PRESCALER_1					0x01

void I2C_Error();
void I2C_Start();
void I2C_Repeated_Start();
void I2C_Stop();
void I2C_Write(uint8_t write_data);
uint8_t I2C_Read(uint8_t acknowledgment);

#endif