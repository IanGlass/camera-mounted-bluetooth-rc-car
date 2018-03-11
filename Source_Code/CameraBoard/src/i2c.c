/*
 * i2c.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28
 */
#include "i2c.h"
#include "led.h"
#include "uart.h"

#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <stdio.h>

#define HORZ_LENGTH 256
#define VERT_LENGTH 96

short int transmissionMode=0;
long unsigned int i2cErrorCount=0;
long unsigned int startCount=0;
long unsigned int dataCount=0;
long unsigned int stopCount=0;

extern char inBuffer[21];
extern uint8_t outBuffer[HORZ_LENGTH*VERT_LENGTH];

extern char outstr[200];

void i2c_init(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// enable APB1 peripheral clock for I2C2
	RCC_APB1PeriphClockCmd(I2C_PERIPHERAL_CLOCK, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(GPIO_PERIPHERAL_CLOCK, ENABLE);

	//TODO: CHECK THIS
	RCC_HSICmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);

	// Setup SCL and SDA pins
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN; // we are going to use PB10 and PB11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			  // set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		  // set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			  // set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		      // enable pull up resistors
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);					  // Init GPIOB

	// Connect I2C2 pins to AF
	GPIO_PinAFConfig(I2C_GPIO_PORT, GPIO_PinSource10, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(I2C_GPIO_PORT, GPIO_PinSource11, GPIO_AF_I2C2);    // SDA

	#ifdef BOARD_CAMERA
	I2C_InitStruct.I2C_OwnAddress1 = CAMERA_ADDRESS;
	#endif
	#ifdef BOARD_COMMS
	I2C_InitStruct.I2C_OwnAddress1  = COMMS_ADDRESS;
	#endif
	#ifdef BOARD_MOTOR
	I2C_InitStruct.I2C_OwnAddress1 = MOTOR_ADDRESS;
	#endif

	// Configure I2C2
	I2C_InitStruct.I2C_ClockSpeed 	= 	I2C_CLOCK_SPEED; 					// 100kHz TODO: NOTE CHANGABLE
	I2C_InitStruct.I2C_Mode 		= 	I2C_Mode_I2C;						// I2C mode
	I2C_InitStruct.I2C_DutyCycle 	= 	I2C_DutyCycle_2;					// 50% duty cycle --> standard
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;								// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStruct);										// init I2C2

#ifdef SLAVE
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	I2C_ITConfig(I2C2,I2C_IT_EVT,ENABLE); // enable event IRQ
	I2C_ITConfig(I2C2,I2C_IT_ERR,ENABLE); // enable error IRQ
	//I2C_ITConfig(I2C2,I2C_IT_BUF,ENABLE); // enable buffer IRQ
#endif

	// Enable I2C
	I2C_Cmd(I2C2, ENABLE);

}

void i2c_init_AS_MASTER(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// enable APB1 peripheral clock for I2C2
	RCC_APB1PeriphClockCmd(I2C_PERIPHERAL_CLOCK, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(GPIO_PERIPHERAL_CLOCK, ENABLE);

	//TODO: CHECK THIS
	RCC_HSICmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);

	// Setup SCL and SDA pins
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN; // we are going to use PB10 and PB11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			  // set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		  // set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			  // set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		      // enable pull up resistors
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);					  // Init GPIOB

	// Connect I2C2 pins to AF
	GPIO_PinAFConfig(I2C_GPIO_PORT, GPIO_PinSource10, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(I2C_GPIO_PORT, GPIO_PinSource11, GPIO_AF_I2C2);    // SDA

	#ifdef BOARD_CAMERA
	I2C_InitStruct.I2C_OwnAddress1 = CAMERA_ADDRESS;
	#endif


	// Configure I2C2
	I2C_InitStruct.I2C_ClockSpeed 	= 	I2C_CLOCK_SPEED; 					// 100kHz TODO: NOTE CHANGABLE
	I2C_InitStruct.I2C_Mode 		= 	I2C_Mode_I2C;						// I2C mode
	I2C_InitStruct.I2C_DutyCycle 	= 	I2C_DutyCycle_2;					// 50% duty cycle --> standard
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;								// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStruct);										// init I2C2

	// Enable I2C
	I2C_Cmd(I2C2, ENABLE);
}


void i2c_master_send_buffer(uint8_t slave_adress, uint8_t* txBuffer, unsigned int bufferLen){
	int i=0;
	//Master Send
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, slave_adress, I2C_Direction_Transmitter);
	//timer_ms_delay(5);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_AcknowledgeConfig(I2C2, DISABLE);

	for(i=0;i<bufferLen;i++){
		I2C_SendData(I2C2,txBuffer[i]);
		I2C_AcknowledgeConfig(I2C2, DISABLE);
		//timer_ms_delay(5);
		while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	I2C_GenerateSTOP(I2C2, ENABLE);
}


