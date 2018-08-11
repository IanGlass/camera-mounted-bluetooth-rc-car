/*
 * uart.c
 *
 *  Created on: May 19, 2013
 *      Author: mlk28, nbb23
 */

#include "uart.h"
#include <stm32f4xx_usart.h>


void uart_delay(uint32_t nCount)
{
  while(nCount--)
  {
  }
}


void uart_init(void){

	// Peripheral Initialization Structures
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// Enable peripheral clock for USART3
	// NOTE: change this from APB1 to APB2 for USART 1 or 6
	#ifdef BOARD_CAMERA
	RCC_APB1PeriphClockCmd(UART_CLK_RCC, ENABLE);
    #endif
	#ifdef BOARD_MOTOR
	RCC_APB2PeriphClockCmd(UART_CLK_RCC, ENABLE);
	#endif
	#ifdef BOARD_COMMS
	//NEED DEFINITIONS - For I2C:
	RCC_APB1PeriphClockCmd(UART_CLK_RCC, ENABLE);
	#endif


	//Enable peripheral clock for GPIOB
	RCC_AHB1PeriphClockCmd(UART_BUS_RCC, ENABLE);

	// GPIOA Configuration:  USART3 TX on PB10 (Also I2C2 SCL) - Pin closest to LEDS
	GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(UART_TX_BUS, &GPIO_InitStructure);


    // GPIOA Configuration:  USART3 RX on PB11 (Also I2C2 SDA) - Pin closest to LEDS */
	GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(UART_RX_BUS, &GPIO_InitStructure);

	/* Connect USART2 pins  */
	// TX = PA2 // RX = PA3
	GPIO_PinAFConfig(UART_TX_BUS, UART_TX_SOURCE, UART_LINE_AF);
    //GPIO_PinAFConfig(UART_RX_BUS, UART_RX_SOURCE, UART_LINE_AF);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(UART_LINE, &USART_InitStructure);

	USART_Cmd(UART_LINE, ENABLE); // Enable USART2


	//Display Title
	uart_send("\n\r");
	uart_send("___________________ \n\r");
	uart_send("|   Wacky Racers!  |\n\r");
	uart_send("|- -   _ _  _ _    |\n\r");
	uart_send("| - - /_ _ _ _ /   |\n\r");
	uart_send("|______O_____O_____|\n\r");


}




void uart_send(const char *message)
{

    int i=0;
    while(message[i]!= '\0')
    {
        USART_SendData(UART_LINE,message[i] );
        uart_delay(0x000F00);
        i++;
    }
	USART_SendData(UART_LINE,'\n');
	uart_delay(0x000F00);
	USART_SendData(UART_LINE,'\r');
	uart_delay(0x000F00);
}




