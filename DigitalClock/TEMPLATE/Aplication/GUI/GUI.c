#include "GUI.h"

void FSM_GUI_State(Flags *pSystemFlags, GUI_DRAW_MENU *pGUIState, GUI_DRAW_ARROW *pGUIArrowState){
	if(pSystemFlags -> GUI_InputEvent == TRUE){
		if(pSystemFlags -> GUI_CMD_UP == TRUE){
			pSystemFlags -> GUI_CMD_UP = FALSE;
			switch (*pGUIState){
				case DRAW_MENU_ROOT:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_04;
						break;
						case MENU_LIST_LENGTH_03:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
						break;
						case MENU_LIST_LENGTH_04:
							*pGUIArrowState = MENU_LIST_LENGTH_03;
						break;
						default:
						break;
					}
				case DRAW_MENU_SYSTEM_SETTINGS:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_07;
						break;
						case MENU_LIST_LENGTH_07:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
						break;
						default:
						break;
					}
				break;
				default:
				break;
			}
		}else if(pSystemFlags -> GUI_CMD_DOWN == TRUE){
			pSystemFlags -> GUI_CMD_DOWN = FALSE;
			switch (*pGUIState){
				case DRAW_MENU_ROOT:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_03;
						break;
						case MENU_LIST_LENGTH_03:
							*pGUIArrowState = MENU_LIST_LENGTH_04;
						break;
						case MENU_LIST_LENGTH_04:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
						break;
						default:
						break;
					}
				case DRAW_MENU_SYSTEM_SETTINGS:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_07;
						break;
						case MENU_LIST_LENGTH_07:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
						break;
						default:
						break;
					}
				break;
				default:
				break;
			}
		}else if(pSystemFlags -> GUI_CMD_ENTER == TRUE){
			pSystemFlags -> GUI_CMD_ENTER = FALSE;
			switch (*pGUIState){
				case DRAW_MENU_ROOT:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_07;
							*pGUIState = DRAW_MENU_MICROCLIMATE;
						break;
						case MENU_LIST_LENGTH_03:
							*pGUIArrowState = MENU_LIST_LENGTH_07;
							*pGUIState = DRAW_MENU_QUOTE;
						break;
						case MENU_LIST_LENGTH_04:
							*pGUIArrowState = MENU_LIST_LENGTH_07;
							*pGUIState = DRAW_MENU_SYSTEM_SETTINGS;
						break;
						default:
						break;
					}
				break;
				case DRAW_MENU_MICROCLIMATE:
					*pGUIArrowState = MENU_LIST_LENGTH_02;
					*pGUIState = DRAW_MENU_ROOT;
				break;
				case DRAW_MENU_SYSTEM_SETTINGS:
					switch (*pGUIArrowState){
						case MENU_LIST_LENGTH_02:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
							if(pSystemFlags->GUI_OutputData == TRUE){
								pSystemFlags->GUI_OutputData = FALSE;
							}else{
								pSystemFlags->GUI_OutputData = TRUE;
							}
							
							*pGUIState = DRAW_MENU_SYSTEM_SETTINGS;
						break;
						case MENU_LIST_LENGTH_07:
							*pGUIArrowState = MENU_LIST_LENGTH_02;
							*pGUIState = DRAW_MENU_ROOT;
						break;
						default:
						break;
					}
				break;
				case DRAW_MENU_QUOTE:
				
					*pGUIArrowState = MENU_LIST_LENGTH_02;
					*pGUIState = DRAW_MENU_ROOT;
				break;
				default:
				break;
			}
			GUI_DrawMenu(*pGUIState, pSystemFlags);
		}
		
		pSystemFlags -> GUI_InputEvent = FALSE;
	}
}
void GUI_DrawMenu(GUI_DRAW_MENU pDrawMenu, Flags *pSystemFlags){
	switch (pDrawMenu){
		case DRAW_MENU_ROOT:
			SSD1306_ClearScreen();
			MY_GUI_DrawString(10, 2, "Room Microclimate");
			MY_GUI_DrawString(10, 3, "Quote of Day");
			MY_GUI_DrawString(10, 4, "System Settings");
			MY_GUI_DrawString(10, 7, "GUI Version V-1.0");
			SSD1306_UpdateScreen();
		break;
		case DRAW_MENU_MICROCLIMATE:
			SSD1306_ClearScreen();

			MY_GUI_DrawString(10, 7, "BACK");
			SSD1306_UpdateScreen();
		break;
		case DRAW_MENU_QUOTE:
			SSD1306_ClearScreen();
			MY_GUI_DrawString(10, 3, "Create your best");
			MY_GUI_DrawString(10, 4, "knowledge library!");
			MY_GUI_DrawString(10, 7, "BACK");
			SSD1306_UpdateScreen();
		break;
		case DRAW_MENU_SYSTEM_SETTINGS:
			SSD1306_ClearScreen();
			uint8_t *settingString = "Output data: 0";
			settingString[13] = pSystemFlags->GUI_OutputData + 48;
			MY_GUI_DrawString(10, 2, settingString);
			MY_GUI_DrawString(10, 7, "BACK");
			SSD1306_UpdateScreen();
		break;
		default:
		break;
	}
}

