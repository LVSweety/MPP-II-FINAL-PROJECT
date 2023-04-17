#include "atmega328p_EXTI.h"

void MY_EXTI_Init(){
	EICRA = (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	EIMSK = (1 << INT1) | (1 << INT0);
}
