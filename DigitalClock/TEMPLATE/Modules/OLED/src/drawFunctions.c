#include "drawFunctions.h"

void MY_GUI_DrawSymbol(uint8_t px, uint8_t py, uint8_t pSymbol){
	SSD1306_SetPosition(px, py);
	SSD1306_DrawString(pSymbol);
}

void MY_GUI_DrawString(uint8_t px, uint8_t py, uint8_t *pSymbol){
	SSD1306_SetPosition(px, py);
	SSD1306_DrawString(pSymbol);
}