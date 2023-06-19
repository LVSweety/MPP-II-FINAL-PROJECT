#ifndef MAIN
#define	MAIN

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <math.h>
#include <avr/eeprom.h>

//#include "atmega328p_CONFIG.h"

#include "variables.h"

#include "clock.h"
#include "segmentDisplay.h"
#include "chronometer.h"
#include "countdown.h"
#include "cmd.h"

#include "BME280.h"
#include "ssd1306.h"

#include "atmega328p_GPIO.h"
#include "atmega328p_TIM.h"
#include "atmega328p_USART.h"
#include "atmega328p_ADC.h"
#include "atmega328p_I2C.h"

#define USART_BUFFER_SIZE 11
#define LAP_BUFFER_SIZE 5
#define MY_EEPROM_ADDRESS 0x10
#endif