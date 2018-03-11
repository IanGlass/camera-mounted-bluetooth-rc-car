/*
 * motor_pwm.c
 *
 *  Created on: May 22, 2013
 *      Author: nbb23
 */

#include "camera_pwm.h"
#include <stdio.h>

void camera_pwm_config(int period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	uint16_t PrescalerValue = 0;
	/* Compute the prescaler value */
	PrescalerValue = 0;//(uint16_t) ((SystemCoreClock /2) / 2800000) - 1;


	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* GPIOA Configuration: TIM5 CH2 (PA1) and TIM2 CH3 (PA2) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect TIM5 pins to AF2 and TIM2 pins to AF1 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);

	/* Time base configuration - TIM5 */
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	/* PWM1 Mode configuration: Channel2 - TIM5 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM12, ENABLE);
	/* TIM5 enable counter */
	TIM_Cmd(TIM12, ENABLE);
}

void camera_pwm_set_dc(uint16_t dutycycle)
{
	TIM12->CCR2 = dutycycle;
	//TIM3->CCR3 = dutycycle;
}
