#include "main.h"
//PRIVATE TYPEDEF		>

//PRIVATE DEFINE		>

//VARIABLES				>
Enviroment_t BME280Env;

Time_t TimeMainClock;
Time_t TimeMainCountdown;
Time_t TimeMainCronometer;
Time_t TimeMainAlaram;
Time_t TemperatureK;
Time_t TimeMainLap[LAP_BUFFER_SIZE];

uint8_t myDisplayClock			[8] = {[0 ... 7] = D0};
uint8_t myDisplayCountdown		[8] = {[0 ... 7] = D0};
uint8_t myDisplayChronometer	[8] = {[0 ... 7] = D0};
uint8_t myDisplayAlarm			[8] = {[0 ... 7] = D0};
uint8_t myDisplayTemperature	[8] = {[0 ... 7] = D0};
uint8_t myDisplayLap			[8] = {[0 ... 7] = D0};

uint8_t BCD [10] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, D9};
	
char USART_Buffer[USART_BUFFER_SIZE];
uint8_t USART0_Flag = 0;
uint8_t lapBuffer = 0;
CMD_StatusCode_t USER_CMD_state, MY_Display_state = CMD_STATUS_CODE_DISPLAY_NONE;
uint8_t myChar = 0;

uint8_t loginAttempts = 0;

int8_t USER_Sellector = 0;
Flags systemFlags;

uint16_t ADCreadout = 0;
uint16_t activeResistance = 0;
double temperature = 0;

//OLED GUI
GUI_DRAW_MENU	MY_MENU_state = DRAW_MENU_ROOT;
GUI_DRAW_ARROW	MY_MENU_ARROW_state = MENU_LIST_LENGTH_02;

uint16_t ADCController = 0;

//FUNCION PROTOTYPES	>
void USER_Clock(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD);
void USER_Chronometer(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD);
void USER_Countdown(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD);

void USER_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t mSeconds);
void USER_SetAlarm(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds);
void USER_SetChronometer(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds);
void USER_SetCountdown(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds);

void USER_Loggin();

//CMD_StatusCode_t MY_GUI_CMD_Check(Flags *pSystemFlags, char *pCMD_ReadBuffer);
CMD_StatusCode_t USER_CMD_Switch(char *pBufferCMD);
	
int main(void){
	//INIT MCU			>
	MY_GPIO_Init();
	MY_USART_Init();
	sei();
	
	_delay_ms(500);
	//USER_Loggin(); //There is a bug with the buffer.
	MY_Display_state = CMD_STATUS_CODE_DISPLAY_CLOCK;
	
	MY_EXTI_Init();
	MY_ADC_Init();
	
	MY_TIM0_Init();
	MY_TIM1_Init();
	MY_TIM2_Init();
	I2C_Init();

	//BME280_Init(0xEC);
	
	//_delay_ms(10);
	//BME280_CompensationData(0xEC);
	
	TimeMainAlaram.seconds = 30;
	BinaryCodedDecimal(&TimeMainAlaram, myDisplayAlarm, BCD);
	
	BMP280_Init();
	
	SSD1306_Init();
	SSD1306_ClearScreen();
	GUI_DrawMenu(MY_MENU_state, &systemFlags);
	GUI_DrawArrow(MY_MENU_ARROW_state);
	
	while (1) {
		if(USART0_Flag == 1){
			USER_CMD_state = USER_CMD_Switch(USART_Buffer);
			FSM_GUI_State(&systemFlags, &MY_MENU_state, &MY_MENU_ARROW_state);
			GUI_DrawSysTime(TimeMainClock);
			GUI_DrawArrow(MY_MENU_ARROW_state);
			
			USART0_Flag = 0;
		}
		
		/*if(systemFlags.GUI_InputEvent == FALSE){
			ADCController = Analog_Read(ADC_CHANNEL_3);
			if(ADCController > 900 || ADCController < 200){
				if(ADCController >= 455){
					systemFlags.GUI_CMD_DOWN = TRUE;
					systemFlags.GUI_InputEvent = TRUE;
					}else{
					systemFlags.GUI_CMD_UP = TRUE;
					systemFlags.GUI_InputEvent = TRUE;
				}
				FSM_GUI_State(&systemFlags, &MY_MENU_state, &MY_MENU_ARROW_state);
				GUI_DrawSysTime(TimeMainClock);
				GUI_DrawArrow(MY_MENU_ARROW_state);
			}
		}*/
		
		if(MY_Display_state == CMD_STATUS_CODE_DISPLAY_TEMPERATURE){
			ADCreadout = Analog_Read(ADC_CHANNEL_2);
			activeResistance = (ADCreadout*10)/(1024-ADCreadout);
			temperature = (double)1/(((double)1/(double)298) + ((double)1/(double)3950) * log((double)activeResistance/(double)10));
			
			TemperatureK.hours		= ((int16_t)temperature / 100);
			TemperatureK.minutes	= ((int16_t)temperature) - (TemperatureK.hours * 100);
			TemperatureK.seconds	= (int16_t)((temperature - (TemperatureK.hours * 100) - TemperatureK.minutes) / (double)0.01);
			TemperatureK.miliseconds= (int16_t)((temperature - ((double)TemperatureK.hours * 100) - (double)TemperatureK.minutes - (double)TemperatureK.seconds*0.01) / (double)0.0001);
		
			BinaryCodedDecimal(&TemperatureK, myDisplayTemperature, BCD);
			_delay_ms(200);
		}
		
		if(MY_Display_state == CMD_STATUS_CODE_DISPLAY_LAP){
			
			MY_Display_state = CMD_STATUS_CODE_DISPLAY_LAP;
			_delay_ms(500);
			
			if (lapBuffer + 1 > LAP_BUFFER_SIZE){
				lapBuffer = 0;
			}
			TimeMainLap[lapBuffer].hours = (lapBuffer+1)*10;
			BinaryCodedDecimal(&TimeMainLap[lapBuffer], myDisplayLap, BCD);
			
			lapBuffer++;
		}
		
		if(MY_Display_state == CMD_STATUS_CODE_DISPLAY_CLOCK && TimeMainClock.hours == TimeMainAlaram.hours && TimeMainClock.minutes == TimeMainAlaram.minutes && TimeMainClock.seconds == TimeMainAlaram.seconds){
			for(int i = 0; i < 3; i++){
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_ALARM;
				_delay_ms(500);
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_NONE;
				_delay_ms(500);
			}
			MY_Display_state = CMD_STATUS_CODE_DISPLAY_CLOCK;
		}
    }
}

