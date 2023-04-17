#include "atmega328p_GPIO.h"

void MY_GPIO_Init(){
	
	DDRB = 0b00100000;
	PORTB = 0b00000000;
	
	DDRC = 0b00000011;
	PORTC = 0b00000000;
	
	DDRD = 0b11110000;
	PORTD = 0b00001100;
}

uint8_t MY_GPIO_ReadPin(uint8_t *GPIOx, uint8_t GPIO_Pin){
	if(*GPIOx & (1 << GPIO_Pin)){
		return 1;
	}
	
	return 0;
}

void MY_GPIO_TogglePin(volatile uint8_t *GPIOx, uint8_t GPIO_Pin){
	*GPIOx ^= (1 << GPIO_Pin);
}

void MY_GPIO_WritePin(uint8_t *GPIOx, uint8_t GPIO_Pin, uint8_t GPIO_PinState){
	if(GPIO_PinState == 1){
		*GPIOx |= (1 << GPIO_Pin);
	}else{
		*GPIOx &= ~(1 << GPIO_Pin);
	}
	
}