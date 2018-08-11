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

void motor_pwm_init();
void motor_pwm_config(int period);
void motor_pwm_set_dc(uint16_t channel,uint16_t dutycycle);
void motor_set_angle(int channel, int angle);

void motor_set_speed(int channel, int power);
void motor_back_speed(int power);
void motor_forward_speed(int power);

#endif /* MOTOR_PWM_H_ */
