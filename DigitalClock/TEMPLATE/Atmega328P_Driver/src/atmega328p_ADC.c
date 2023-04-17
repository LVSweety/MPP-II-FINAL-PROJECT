#include "atmega328p_ADC.h"

void MY_ADC_Init(){
	ADMUX = (1 << REFS0) | ADC_CHANNEL_2;
	ADCSRB = ADC_TRIGGER_SOURCE_FREE_RUNNING_MODE;
	ADCSRA = (1 << ADEN) | ADC_PRESCALER_128;	
}

uint16_t Analog_Read(uint8_t ADC_Channel){
	ADMUX = (ADMUX & 0xF0) | ADC_Channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}