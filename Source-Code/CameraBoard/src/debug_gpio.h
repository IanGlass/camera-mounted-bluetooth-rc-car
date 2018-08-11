/*
 * debug_gpio.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#ifndef DEBUG_GPIO_H_
#define DEBUG_GPIO_H_

#include "stm32f4xx_gpio.h"
#include "board.h"


void flash_led_init(void);
void flash_led_left_toggle();
void flash_led_right_toggle();
void flash_led_left_on(void);
void flash_led_right_on(void);
void flash_led_left_off(void);
void flash_led_right_off(void);

#ifdef BOARD_CAMERA
#define DEBUG_GPIO_1 GPIOB,GPIO_Pin_3
#define DEBUG_GPIO_2 GPIOB,GPIO_Pin_4
#define DEBUG_GPIO_3 GPIOB,GPIO_Pin_5
#define DEBUG_GPIO_A_MASK (0)
#define DEBUG_GPIO_B_MASK (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5)
#define DEBUG_GPIO_C_MASK (0)
#endif

#ifdef BOARD_MOTOR
#define DEBUG_GPIO_1 GPIOA,GPIO_Pin_10
//#define DEBUG_GPIO_2 GPIOA,GPIO_Pin_9
#define DEBUG_GPIO_3 GPIOA,GPIO_Pin_8
#define DEBUG_GPIO_4 GPIOC,GPIO_Pin_9
#define DEBUG_GPIO_5 GPIOC,GPIO_Pin_8
#define DEBUG_GPIO_6 GPIOC,GPIO_Pin_7
#define DEBUG_GPIO_7 GPIOC,GPIO_Pin_6
#define DEBUG_GPIO_8 GPIOB,GPIO_Pin_15
#define DEBUG_GPIO_A_MASK (GPIO_Pin_10 | /*GPIO_Pin_9 |*/ GPIO_Pin_8)
#define DEBUG_GPIO_B_MASK (GPIO_Pin_15)
#define DEBUG_GPIO_C_MASK (GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6)
#endif

#ifdef BOARD_COMMS
//NEED DEFINITIONS
#define DEBUG_GPIO_A_MASK (0)
#define DEBUG_GPIO_B_MASK (0)
#define DEBUG_GPIO_C_MASK (0)
#endif

void debug_gpio_init(void);
void debug_gpio_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void debug_gpio_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void debug_gpio_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


#endif /* DEBUG_GPIO_H_ */
