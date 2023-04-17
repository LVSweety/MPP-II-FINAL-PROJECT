#ifndef ATMEGA328P_USART
#define ATMEGA328P_USART

#include <avr/io.h>
#include <string.h>

void MY_USART_Init();
void USART_Transmit_Byte (uint8_t USART_byte);

#endif