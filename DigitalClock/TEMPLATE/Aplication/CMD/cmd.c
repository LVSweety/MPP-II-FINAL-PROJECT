/*
 * cmd.c
 *
 * Created: 13/04/2023 11:36:28
 *  Author: Sweets
 */ 
#include "cmd.h"

CMD_StatusCode_t MY_GUI_CMDCheck(Flags *pSystemFlags, char *pCMD_ReadBuffer){
	if(pSystemFlags -> GUI_InputEvent == FALSE){
		if(strcmp(pCMD_ReadBuffer, "<5UP>") == 0){
				
			pSystemFlags -> GUI_CMD_UP = TRUE;
			pSystemFlags -> GUI_InputEvent = TRUE;
			return CMD_STATUS_CODE_OLED_GUI_UP;
				
		}else if(strcmp(pCMD_ReadBuffer, "<5DOWN>") == 0){
				
			pSystemFlags -> GUI_CMD_DOWN = TRUE;
			pSystemFlags -> GUI_InputEvent = TRUE;
			return CMD_STATUS_CODE_OLED_GUI_DOWN;
				
		}else if(strcmp(pCMD_ReadBuffer, "<5ENTER>") == 0){
				
			pSystemFlags -> GUI_CMD_ENTER = TRUE;
			pSystemFlags -> GUI_InputEvent = TRUE;
			return CMD_STATUS_CODE_OLED_GUI_ENTER;
				
		}
	}
	return CMD_STATUS_CODE_ERROR;
}
