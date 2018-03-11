/*
 * camera_configuration.c
 *
 *  Created on: May 28, 2013
 *      Author: mlk28
 */

/* Includes */
#include "camera_configuration.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_i2c.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_rcc.h"

#include "i2c_message.h"
#include "led.h"
#include "camera_pwm.h"
#include "debug_gpio.h"

//Camera Globals
int begin_camera_data=0;
uint16_t vert_index=0;
uint16_t horz_index=0;
uint16_t bits_index=0;

#define HORZ_LENGTH 256
#define VERT_LENGTH 96
#define SLOWED_EXT_CLK 10 // 200 works


#define data_bit_0 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)
#define data_bit_1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)
#define data_bit_2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)
#define data_bit_3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define data_bit_4 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define data_bit_5 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define data_bit_6 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define data_bit_7 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define data_bit_HD GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)
#define data_bit_VD GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)

extern uint8_t outBuffer[HORZ_LENGTH*VERT_LENGTH];


uint8_t image_count=0;

void camera_configuration_init(void){

	//Initiate Debug GPIO
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Initiate on Port B
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//Initiate on Port D
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_InitStructure);


    //Configure GPIO_inputs
	//A
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_15;  //3,2,1,DCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_11;  //7,6,5,4,0,HD,VD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//Camera PWM
	camera_pwm_config(1);
	camera_pwm_set_dc((2*(10000-1500+50*5))/10000);
}



void camera_configuration_reset(void){

	//Reset Cam
	led_on(DEBUG_LED_0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);  // Turn CAM Rest High
	timer_ms_delay(100);
	debug_gpio_on(GPIOD, GPIO_Pin_2);  // Turn on Regs
	timer_ms_delay(100);

	GPIO_SetBits(GPIOB, GPIO_Pin_14);  //Turn Reset Off
	led_off(DEBUG_LED_0);
	timer_ms_delay(100);
}

void camera_configuration_interupt_init(void){

    //DCLK INT
	//Enable clock for GPIOA 15 DCLK
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	InitGPIO(GPIOA, GPIO_Pin_15, GPIO_Mode_IN); //initialise PA0 is input
	 //attach interrupt to GPIO
	Attach_GPIO_Interrupt(EXTI_PortSourceGPIOA, EXTI_PinSource15, EXTI_Line15 ,EXTI_Trigger_Falling, 0 );


    //VD INT
	//Enable clock for GPIOC 11 DCLK
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	InitGPIO(GPIOC, GPIO_Pin_11, GPIO_Mode_IN); //initialise PA0 is input
	 //attach interrupt to GPIO
	Attach_GPIO_Interrupt(EXTI_PortSourceGPIOC, EXTI_PinSource11, EXTI_Line11 ,EXTI_Trigger_Rising, 0 );


    //HD INT
	//Enable clock for GPIOC 11 DCLK
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	InitGPIO(GPIOC, GPIO_Pin_10, GPIO_Mode_IN); //initialise PA0 is input
	 //attach interrupt to GPIO
	Attach_GPIO_Interrupt(EXTI_PortSourceGPIOC, EXTI_PinSource10, EXTI_Line10 ,EXTI_Trigger_Rising, 0 );

}


void camera_configuration_interupt_deinit(void){
	EXTI_DeInit();
}



