/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stdio.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include "led.h"
#include "timer.h"
#include "button.h"
#include "standby.h"
#include "i2c.h"
#include "uart.h"
#include "debug_gpio.h"
#include "motor_pwm.h"
#include "motor_adc.h"

//ADC variables:
long int batteryVoltage=0;
long int emfVoltage=0;
long int bemfVoltage=0;

//I2C variables:
extern short int transmissionMode;
long unsigned int sysTick=0;
extern long unsigned int i2cErrorCount;
extern long unsigned int startCount;
extern long unsigned int dataCount;
extern long unsigned int stopCount;
char inBuffer[21];
char outBuffer[21];

//Print variable
extern char outstr[200];

//Motor variable
extern int forwardMode;

//Flags
extern long int speedFlag;
extern long int speedValue;
extern long int angleFlag;
extern long int angleValue;

int main(void)
{
    int i = 0;
    //int current_adc_value = 0;

    SystemInit();
    debug_gpio_init();
    led_init();
    timer_init();
    standby_init();
    //button_init(); // Don't put this in if using standby_init
    uart_init();
    i2c_init();
    motor_pwm_init();
    motor_adc_init();

    motor_pwm_config(PERIOD_100HZ);
    led_on(STATUS_LED_ON);


    inBuffer[21] = '\0';
    for (i=0; i<20; i++){
    	inBuffer[i] = 'z';
    }

    outBuffer[21] = '\0';
    for (i=0; i<10; i++){
    	outBuffer[i] = 'A'+(char)i;
    }
    outBuffer[i] = '\0';

    sprintf(outstr, "                                                                              ");
    uart_send(outstr);
    //debug_gpio_on(DEBUG_GPIO_4);

    /*motor_pwm_config(10000); //50Hz
    motor_pwm_set_dc(2,5000);
    motor_pwm_set_dc(3,5000);*/

    print_battery_voltage();

    while (1)
    {
    	//debug_gpio_toggle(DEBUG_GPIO_3);
    	/*set_bat_str();
    	i2c_transfer_report();
    	uart_send("\n");

    	set_speed_read_str();
    	i2c_transfer_report();
    	uart_send("\n");

    	set_speed_write_str();
    	i2c_transfer_report();
    	uart_send("\n");

    	set_angle_read_str();
    	i2c_transfer_report();
    	uart_send("\n");

    	set_angle_write_str();
    	i2c_transfer_report();
    	uart_send("\n");*/



    	/*motor_set_angle(3, 100);
    	timer_ms_delay(50);
    	motor_set_angle(3, 0);
    	timer_ms_delay(50);
    	motor_set_angle(3, -100);
    	timer_ms_delay(50);*/
    	if (speedFlag){
    		sprintf(outstr, "\n%i\n", speedValue);
    		uart_send(outstr);
    		if (speedValue>=0) {
    			motor_forward_speed(speedValue);
    		}
    		if (speedValue<0) {
    			motor_back_speed(-speedValue);
    		}
    		speedFlag=0;
    	}
    	if (angleFlag){
    		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
    		sprintf(outstr, "\n%i\n", angleValue);
    		uart_send(outstr);
    		motor_set_angle(3, angleValue);
    		angleFlag=0;
    		//timer_ms_delay(200);
    		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM12);
    	}
    	//sprintf(outstr, "\nANGLE: %i\n", angleValue);
    	//uart_send(outstr);
    	//motor_set_angle(3, angleValue);

    	i2c_transfer_report();
    	//uart_send("     \n");
    	/*uart_send("\n");
    	print_battery_voltage();
    	uart_send("\n");
    	print_emf_voltage();
    	uart_send("\n");*/
    	//print_bemf_voltage();
    }
}
