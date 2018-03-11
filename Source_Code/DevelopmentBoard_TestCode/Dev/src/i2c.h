/*
 * i2c.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28
 */

#ifndef I2C_H_
#define I2C_H_

#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "board.h"


#define CAMERA_ADDRESS 0x00
#define MOTOR_ADDRESS  0x3C
#define COMMS_ADDRESS  0x4C

#define I2C_CLOCK_SPEED 	 	10000   //10kHz

#ifdef BOARD_CAMERA
#define I2C_PERIPHERAL_CLOCK 	RCC_APB1Periph_I2C2
#define GPIO_PERIPHERAL_CLOCK   RCC_AHB1Periph_GPIOB
#define I2C_GPIO_PORT			GPIOB
#define I2C_SCL_PIN			 	GPIO_Pin_10
#define I2C_SDA_PIN			 	GPIO_Pin_11
#define GPIO_AF_SELECTION  		GPIO_AF_I2C2
#define I2C_SEL					I2C2
#define I2C_EVENT_ISR			I2C2_EV_IRQn
#define I2C_ERROR_ISR			I2C2_ER_IRQn
#endif

#ifdef BOARD_MOTOR
#define I2C_PERIPHERAL_CLOCK 	RCC_APB1Periph_I2C2
#define I2C_GPIO_PORT        	RCC_AHB1Periph_GPIOB
#define I2C_SCL_PIN			 	GPIO_Pin_10
#define I2C_SDA_PIN			 	GPIO_Pin_11
#define GPIO_AF_SELECTION  		GPIO_AF_I2C2
#define I2C_SEL					I2C2
#define I2C_EVENT_ISR			I2C2_EV_IRQn
#define I2C_ERROR_ISR			I2C2_ER_IRQn
#endif

#ifdef BOARD_COMMS
//NEED DEFINITIONS
#endif

void i2c_init(void);

void i2c_master_send_buffer(uint8_t slave_adress, uint8_t* txBuffer, unsigned int bufferLen);

void i2c_master_recv_buffer(uint8_t slave_adress, uint8_t* rxBuffer, unsigned int bufferLen);

void i2c_transfer_report(void);

inline void I2C_CleanADDRandSTOPF();

#ifdef SLAVE

void I2C2_EV_IRQHandler(void);

void I2C2_ER_IRQHandler(void);

#endif

#endif /* CAMERA_I2C_H_ */