void i2c_master_recv_buffer(uint8_t slave_adress, uint8_t* rxBuffer, unsigned int bufferLen){
	int i=0;

	//Master Recv
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, MOTOR_ADDRESS, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	for(i=0;i<bufferLen;i++){
		if((i+1)>=bufferLen) I2C_AcknowledgeConfig(I2C2, DISABLE);  			// Do not send ACK on Final Transmit - LOL Nick magical missing line of code
		while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED));
		rxBuffer[i]=I2C_ReceiveData(I2C2);
	}

	rxBuffer[i]='\0';

	I2C_AcknowledgeConfig(I2C2, ENABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
}


int i2c_get_bat(void){
	char txBuff[5];
	char rxBuff[5];
	int batteryVoltage=0;
	sprintf(txBuff,"BAT");
	i2c_master_send_buffer(MOTOR_ADDRESS,txBuff,3);
	timer_ms_delay(10);
	i2c_master_recv_buffer(MOTOR_ADDRESS,rxBuff,7);
	led_toggle(DEBUG_LED_0|DEBUG_LED_1);
	batteryVoltage=(((int)rxBuff[3]<<8)+(int)rxBuff[4]);
	timer_ms_delay(10);
	return batteryVoltage;
}


void i2c_set_speed(int speed){
	char txBuff[5];
	sprintf(txBuff,"MSPW");
	txBuff[4]=speed+100;
	i2c_master_send_buffer(MOTOR_ADDRESS,txBuff,5);
	//timer_ms_delay(10);
}

void i2c_set_angle(int angle){
	char txBuff[5];
	sprintf(txBuff,"MANW");
	txBuff[4]=angle+100;
	i2c_master_send_buffer(MOTOR_ADDRESS,txBuff,5);
	//timer_ms_delay(10);
}


void i2c_transfer_report(void){
	sprintf(outstr, "%s \tERRORS: %i \tSTART: %i \tDATA: %i \tSTOP: %i \tRX: %s \tTX: %s            ", (transmissionMode)?("SLAVE: TX Mode"):("SLAVE: RX Mode"), i2cErrorCount, startCount, dataCount, stopCount, inBuffer, outBuffer);
	uart_send(outstr);
}


inline void I2C_CleanADDRandSTOPF() {
/* DO NOT TOUCH .
 * DO NOT TOUCH .
 * DO NOT TOUCH !!!
 * If you do, you will surely die.
 * This code is magic and should not be read or questioned.
 */
	volatile uint32_t temp;
	while ((I2C2->SR1&I2C_SR1_ADDR)==I2C_SR1_ADDR) {
		temp=I2C2->SR1;
		temp=I2C2->SR2;
	}
	while ((I2C2->SR1&I2C_SR1_STOPF)==I2C_SR1_STOPF) {
		temp=I2C2->SR1;
		I2C2->CR1 |= 0x1;
	}
}

#ifdef SLAVE

void I2C2_EV_IRQHandler(void){
	char tempByte;

	if (!(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE))) {
		tempByte = I2C_ReceiveData(I2C2);
	}

	switch(I2C_GetLastEvent(I2C2)){

		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
			stopCount=0;
			startCount=0;
			dataCount=0;
			transmissionMode=0;
			led_on(DEBUG_LED_2);

			startCount++;
			break;

		case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
			EXTI_DeInit();
			stopCount=0;
			startCount=0;
			dataCount=0;
			transmissionMode=1;
			led_on(DEBUG_LED_1);
			I2C_SendData(I2C2, outBuffer[(startCount+dataCount)%20]);
			startCount++;
			break;


		case I2C_EVENT_SLAVE_BYTE_RECEIVED:
			inBuffer[(dataCount+stopCount)%20] = tempByte;
			dataCount++;
			break;

		case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
			I2C_SendData(I2C2, outBuffer[(startCount+dataCount)%(HORZ_LENGTH*VERT_LENGTH)]);
			dataCount++;
			break;

		case I2C_EVENT_SLAVE_STOP_DETECTED:
			led_off(DEBUG_LED_0);
			led_off(DEBUG_LED_1);

			if (!transmissionMode) {
				// We have finished receiving
				inBuffer[(dataCount+stopCount++)%20] = tempByte;
				inBuffer[(dataCount+stopCount)%20] = '\0';

				i2c_setFlags();
				//i2c_setReturnMessage();
			}

			camera_configuration_interupt_init();
			standby_init();
			led_off(DEBUG_LED_2);

			break;
	}
	I2C_CleanADDRandSTOPF();
}

void I2C2_ER_IRQHandler(void){
	i2cErrorCount++;
	I2C_ClearITPendingBit(I2C2,I2C_IT_BERR|I2C_IT_ARLO|I2C_IT_AF|I2C_IT_OVR|I2C_IT_PECERR|I2C_IT_TIMEOUT|I2C_IT_SMBALERT);
}

#endif



