/*
 * standby.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#ifndef STANDBY_H_
#define STANDBY_H_

#include <stm32f4xx_pwr.h>
#include "led.h"
#include "button.h"
#include "timer.h"


#define ONLINE 1
#define STANDBY 0

#define STANDBY_INT_SOURCE  EXTI_PortSourceGPIOA
#define STANDBY_INT_PIN     EXTI_PinSource0
#define STANDBY_INT_LINE    EXTI_Line0


void standby_init(void);
void EXTI0_IRQHandler(void);


#endif /* STANDBY_H_ */
