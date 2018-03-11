/*
 * camera_image.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28
 */

#include "camera_image.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

int pixel[8] = {0};



/*void sync_handler (void) {
	//Edge detect to see if Vsync or Hsync was triggered
	//If Hsync edge increment row


	//If Vsync edge increment frame






}*/


/*void D_CLK_handler (void) {
	Detection for D_CLK (since RGB values, Vsync and D_CLK are different) and data bit get
}*/

void camera_enable(void) {
	//Turn on PD2 to enable V_REG 2.85V
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

/*void camera_disable(void) }
	//Turn off PD2 to disable V_REG 2.85V
	GPIO_WriteBit(GPIOD, GPIO_Pin_2, 0);
}*/

void camera_camera_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	//For CAM ENABLE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_InitStructure);

	camera_enable();

	//Enable port C and A and set Data pins (PC12, PA10, PA9, PA8, PC9, PC8, PC7, PC6) as inputs
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9| GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_9| GPIO_Pin_8| GPIO_Pin_7| GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//Set PA15 (CAM DCLK) as inputs
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//Set PB13 and PB12 (CAM SDA, CAM SLK) as outputs

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//Instantiate PC11 and PC10 (Vsync, Hsync) interrupts











	//Set PB15 (CAM EXTCLK) as outputs on PWM peripheral
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB,&GPIO_InitStructure);


	/* TIM12 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	/* Compute the prescaler value */
    u32 PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel2 (GPIOB Pin 15)*/
    TIM_OC2Init(TIM12, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);

    TIM_Cmd(TIM12, ENABLE);
    //



}

/*void camera_start(void) {
	//Send 0x02 to start camera transmitting
}*/

