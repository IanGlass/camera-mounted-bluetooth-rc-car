/*
 * motor_pwm.h
 *
 *  Created on: May 22, 2013
 *      Author: nbb23
 */

#ifndef MOTOR_PWM_H_
#define MOTOR_PWM_H_

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include "timer.h"

#define PERIOD_100HZ 5375

void camera_pwm_init();
void camera_pwm_config(int period);
void camera_pwm_set_dc(uint16_t dutycycle);



#endif /* MOTOR_PWM_H_ */
