#ifndef GUI
#define GUI

#include <avr/io.h>
#include <math.h>

#include "ssd1306.h"
#include "variables.h"
#include "drawFunctions.h"

void FSM_GUI_State(Flags *pSystemFlags, GUI_DRAW_MENU *pGUIState, GUI_DRAW_ARROW *pGUIArrowState);
void GUI_DrawMenu(GUI_DRAW_MENU pDrawMenu, Flags *pSystemFlags);
void GUI_DrawArrow(GUI_DRAW_ARROW pGUIArrowState);
void GUI_DrawSysTime(Time_t TimeMainClock);
void GUI_DrawEnv(Enviroment_t BME280Env);
void GUI_DrawVoltageOutput(uint16_t ADCvalue, Time_t TimeMainClock, uint8_t channel);
#endif