void USER_Loggin(){
	while (1){
		if(loginAttempts < 3 && USART0_Flag == 1){
			if(strcmp(USART_Buffer, "<PASS>") == 0){ //FIX LATER
				USART_Transmit_String("PASSWORD ACCEPTED!\n");
				USART0_Flag = 0;
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_CLOCK;
				return;
			}else{
				loginAttempts++;
			}
		}
	}
	return;
}

void USER_Clock(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD){
	TimeClock(pTime);
	BinaryCodedDecimal(pTime, pClockArr, pBCD);
}

void USER_Chronometer(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD){
	TimeChronometer(pTime);
	BinaryCodedDecimal(pTime, pClockArr, pBCD);
}

void USER_Countdown(Time_t *pTime, uint8_t *pClockArr, uint8_t *pBCD){
	TimeCountdown(pTime);
	BinaryCodedDecimal(pTime, pClockArr, pBCD);
}

void USER_SetClock(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds){
	TimeMainClock.hours = hours;
	TimeMainClock.minutes = minutes;
	TimeMainClock.seconds = seconds;
	TimeMainClock.miliseconds = mSeconds;
}

void USER_SetAlarm(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds){
	TimeMainAlaram.hours = hours;
	TimeMainAlaram.minutes = minutes;
	TimeMainAlaram.seconds = seconds;
	TimeMainAlaram.miliseconds = mSeconds;
}

void USER_SetChronometer(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds){
	TimeMainCronometer.hours = hours;
	TimeMainCronometer.minutes = minutes;
	TimeMainCronometer.seconds = seconds;
	TimeMainCronometer.miliseconds = mSeconds;
}

void USER_SetCountdown(uint8_t hours,uint8_t minutes, uint8_t seconds, uint8_t mSeconds){
	TimeMainCountdown.hours = hours;
	TimeMainCountdown.minutes = minutes;
	TimeMainCountdown.seconds = seconds;
	TimeMainCountdown.miliseconds = mSeconds;
}

