/*
 * main.h
 *
 *  Created on: May 18, 2013
 *      Author: Scott
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#endif /* MAIN_H_ */

const uint16_t LEDS = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
const uint16_t LED[4] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3};

void init();
void loop();
void initButton();
void initButtonLED();
void delay();
void initExtInterrupt(void);
void initClkInterrupt();
void switchLed(void);
