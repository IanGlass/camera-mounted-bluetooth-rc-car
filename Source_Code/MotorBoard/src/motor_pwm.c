/*
 * motor_pwm.c
 *
 *  Created on: May 22, 2013
 *      Author: nbb23
 */

#include "motor_pwm.h"
#include "uart.h"
#include <stdio.h>

int forwardMode = 1;
extern char outstr[200];
// TODO: Needs to be updated to use two different TIMs

void motor_pwm_init() {

}

void motor_pwm_config(int period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	uint16_t PrescalerValue = 0;
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 2800000) - 1;


	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* GPIOA Configuration: TIM5 CH2 (PA1) and TIM2 CH3 (PA2) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM5 pins to AF2 and TIM2 pins to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

	/* Time base configuration - TIM5 */
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	/* PWM1 Mode configuration: Channel2 - TIM5 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5, ENABLE);
	/* TIM5 enable counter */
	TIM_Cmd(TIM5, ENABLE);


	/* TIM2 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* GPIOA Configuration: TIM5 CH2 (PA1) and TIM2 CH3 (PA2) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM5 pins to AF2 and TIM2 pins to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);

	/* Time base configuration - TIM2 */
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
	/* PWM1 Mode configuration: Channel3 - TIM2*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM9, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM9, ENABLE);
}

void motor_pwm_set_dc(uint16_t channel,uint16_t dutycycle)
{
	switch (channel) {
		case 2:
			TIM5->CCR2 = dutycycle;
		case 3:
			TIM9->CCR1 = dutycycle;
	}
}

void motor_set_angle(int channel, int angle){ //-50% is 1.25, 100% is 2
	int upTime=1500+angle*5; //us, micro seconds
	motor_pwm_set_dc(channel,(PERIOD_100HZ*(10000-upTime))/10000);
}

void motor_set_speed(int channel, int power) {
	int upTime=1500+power*5; //us, micro seconds
	motor_pwm_set_dc(channel,(PERIOD_100HZ*(10000-upTime))/10000);
}

void motor_back_speed(int power) {
	if (forwardMode) {
		motor_set_speed(2, 50);
		timer_ms_delay(30);
		motor_set_speed(2, 0);
		timer_ms_delay(20);
	}
	motor_set_speed(2, power/10);
	sprintf(outstr, "Back: %i%% Power         \n", power);
	uart_send(outstr);
	forwardMode=0;
}

void motor_forward_speed(int power) {
	motor_set_speed(2, -power);
	sprintf(outstr, "Forward: %i%% Power         \n", power);
	uart_send(outstr);
	forwardMode=1;
}
