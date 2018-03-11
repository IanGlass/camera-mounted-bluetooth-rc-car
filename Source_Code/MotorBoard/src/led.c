/*
 * led.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */
#include "led.h"


void led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;


	//Initiate Status LEDs
	RCC_AHB1PeriphClockCmd(STATUS_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = STATUS_LED_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(STATUS_PORT,&GPIO_InitStructure);



	//Initiate Debug LEDs
	RCC_AHB1PeriphClockCmd(DEBUG_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DEBUG_LED_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(DEBUG_PORT,&GPIO_InitStructure);

}

void led_on(unsigned int led_mask){
	if( (led_mask) & (STATUS_LED_MASK) ){
		GPIO_SetBits(STATUS_PORT,led_mask);
	}
	else if( (led_mask) & (DEBUG_LED_MASK) ){
		GPIO_SetBits(DEBUG_PORT,led_mask);
	}
}


void led_off(unsigned int led_mask){
	if( (led_mask) & (STATUS_LED_MASK) ){
		GPIO_ResetBits(STATUS_PORT,led_mask);
	}
	else if( (led_mask) & (DEBUG_LED_MASK) ){
		GPIO_ResetBits(DEBUG_PORT,led_mask);
	}
}


void led_toggle(unsigned int led_mask){
	if( (led_mask) & (STATUS_LED_MASK) ){
		GPIO_ToggleBits(STATUS_PORT,led_mask);
	}
	else if( (led_mask) & (DEBUG_LED_MASK) ){
		GPIO_ToggleBits(DEBUG_PORT,led_mask);
	}
}



















