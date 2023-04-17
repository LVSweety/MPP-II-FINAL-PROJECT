#ifndef ATMEGA328P_GPIO
#define	ATMEGA328P_GPIO

#include <avr/io.h>

void	MY_GPIO_Init();
uint8_t MY_GPIO_ReadPin(uint8_t *GPIOx, uint8_t GPIO_Pin);
void	MY_GPIO_WritePin(uint8_t *GPIOx, uint8_t GPIO_Pin, uint8_t GPIO_PinState);
void	MY_GPIO_TogglePin(volatile uint8_t *GPIOx, uint8_t GPIO_Pin);

#endif