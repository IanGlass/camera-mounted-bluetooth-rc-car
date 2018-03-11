/*
 * led.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#ifndef LED_H_
#define LED_H_

#include <stm32f4xx_gpio.h>
#include "board.h"


#ifdef BOARD_CAMERA
#define DEBUG_PORT         GPIOA
#define DEBUG_PORT_RCC     RCC_AHB1Periph_GPIOA
#define DEBUG_LED_0        GPIO_Pin_6
#define DEBUG_LED_1        GPIO_Pin_7
#define DEBUG_LED_2        GPIO_Pin_4
#define DEBUG_LED_3        GPIO_Pin_5
#endif

#ifdef BOARD_MOTOR
#define DEBUG_PORT         GPIOC
#define DEBUG_PORT_RCC     RCC_AHB1Periph_GPIOC
#define DEBUG_LED_0        GPIO_Pin_1
#define DEBUG_LED_1        GPIO_Pin_0
#define DEBUG_LED_2        GPIO_Pin_3
#define DEBUG_LED_3        GPIO_Pin_2
#endif

#ifdef BOARD_COMMS
#define DEBUG_PORT         GPIOC
#define DEBUG_PORT_RCC     RCC_AHB1Periph_GPIOC
#define DEBUG_LED_0        GPIO_Pin_0
#define DEBUG_LED_1        GPIO_Pin_1
#define DEBUG_LED_2        GPIO_Pin_2
#define DEBUG_LED_3        GPIO_Pin_3
#endif

#define DEBUG_LED_MASK ( (DEBUG_LED_0)|(DEBUG_LED_1)|(DEBUG_LED_2)|(DEBUG_LED_3) )


#ifdef BOARD_CAMERA
#define STATUS_PORT        GPIOB
#define STATUS_PORT_RCC    RCC_AHB1Periph_GPIOB
#define STATUS_LED_STANDBY GPIO_Pin_1
#define STATUS_LED_ON      GPIO_Pin_2
#endif

#ifdef BOARD_MOTOR
#define STATUS_PORT        GPIOC
#define STATUS_PORT_RCC    RCC_AHB1Periph_GPIOC
#define STATUS_LED_STANDBY GPIO_Pin_15
#define STATUS_LED_ON      GPIO_Pin_5
#endif

#ifdef BOARD_COMMS
#define STATUS_PORT        GPIOC
#define STATUS_PORT_RCC    RCC_AHB1Periph_GPIOC
#define STATUS_LED_STANDBY GPIO_Pin_15
#define STATUS_LED_ON      GPIO_Pin_14
#endif

#define STATUS_LED_MASK ( (STATUS_LED_STANDBY)|(STATUS_LED_ON) )


void led_init(void);
void led_on(unsigned int led_mask);
void led_off(unsigned int led_mask);
void led_toggle(unsigned int led_mask);


#endif /* CAMERA_LED_H_ */


