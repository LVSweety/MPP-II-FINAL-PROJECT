#include "atmega328p_USART.h"

void MY_USART_Init(){
	
	UBRR0 = 103;

	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
	//UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);	
}

void USART_Transmit_Byte (uint8_t USART_byte){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = USART_byte;
}

void USART_Transmit_String (char *USART_string){
	uint8_t iterrator = 0;
	while (USART_string[iterrator] != '\0'){
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = USART_string[iterrator];
		iterrator++;
	}
}