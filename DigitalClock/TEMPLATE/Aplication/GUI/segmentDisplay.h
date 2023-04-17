#ifndef SEGMENTDISPLAY
#define SEGMENTDISPLAY

#include <avr/io.h>
#include "variables.h"

#define D0	0b00000000
#define D1	0b00000001
#define D2	0b00000010
#define D3	0b00000011
#define D4	0b00000100
#define D5	0b00000101
#define D6	0b00000110
#define D7	0b00000111
#define D8	0b00001000
#define D9	0b00001001

/*
#define DIG0	0b01111110	//0x7E, 126
#define DIG1 	0b00001100  //0x0C, 12
#define DIG2 	0b10110110  //0xB6, 182
#define DIG3 	0b10011110  //0x9E, 158
#define DIG4 	0b11001100  //0xCC, 204
#define DIG5	0b11011010  //0xDA, 218
#define DIG6	0b11111010  //0xFA, 250
#define DIG7 	0b00001110  //0x0E, 14
#define DIG8	0b11111110  //0xFE, 254
#define DIG9	0b11011110  //0xDE, 222
*/

void BinaryCodedDecimal(Time_t *pTime, uint8_t *pArrDisplay, uint8_t *pArrBCD);
void ShiftRegister(uint8_t pClockPin, uint8_t pDataPin, uint8_t *pArrDisplay);

#endif