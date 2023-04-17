#include "segmentDisplay.h"

void BinaryCodedDecimal(Time_t *pTime, uint8_t *pArrDisplay, uint8_t *pArrBCD){
	pArrDisplay [0] = pArrBCD [pTime->miliseconds % 10];
	pArrDisplay [1] = pArrBCD [pTime->miliseconds / 10];
	pArrDisplay [2] = pArrBCD [pTime->seconds % 10];
	pArrDisplay [3] = pArrBCD [pTime->seconds / 10];
	pArrDisplay [4] = pArrBCD [pTime->minutes % 10];
	pArrDisplay [5] = pArrBCD [pTime->minutes / 10];
	pArrDisplay [6] = pArrBCD [pTime->hours % 10];
	pArrDisplay [7] = pArrBCD [pTime->hours / 10];
}

void ShiftRegister(uint8_t pClockPin, uint8_t pDataPin, uint8_t *pArrDisplay){
	static uint8_t mask = 0x0F;
	static uint8_t shiftCnt = 0;

	if (shiftCnt == 0){
		/* Change PORTB if necessary */
		PORTC |= (1 << pDataPin);
	}else{
		PORTC &= ~(1 << pDataPin);
	}
	/* Change PORTB if necessary */
	PORTD = (pArrDisplay [shiftCnt] << 4) | mask;
	PORTC |= (1 << pClockPin);
	asm volatile
	(
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	::);
	PORTC &= ~(1 << pClockPin);

	shiftCnt++;
	if (shiftCnt == 8){
		shiftCnt = 0;
	}
}