void EXTI15_10_IRQHandler(void)
{
	//HD Interrupt
	 if(EXTI_GetITStatus(EXTI_Line10) != RESET)  //
	 {
		 	//led_toggle(DEBUG_LED_0);

		 	EXTI_ClearFlag(EXTI_Line10);  			//Clear interrupt

		 	if(begin_camera_data==0){ 	  			//If this is start of data transfer -> Reduce PWM signal frequency.
		 		begin_camera_data=1;
		 		camera_pwm_config(SLOWED_EXT_CLK);
		 		camera_pwm_set_dc((SLOWED_EXT_CLK*(10000-1500+50*5))/10000);
		 	}

		 	vert_index++;
		 	horz_index=0;
			//horz_index=(horz_index+1)%HORZ_LENGTH;

	 }

	 //VD Interrupt
	 if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	 {
			EXTI_ClearFlag(EXTI_Line11);
			led_toggle(DEBUG_LED_1);
			image_count++;
			vert_index=0;
			horz_index=0;
			//vert_index=(vert_index+1)%VERT_LENGTH;

	 }

	 //DCLK//EXTI0 ISR
	 if(EXTI_GetITStatus(EXTI_Line15) != RESET)  							// DCLK Interrupt
	 {
			EXTI_ClearFlag(EXTI_Line15); 									//clear interrupt

			if(data_bit_HD&&data_bit_VD){
				//led_toggle(DEBUG_LED_2);

				horz_index++;
				int index=(vert_index*HORZ_LENGTH+horz_index)%(VERT_LENGTH*HORZ_LENGTH);

				outBuffer[index]=   (data_bit_0<<0)+\
									  (data_bit_1<<1)+\
									  (data_bit_2<<2)+\
									  (data_bit_3<<3)+\
									  (data_bit_4<<4)+\
									  (data_bit_5<<5)+\
									  (data_bit_6<<6)+\
									  (data_bit_7<<7);
				bits_index++;
			}
	 }
}



/**
  * @brief  configures specified GPIO pin as output.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be configured in output mode.
  *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @param  GPIO_Mode: Specify GPIO Configuration i.e. input/output/ADC/AF
  *         This parameter can be a value of @ref GPIOMode_TypeDef
  * @retval None
  */

void InitGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode)
{
	 GPIOPuPd_TypeDef PuPdMode = 0;
	 GPIO_InitTypeDef  GPIO_InitStructure;

	 switch(GPIO_Mode)
	 {
	 case GPIO_Mode_OUT:
	  PuPdMode = GPIO_PuPd_NOPULL; //digital output. Not using open drain mode as I do not know how that operates
	  break;
	 case GPIO_Mode_IN:
	  PuPdMode = GPIO_PuPd_NOPULL; //digital read have Pin as input floating
	  break;
	 case GPIO_Mode_AN:
	  PuPdMode = GPIO_PuPd_NOPULL; //for analogue read have Pin as input floating
	  break;
	 case GPIO_Mode_AF: //need to do a remapping if using alternate functions
	  PuPdMode = GPIO_PuPd_UP; //for PWM have not looked at accounting for the various other alternate functions
	  break;
 }

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //used for digital output and PWM output
               //this setting does not matter for ADC and digital read
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = PuPdMode;
 GPIO_Init(GPIOx, &GPIO_InitStructure);
}



/**
  * @brief  attach an external interrupt source to a GPIO pin.
  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
  *          EXTI lines where x can be (A..I).
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  *           This parameter can be EXTI_PinSourcex where x can be (0..15, except
  *           for EXTI_PortSourceGPIOI x can be (0..11).
  * @param  EXTI_Line: Specifies the EXTI Line to be configured.
  *     This parameter can be EXTI_LINEx where x can be (0..15)
  * @param  EXTI_Trigger: Specify whether Interrupt is generated on the rising, falling or rising and falling edges
  * @param  Priority: Priority of the Interrupt (lower the number the higher the priority)
  * @retval None
  * @NOTE: Note that there are 22 EXTI interrupt sources. This function can only be used to configure upto
  *     15 of those interrupts sources
  */
void Attach_GPIO_Interrupt(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex, uint32_t EXTI_Line,EXTITrigger_TypeDef EXTI_Trigger, uint8_t Priority)
{

	 /* Connect EXTI Line to appropriate GPIO Pin */
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);

	 NVIC_InitTypeDef   NVIC_InitStructure;
	 EXTI_InitTypeDef   EXTI_InitStructure;

	 /* Configure EXTI Line */
	 EXTI_InitStructure.EXTI_Line = EXTI_Line;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);

	 /* Enable and set EXTI Line Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
}
















