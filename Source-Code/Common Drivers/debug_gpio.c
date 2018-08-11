/*
 * debug_gpio.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */
#include "debug_gpio.h"

void debug_gpio_init(void)
{
		//Initiate Debug GPIO
		GPIO_InitTypeDef  GPIO_InitStructure;

		//Initiate on Port A
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin = DEBUG_GPIO_A_MASK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOA,&GPIO_InitStructure);

		//Initiate on Port B
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = DEBUG_GPIO_B_MASK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOB,&GPIO_InitStructure);

		//Initiate on Port C
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_InitStructure.GPIO_Pin = DEBUG_GPIO_C_MASK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_Init(GPIOC,&GPIO_InitStructure);

}

void debug_gpio_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	GPIO_SetBits(GPIOx,GPIO_Pin);
}


void debug_gpio_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	GPIO_ResetBits(GPIOx,GPIO_Pin);
}


void debug_gpio_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	GPIO_ToggleBits(GPIOx,GPIO_Pin);
}


