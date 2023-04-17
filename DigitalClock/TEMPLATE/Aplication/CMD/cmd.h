/*
 * cmd.h
 *
 * Created: 13/04/2023 11:36:39
 *  Author: Sweets
 */ 


#ifndef CMD
#define CMD

#include "variables.h"
#include <string.h>

CMD_StatusCode_t MY_GUI_CMDCheck(Flags *pSystemFlags, char *pCMD_ReadBuffer);

#endif