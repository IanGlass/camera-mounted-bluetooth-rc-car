/*
 * button.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */
#include "button.h"

extern long unsigned int sysTick;
//TODO: REG1 and REG2 EN/DIS able. CPU Standby

void button_init(void){
	//Initiate Button
	RCC_AHB1PeriphClockCmd(BUTTON_PORT_RCC, ENABLE);
	RCC_AHB1PeriphClockLPModeCmd(BUTTON_PORT_RCC, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(BUTTON_PORT,&GPIO_InitStructure);

}


uint8_t button_get(void){
	return !(GPIO_ReadInputDataBit(BUTTON_PORT,BUTTON_PIN));
}




