/*
 * cmd.c
 *
 * Created: 13/04/2023 11:36:28
 *  Author: Sweets
 */ 
#include "cmd.h"

CMD_StatusCode_t MY_GUI_CMDCheck(Flags *pSystemFlags, char *pCMD_ReadBuffer){
	if(pSystemFlags -> inputEvent == BOOL_FALSE){
		
		if (strcmp(pCMD_ReadBuffer, "<5UP>") == 0){
			pSystemFlags -> CMD_UP = 1;
			pSystemFlags -> inputEvent = BOOL_TRUE;
			
			return CMD_STATUS_CODE_OLED_GUI_UP;
		}
		if (strcmp(pCMD_ReadBuffer, "<5DOWN>") == 0){
			pSystemFlags -> CMD_UP = 1;
			pSystemFlags -> inputEvent = BOOL_TRUE;
			
			return CMD_STATUS_CODE_OLED_GUI_DOWN;
		}
		if (strcmp(pCMD_ReadBuffer, "<5ENTER>") == 0){
			pSystemFlags -> CMD_UP = 1;
			pSystemFlags -> inputEvent = BOOL_TRUE;
			
			return CMD_STATUS_CODE_OLED_GUI_ENTER;
		}
		
	}
}