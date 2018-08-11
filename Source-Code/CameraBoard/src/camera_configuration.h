/*
 * camera_configuration.h
 *
 *  Created on: May 28, 2013
 *      Author: mlk28
 */

#ifndef CAMERA_CONFIGURATION_H_
#define CAMERA_CONFIGURATION_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void camera_configuration_init(void);
void camera_configuration_reset(void);
void camera_configuration_interupt_init(void);
void InitGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
void Attach_GPIO_Interrupt(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex, uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger, uint8_t Priority);

#endif /* CAMERA_CONFIGURATION_H_ */
