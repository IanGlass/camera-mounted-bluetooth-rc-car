/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_i2c.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_rcc.h"

#include <stdio.h>

#include "main.h"
#include "board.h"
#include "uart.h"
#include "led.h"
#include "button.h"
#include "timer.h"
#include "camera_pwm.h"
#include "standby.h"
#include "uart.h"
#include "i2c.h"
#include "debug_gpio.h"
#include "camera_configuration.h"

/* Global Variables */
long unsigned int sysTick=0;
long int batteryVoltage;
long int emfVoltage;
long int bemfVoltage;


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/



int main(void)
{
	flash_led_init();
	flash_led_left_on();
	flash_led_right_on();

	init_all();

	//led_on(STATUS_LED_STANDBY);
	while (1);
}

void init_all()
{
	char i2cBuff0[2];

	led_init();
	led_on(STATUS_LED_ON);

	timer_init();
	standby_init();
	//uart_init();

	camera_configuration_init();
	camera_configuration_reset();
	camera_configuration_interupt_init();

	i2c_init_AS_MASTER();
	timer_ms_delay(1000);

	sprintf(i2cBuff0,"%c%c",2,128);
	i2c_master_send_buffer(0x78,i2cBuff0,2);
	timer_ms_delay(10);
	sprintf(i2cBuff0,"%c%c",3,35);
	i2c_master_send_buffer(0x78,i2cBuff0,2);
	timer_ms_delay(10);
	sprintf(i2cBuff0,"%c%c",30,72);
	i2c_master_send_buffer(0x78,i2cBuff0,2);

	timer_ms_delay(1000);

	i2c_init();
}


