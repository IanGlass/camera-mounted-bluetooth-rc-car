/*
 * motor_adc.h
 *
 *  Created on: May 22, 2013
 *      Author: nbb23
 *        Note: Currently using ADC1 for reading the battery value,
 *              and ADC2 for reading the EMF and BEMF values.
 */

#ifndef MOTOR_ADC_H_
#define MOTOR_ADC_H_


#include <stm32f4xx_adc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "timer.h"

#include <stdio.h>
#include "uart.h"
#include "led.h"


#define BAT_SUPPLY_MIN 5400

#define ADC_MODE_BATTERY 0
#define ADC_MODE_EMF 1
#define ADC_MODE_BEMF 2

#define ADC_BAT_CHANNEL ADC_Channel_14
#define ADC_EMF_CHANNEL ADC_Channel_7
#define ADC_BEMF_CHANNEL ADC_Channel_6

#define EMF_OFFSET 0
#define BEMF_OFFSET 0
#define EMF_MULTIPLIER 1
#define BEMF_MULTIPLIER 1

#define BAT_VOLTAGE_DIVIDER 12900/2900
#define VOLTAGE_RES_RATIO 3300/4096

void motor_adc_init(void);
void get_battery_voltage(void); //Outputs the current battery voltage in mV
void get_emf_voltage(void); //Outputs the current EMF voltage in mV (can be -ve)
void get_bemf_voltage(void); //Outputs the current BEMF voltage in mV (can be -ve)

void print_battery_voltage(void);
void print_emf_voltage(void);
void print_bemf_voltage(void);


#endif /* MOTOR_ADC_H_ */
