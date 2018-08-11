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
*****************************************************************************
*/

/* Includes */
#include "main.h"
#include "stm32f4xx.h"
#include "ir.h"

#include "uart.h"
#include "led.h"
#include "button.h"
#include "timer.h"
#include "standby.h"
#include "i2c.h"
#include "debug_gpio.h"

#define BATMAN 1

//static uint8_t lastButtonStatus = RESET;
static u16 currentLed = 0;
static u32 currentTime = 0;

int wheel_angle = 0;
int motor_speed = 0;
long unsigned int sysTick;

void init_all()
{


	led_init();
	button_init();
	timer_init();
	standby_init();
	uart_init();
	debug_gpio_init();
	i2c_init();

	led_on(STATUS_LED_ON);
}

int main() {
    init();
    uint16_t ir_data = 0;

    int i=0;
    init_all();
    while(1){


    	for(i=-100;i<100;i++){
    		i2c_set_speed(i);
    	}

    	for(i=100;i>-100;i--){
    		i2c_set_speed(i);
    	}

    	//uart_send("World\n\r");






    }



    ir_rc5_rx_init();
    //initExtInterrupt();
	//initClkInterrupt();
    while(BATMAN)
    {
    	ir_data = ir_rc5_rx_read();
    	MOTORCONTROL(ir_data);


        //loop();
	}
}

void init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOC, &gpio);


}

void initButton() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio);
}

void initButtonLED() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOC, &gpio);
}

void loop() {
	__NOP();
    //static uint32_t counter = 0;

    //uint8_t currentButtonStatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

	//if (lastButtonStatus != currentButtonStatus && currentButtonStatus != RESET) {
	//	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
	//}
    //lastButtonStatus = currentButtonStatus;

    //++counter;

    //GPIO_ResetBits(GPIOC, LEDS);
    //GPIO_SetBits(GPIOC, LED[counter % 4]);

    //delay(50);
	uint16_t data = 0;
	data = ir_rc5_rx_read();

	if (data == 12320) {
		GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
	}
	if (data == 12304) {
		GPIO_ToggleBits(GPIOC, GPIO_Pin_3);
	}
	if (data == 12321) {
		GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
	}
	if (data == 12305) {
		GPIO_ToggleBits(GPIOC, GPIO_Pin_2);
	}
}

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void toggleLed(void) {
    if (currentTime % 200 == 0) {
        GPIO_ToggleBits(GPIOC, LED[currentLed]);
    }
}



void initExtInterrupt() {
    EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void switchLed(void) {
    GPIO_ResetBits(GPIOC, LED[currentLed]);

    currentLed++;
    if (currentLed > 3) {
        currentLed = 0;
    }
}


void MOTORCONTROL(data)
{
	if (data == 12320)
    {
       	if(motor_speed>=0)
       	{
       	   	motor_speed += 2;
       	}
       	else
       	{
       		motor_speed = 0;
       	}
    }
    else if (data == 12304)
    {
      	wheel_angle -= 2;
    }
    else if (data == 12321)
    {
      	if(motor_speed<=0)
       	{
       		motor_speed -= 2;
       	}
      	else
      	{
      		motor_speed = 0;
      	}
    }
    else if (data == 12305)
    {
       	wheel_angle += 2;
    }
    else
    {
       	if(motor_speed>0)
       	{
       		motor_speed -=1;
       	}
       	else if(motor_speed<0)
       	{
       		motor_speed +=1;
       	}
    }
}



