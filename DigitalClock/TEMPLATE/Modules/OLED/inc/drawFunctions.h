/*
 * drawFunctions.h
 *
 * Created: 13/04/2023 11:21:18
 *  Author: Sweets
 */ 


#ifndef DRAWFUNCTIONS
#define DRAWFUNCTIONS

#include <avr/io.h>
#include "ssd1306.h"

void MY_GUI_DrawSymbol(uint8_t px, uint8_t py, uint8_t pSymbol);
void MY_GUI_DrawString(uint8_t px, uint8_t py, uint8_t *pSymbol);

#endif /* DRAWFUNCTIONS_H_ */