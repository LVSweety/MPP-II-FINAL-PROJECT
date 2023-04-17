#include "atmega328p_TIM.h"

void MY_TIM0_Init(){

	TCNT0 = 0;
	OCR0A = 10;

	TCCR0A |= (1 << WGM01);

	TIMSK0 |= (1 << OCIE0A);

	MY_TIMX_Start(TIM0_START, TIM0_PRESCALING_FACTOR_1024);
}

void MY_TIM1_Init(){

	TCNT1 = 0;
	OCR1A = 15625;

	TCCR1B |= (1 << WGM12);

	TIMSK1 |= (1 << OCIE1A);

	MY_TIMX_Start(TIM1_START, TIM1_PRESCALING_FACTOR_1024);
}

void MY_TIM2_Init(){
	
	TCNT2 = 0;
	OCR2A = 156;

	TCCR2A |= (1 << WGM21);

	TIMSK2 |= (1 << OCIE2A);

	MY_TIMX_Start(TIM2_START, TIM2_PRESCALING_FACTOR_1024);
}

void MY_TIMX_Start (uint8_t pTIMxNameState, uint8_t pPrescalerFactor){
	switch(pTIMxNameState){
		case TIM0_START:
		TCCR0B &= 0xF8; // maska 0b1111_1000, attiesta pirmos tr?s bitus no lab?s puses
		TCCR0B |= pPrescalerFactor;
		break;

		case TIM1_START:
		TCCR1B &= 0xF8;
		TCCR1B |= pPrescalerFactor;
		break;

		case TIM2_START:
		TCCR2B &= 0xF8;
		TCCR2B |= pPrescalerFactor;
		break;
	}
}

void MY_TIMX_Stop (uint8_t pTIMxNameState)
{
	switch(pTIMxNameState)
	{
		/* START particular timer/counter: set necessary CS (clock select bits) bit combination
		 * CS02		CS01	CS00	|	Description
		 * -----------------------------------------------------------------------
		 * 0		0		1		|	clkI/O/(no prescaling)
		 * 0		1		0		|	clkI/O/8 (from prescaler)
		 * 0		1		1		|	clkI/O/64 (from prescaler)
		 * 1		0		0		|	clkI/O/256 (from prescaler)
		 * 1		0		1		|	clkI/O/1024 (from prescaler)
		 * 1		1		0		|	External clock source on T0 pin. Clock on falling edge.
		 * 1		1		1		|	External clock source on T0 pin. Clock on rising edge.
		 * 0		0		0		|	No clock source (Timer/Counter stopped)
		 */

		/* STOP particular timer/counter: clear all CS (clock select bits) */
		case TIM0_STOP:
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
		break;

		case TIM1_STOP:
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		break;

		case TIM2_STOP:
		TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
		break;
	}
}