CMD_StatusCode_t USER_CMD_Switch(char *pBufferCMD){
	switch(pBufferCMD[1]){
		/* System Configuration */
		case '0': // <0commanda>
			if(strcmp(pBufferCMD, "<00>") == 0){
				//MY_BufferClear(USART_Buffer, USART_BUFFER_SIZE);
				//MY_BufferClear(CMD_Buffer, CMD_BUFFER_SIZE);
				USART_Transmit_String("Clearing USART buffer\n");
				return CMD_STATUS_CODE_BUFFER_CLEAR;
			}
		break;
		/* Clock Configuration */
		case '1':
			if(strcmp(pBufferCMD, "<10>") == 0){
				/* Clock CLEAR  */
				USER_SetClock(0, 0, 0, 0);
				USART_Transmit_String("Clearing clock\n");
				return CMD_STATUS_CODE_CLOCK_CLEAR;
			
			}else if(strcmp(pBufferCMD, "<11>") == 0){
				/* Clock STOP  */
				MY_TIMX_Stop(TIM1_STOP);
				USART_Transmit_String("Stopping clock\n");
				return CMD_STATUS_CODE_CLOCK_STOP;
			
			}else if(strcmp(pBufferCMD, "<12>") == 0){
				/* Clock START */
				USART_Transmit_String("Starting clock\n");
				MY_TIMX_Start(TIM1_START, TIM1_PRESCALING_FACTOR_1024);
				return CMD_STATUS_CODE_CLOCK_START;
			
			}else if ((pBufferCMD [1] == '1') && (pBufferCMD [2] == '3')){
				/* Clock SET */
				USART_Transmit_String("Setting clock\n");
				uint8_t hours	= (((pBufferCMD [3] - '0') * 10) + (pBufferCMD [4] - '0'));
				uint8_t minutes = (((pBufferCMD [5] - '0') * 10) + (pBufferCMD [6] - '0'));
				uint8_t seconds = (((pBufferCMD [7] - '0') * 10) + (pBufferCMD [8] - '0'));
				USER_SetClock(hours, minutes, seconds, 0);
				return CMD_STATUS_CODE_CLOCK_SET;
			
			}else if ((pBufferCMD [1] == '1') && (pBufferCMD [2] == '4')){
				/* Alarm SET */
				USART_Transmit_String("Setting alarm\n");
				uint8_t hours	= (((pBufferCMD [3] - '0') * 10) + (pBufferCMD [4] - '0'));
				uint8_t minutes = (((pBufferCMD [5] - '0') * 10) + (pBufferCMD [6] - '0'));
				uint8_t seconds = (((pBufferCMD [7] - '0') * 10) + (pBufferCMD [8] - '0'));
				USER_SetAlarm(hours, minutes, seconds, 0);
				BinaryCodedDecimal(&TimeMainAlaram, myDisplayAlarm, BCD);
				return CMD_STATUS_CODE_CLOCK_ALARM_SET;
			
			}
		break;

		/* Countdown Configuration */
		case '3':
			if(strcmp(pBufferCMD, "<30>") == 0){
				/* Countdown CLEAR  */
				USART_Transmit_String("Clearing countdown\n");
				USER_SetClock(0, 0, 0, 0);
				return CMD_STATUS_CODE_COUNTDOWN_CLEAR;
				
			}else if(strcmp(pBufferCMD, "<31>") == 0){
				/* Countdown STOP  */
				USART_Transmit_String("Stopping countdown\n");
				MY_TIMX_Stop(TIM2_STOP);
				return CMD_STATUS_CODE_COUNTDOWN_STOP;
			
			}else if(strcmp(pBufferCMD, "<32>") == 0){
				/* Countdown START */
				USART_Transmit_String("Starting countdown\n");
				MY_TIMX_Start(TIM2_START, TIM2_PRESCALING_FACTOR_1024);
				return CMD_STATUS_CODE_COUNTDOWN_START;
			
			}else if ((pBufferCMD [1] == '3') && (pBufferCMD [2] == '3')){
				/* Countdown SET */
				USART_Transmit_String("Setting countdown\n");
				uint8_t minutes		= (((pBufferCMD [3] - '0') * 10) + (pBufferCMD [4] - '0'));
				uint8_t seconds  	= (((pBufferCMD [5] - '0') * 10) + (pBufferCMD [6] - '0'));
				uint8_t miliseconds	= (((pBufferCMD [7] - '0') * 10) + (pBufferCMD [8] - '0'));
				USER_SetCountdown(0, minutes, seconds, miliseconds);
				return CMD_STATUS_CODE_COUNTDOWN_SET;
			}
		break;

		/* Chronometer Configuration */
		case '2':
			if(strcmp(pBufferCMD, "<20>") == 0){
				/* Chronometer CLEAR  */
				USART_Transmit_String("Clearing chronometer\n");
				USER_SetChronometer(0, 0, 0, 0);
				return CMD_STATUS_CODE_CHRONOMETER_CLEAR;
			
			}else if(strcmp(pBufferCMD, "<21>") == 0){
				/* Chronometer STOP  */
				USART_Transmit_String("Stopping chronometer\n");
				MY_TIMX_Stop(TIM2_STOP);
				return CMD_STATUS_CODE_CHRONOMETER_STOP;
			
			}else if(strcmp(pBufferCMD, "<22>") == 0){
				/* Chronometer START */
				USART_Transmit_String("Starting chronometer\n");
				MY_TIMX_Start(TIM2_START, TIM2_PRESCALING_FACTOR_1024);
				return CMD_STATUS_CODE_CHRONOMETER_START;
			
			}else if(strcmp(pBufferCMD, "<23>") == 0){
				/* Chronometer LAP */
				USART_Transmit_String("Saving a lap\n");
				if (lapBuffer < LAP_BUFFER_SIZE){
					TimeMainLap[lapBuffer].miliseconds = TimeMainCronometer.miliseconds;
					TimeMainLap[lapBuffer].seconds = TimeMainCronometer.seconds;
					TimeMainLap[lapBuffer].minutes = TimeMainCronometer.minutes;
					lapBuffer++;
				}else{
					lapBuffer = 0;
					TimeMainLap[lapBuffer].miliseconds = TimeMainCronometer.miliseconds;
					TimeMainLap[lapBuffer].seconds = TimeMainCronometer.seconds;
					TimeMainLap[lapBuffer].minutes = TimeMainCronometer.minutes;
					lapBuffer++;
				}
				
				return CMD_STATUS_CODE_CHRONOMETER_LAP;
			
			}
		break;

		/* Display Mode */
		case '4':
			if(strcmp(pBufferCMD, "<40>") == 0){
				USART_Transmit_String("DISPLAY:CLOCK\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_CLOCK;
				return CMD_STATUS_CODE_DISPLAY_CLOCK;
			}else if(strcmp(pBufferCMD, "<41>") == 0){
				USART_Transmit_String("DISPLAY:COUNTDOWN\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_COUNTDOWN;
				return CMD_STATUS_CODE_DISPLAY_COUNTDOWN;
			}else if(strcmp(pBufferCMD, "<42>") == 0){
				lapBuffer = 0;
				USART_Transmit_String("DISPLAY:CHRONOMETER\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_CHRONOMETER;
				return CMD_STATUS_CODE_DISPLAY_CHRONOMETER;
			}else if(strcmp(pBufferCMD, "<43>") == 0){
				USART_Transmit_String("DISPLAY:ALARM\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_ALARM;
				return CMD_STATUS_CODE_DISPLAY_ALARM;
			}else if(strcmp(pBufferCMD, "<44>") == 0){
				USART_Transmit_String("DISPLAY:TEMPERATURE\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_TEMPERATURE;
				return CMD_STATUS_CODE_DISPLAY_TEMPERATURE;
			}else if(strcmp(pBufferCMD, "<45>") == 0){
				lapBuffer = 0;
				USART_Transmit_String("DISPLAY:LAP\n");
				MY_Display_state = CMD_STATUS_CODE_DISPLAY_LAP;
				return CMD_STATUS_CODE_DISPLAY_LAP;
			}

		break;
		/*OLED Display*/
		case '5':
			return MY_GUI_CMDCheck(&systemFlags, pBufferCMD);
		break;
		case 'M':
			if(strcmp(pBufferCMD, "<MYACPCH2>") == 0){
				GUI_DrawVoltageOutput(Analog_Read(ADC_CHANNEL_2), TimeMainClock, '2');
			}else if(strcmp(pBufferCMD, "<MYACPCH3>") == 0){
				GUI_DrawVoltageOutput(Analog_Read(ADC_CHANNEL_3), TimeMainClock, '3');
			}
		break;
		USART_Transmit_String("Failed to resolve command\n");
	return CMD_STATUS_CODE_ERROR;
	}
}

//USER_System_EVENT USER_ReadSystemEvent(){
	//if(systemFlags.inputEvent == BOOL_TRUE){
		//if(systemFlags.CMD_DOWN = BOOL_TRUE){
		 	//USER_Sellector++;
			//systemFlags.CMD_DOWN = BOOL_FALSE;
			//systemFlags.inputEvent = BOOL_FALSE;
			//return DISPLAY_EVENT_SELLECTION_DOWN;
		//}
		//if(systemFlags.CMD_DOWN = BOOL_TRUE){
		//	USER_Sellector+;
		//	systemFlags.CMD_DOWN = BOOL_FALSE
		//	systemFlags.inputEvent = BOOL_FALSE
		//	return DISPLAY_EVENT_SELLECTION_DOWN
		//}
		//if(systemFlags.CMD_DOWN = BOOL_TRUE){
		//	USER_Sellector+;
		//	systemFlags.CMD_DOWN = BOOL_FALSE
		//	systemFlags.inputEvent = BOOL_FALSE
		//	return DISPLAY_EVENT_SELLECTION_DOWN
		//}
		//if(systemFlags.CMD_DOWN = BOOL_TRUE){
		//	USER_Sellector+;
		//	systemFlags.CMD_DOWN = BOOL_FALSE
		//	systemFlags.inputEvent = BOOL_FALSE
		//	return DISPLAY_EVENT_SELLECTION_DOWN
		//}
	//}
	//return DISPLAY_EVENT_SELLECTION_NONE;
//}

void MY_BufferClear(){
	
}

ISR(TIMER0_COMPA_vect){
	switch(MY_Display_state){
		case CMD_STATUS_CODE_DISPLAY_CHRONOMETER:
			ShiftRegister(PORTC0, PORTC1, myDisplayChronometer);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_CLOCK:
			ShiftRegister(PORTC0, PORTC1, myDisplayClock);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_COUNTDOWN:
			ShiftRegister(PORTC0, PORTC1, myDisplayCountdown);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_TEMPERATURE:
			ShiftRegister(PORTC0, PORTC1, myDisplayTemperature);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_ALARM:
			ShiftRegister(PORTC0, PORTC1, myDisplayAlarm);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_LAP:
			ShiftRegister(PORTC0, PORTC1, myDisplayLap);
		break;
		
		case CMD_STATUS_CODE_DISPLAY_NONE:
			PORTC ^= (1 << PORTC0);
		break;
		
		default:
			PORTC ^= (1 << PORTC0);
		break;
	}
}

ISR(TIMER1_COMPA_vect){
	USER_Clock(&TimeMainClock, myDisplayClock, BCD);
	GUI_DrawSysTime(TimeMainClock);
	
	if(MY_MENU_state == DRAW_MENU_MICROCLIMATE || systemFlags.GUI_OutputData == TRUE){
		BME280Env.T = BMP280_GetTemperature();
		BME280Env.P = BMP280_GetPressure();
		BME280Env.H = BMP280_GetHumidity();
		if(systemFlags.GUI_OutputData == TRUE){
			GUI_DrawEnvOutput(BME280Env, TimeMainClock);
		}
		if(MY_MENU_state == DRAW_MENU_MICROCLIMATE){
			GUI_DrawEnv(BME280Env);
		}
	}
}

ISR(TIMER2_COMPA_vect){
	switch(MY_Display_state){
		case CMD_STATUS_CODE_DISPLAY_COUNTDOWN:
			USER_Countdown(&TimeMainCountdown, myDisplayCountdown, BCD);
		break;
		case CMD_STATUS_CODE_DISPLAY_CHRONOMETER:
			USER_Chronometer(&TimeMainCronometer, myDisplayChronometer, BCD);
		break;
		default:
		break;
	}
}

ISR(USART_RX_vect){
	static uint8_t myISRCnt = 0;
	USART_Buffer[myISRCnt] = UDR0;
	//Check if first buffer element equals to '<' (0x3c)
	if (USART_Buffer[0] != '<'){
		//Clear USART read buffer
		for (uint8_t i = 0; i < USART_BUFFER_SIZE; i++){
			USART_Buffer[i] = 0;
		}
		myISRCnt= 255;
	}else if (USART_Buffer[myISRCnt] == '>'){
		USART_Buffer[myISRCnt + 1] = '\0';
		myISRCnt = 255;
		USART0_Flag = 1;
		
	}
	myISRCnt++;
}

ISR (INT0_vect){
	PORTB ^= (1 << PORTB5);
	if (MY_Display_state == CMD_STATUS_CODE_DISPLAY_CHRONOMETER){
		USART_Transmit_String("Saving a lap\n");
		if (lapBuffer < LAP_BUFFER_SIZE){
			TimeMainLap[lapBuffer].miliseconds = TimeMainCronometer.miliseconds;
			TimeMainLap[lapBuffer].seconds = TimeMainCronometer.seconds;
			TimeMainLap[lapBuffer].minutes = TimeMainCronometer.minutes;
			lapBuffer++;
		}else{
			USART_Transmit_String("Saving a lap\n");
			lapBuffer = 0;
			TimeMainLap[lapBuffer].miliseconds = TimeMainCronometer.miliseconds;
			TimeMainLap[lapBuffer].seconds = TimeMainCronometer.seconds;
			TimeMainLap[lapBuffer].minutes = TimeMainCronometer.minutes;
			lapBuffer++;
		}
	}
				
}