void GUI_DrawArrow(GUI_DRAW_ARROW pGUIArrowState){
	for (uint8_t i = 0; i < 8; i++){
		MY_GUI_DrawString(0, i, " ");
	}
	switch (pGUIArrowState){
		case MENU_LIST_LENGTH_00:
			MY_GUI_DrawString(0, 0, ">");
		break;
		case MENU_LIST_LENGTH_01:
			MY_GUI_DrawString(0, 1, ">");
		break;
		case MENU_LIST_LENGTH_02:
			MY_GUI_DrawString(0, 2, ">");
		break;
		case MENU_LIST_LENGTH_03:
			MY_GUI_DrawString(0, 3, ">");
		break;
		case MENU_LIST_LENGTH_04:
			MY_GUI_DrawString(0, 4, ">");
		break;
		case MENU_LIST_LENGTH_05:
			MY_GUI_DrawString(0, 5, ">");
		break;
		case MENU_LIST_LENGTH_06:
			MY_GUI_DrawString(0, 6, ">");
		break;
		case MENU_LIST_LENGTH_07:
			MY_GUI_DrawString(0, 7, ">");
		break;
		default:
		break;
	}
	SSD1306_UpdateScreen();
}

void GUI_DrawSysTime(Time_t TimeMainClock){
	uint8_t *timeString = "Sys Time 00:00:00";
	timeString[9] = TimeMainClock.hours/10 + 48; 
	timeString[10] = TimeMainClock.hours%10 + 48;
	
	timeString[12] = TimeMainClock.minutes/10 + 48;
	timeString[13] = TimeMainClock.minutes%10 + 48;
	
	timeString[15] = TimeMainClock.seconds/10 + 48;
	timeString[16] = TimeMainClock.seconds%10 + 48;
	
	MY_GUI_DrawString(10, 0, timeString);
	SSD1306_UpdateScreen();
}

void GUI_DrawEnv(Enviroment_t BME280Env){
	uint8_t *temperatureString = "Temp 00.00 C";
	uint8_t *pressureString = "Pres 0000.0 hPa";
	
	temperatureString[5] = BME280Env.T / 1000 + 48;
	temperatureString[6] = (BME280Env.T % 1000) / 100 + 48;
	temperatureString[8] = (BME280Env.T % 100) / 10 + 48;
	temperatureString[9] = BME280Env.T % 10 + 48;
	MY_GUI_DrawString(10, 2, temperatureString);
	
	
	pressureString[5] = BME280Env.P / 10000 + 48;
	pressureString[6] = (BME280Env.P % 10000) / 1000 + 48;
	pressureString[7] = (BME280Env.P % 1000) / 100 + 48;
	pressureString[8] = (BME280Env.P % 100) / 10 + 48;
	pressureString[10] = BME280Env.P % 10 + 48;
	MY_GUI_DrawString(10, 3, pressureString);
}

void GUI_DrawEnvOutput(Enviroment_t BME280Env, Time_t TimeMainClock){
	uint8_t *packetString = "[00:00:00]|T:00.00C|P:0000.0hPa\n";
	
	packetString[1] = TimeMainClock.hours/10 + 48;
	packetString[2] = TimeMainClock.hours%10 + 48;
	packetString[4] = TimeMainClock.minutes/10 + 48;
	packetString[5] = TimeMainClock.minutes%10 + 48;
	packetString[7] = TimeMainClock.seconds/10 + 48;
	packetString[8] = TimeMainClock.seconds%10 + 48;
	packetString[13] = BME280Env.T / 1000 + 48;
	packetString[14] = (BME280Env.T % 1000) / 100 + 48;
	packetString[16] = (BME280Env.T % 100) / 10 + 48;
	packetString[17] = BME280Env.T % 10 + 48;
	packetString[22] = BME280Env.P / 10000 + 48;
	packetString[23] = (BME280Env.P % 10000) / 1000 + 48;
	packetString[24] = (BME280Env.P % 1000) / 100 + 48;
	packetString[25] = (BME280Env.P % 100) / 10 + 48;
	packetString[27] = BME280Env.P % 10 + 48;
	
	USART_Transmit_String(packetString);
}