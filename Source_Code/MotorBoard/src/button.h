/*
 * button.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stm32f4xx_gpio.h>
#include "board.h"

// All groups should have this the same, should be no need to #ifdef this. CHECK bus A pin 0
#define BUTTON_PORT        GPIOA
#define BUTTON_PORT_RCC    RCC_AHB1Periph_GPIOA
#define BUTTON_PIN         GPIO_Pin_0


void button_init(void);
uint8_t button_get(void);


#endif /* BUTTON_H_ */
