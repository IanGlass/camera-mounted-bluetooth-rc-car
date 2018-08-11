/*
 * standby.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#include "standby.h"

unsigned int mcu_state=ONLINE;

void standby_interrupt_init(void);

void standby_init(void){
	button_init();
	standby_interrupt_init();

	//Standby Mode Configuration
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_WakeUpPinCmd(ENABLE);
}

void standby_interrupt_init(void) {
    EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB2PeriphClockLPModeCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(STANDBY_INT_SOURCE,STANDBY_INT_PIN);

    EXTI_InitStructure.EXTI_Line = STANDBY_INT_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}


void EXTI0_IRQHandler(void) {

    if (EXTI_GetITStatus(STANDBY_INT_LINE) != RESET) {
    	EXTI_ClearITPendingBit(STANDBY_INT_LINE);

    	switch(mcu_state){
    		case STANDBY:

    			led_on(STATUS_LED_ON);
    			led_off(STATUS_LED_STANDBY);
    			mcu_state=ONLINE;
    			break;

    		case ONLINE:

    			led_off(DEBUG_LED_MASK);
    			led_off(STATUS_LED_ON);
    			//led_on(STATUS_LED_STANDBY);
    			mcu_state=STANDBY;

    			//SleepMode_Measure();
    			PWR_EnterSTANDBYMode();
    			//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);

    			break;
    	}
    }
}




