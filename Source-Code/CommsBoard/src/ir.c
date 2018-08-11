/*
 * ir.c
 *
 *  Created on: May 22, 2013
 *      Author: Scott
 */

#include "ir.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define IR_RC5_BIT_PERIOD_US 1778

#ifndef IR_RC5_RX_ACTIVE_STATE
/* Most IR demodulators have an active low output.  */
#define IR_RC5_RX_ACTIVE_STATE 0
#endif
static u32 micro_sec = 0;
/** Return output state of IR receiver.
    @return IR receiver state (1 = IR modulation detected).  */
static inline uint8_t ir_rc5_rx_get (void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == IR_RC5_RX_ACTIVE_STATE;
}

void delay_us(uint16_t us) {
    us *= 3.33;
    while(us--) {
        __NOP();
    }

}
/*
void delay_us(uint16_t us) {
    us /= 100;
    uint16_t start = 0;
    start = micro_sec;
    while((micro_sec-start) < us) {
        __NOP();
    }
*/


void initClkInterrupt() {
    SysTick_Config(80);
    NVIC_SetPriority(SysTick_IRQn, 1);
}


void SysTick_Handler(void) {
	GPIO_ToggleBits(GPIOB, GPIO_Pin_10);
    micro_sec++;
}

uint8_t ir_rc5_rx_ready_p (void)
{
    return ir_rc5_rx_get ();
}


static uint16_t ir_rc5_rx_wait_state (uint8_t state)
{
    uint16_t us;

    for (us = 0; us < IR_RC5_BIT_PERIOD_US; us++)
    {
        if (ir_rc5_rx_get () == state)
            return us;
        delay_us(1-0.4);
    }
    return us;
}


static uint16_t ir_rc5_rx_wait_transition (void)
{
    uint8_t initial;

    initial = ir_rc5_rx_get ();

    return ir_rc5_rx_wait_state (!initial);
}


/** Receive RC5 data packet over IR serial link.
    @return status code
    @note No error checking is performed.  If there is no activity on the
    IR serial link, this function returns immediately.  Otherwise, this
    function blocks until the entire frame is received.  This must be called
    frequently to ensure that a start bit is seen.  */
int16_t ir_rc5_rx_read (void)
{
    int16_t data;
    int i;
    uint16_t us;
    /* Look to see if there is some IR modulation marking the second
       half of the first start bit.  */
    if (!ir_rc5_rx_ready_p ())
    {
        return IR_RC5_RX_NONE;
    }
    /* The old RC-5 format had two start bits; this made a bit-bashed
       software implementation easier.  The problem is that we have
       been called just before the falling edge of the start bit.  */

    /* Search for the next falling transition.  */
    us = ir_rc5_rx_wait_state (0);
    if (us >= IR_RC5_BIT_PERIOD_US)
        return IR_RC5_RX_TIMEOUT;

    if (us > (IR_RC5_BIT_PERIOD_US >> 1))
    {
        /* The field bit is 0.  */
        data = 2;
    }
    else
    {
        /* The field bit is 1; so delay until middle of bit period.  */
        data = 3;
        us = ir_rc5_rx_wait_state (1);
        if (us >= IR_RC5_BIT_PERIOD_US)
            return IR_RC5_RX_TIMEOUT;
    }

    for (i = 2; i < 14; i++)
    {
        data <<= 1;

        delay_us(0.5 * IR_RC5_BIT_PERIOD_US + 100);
        us = ir_rc5_rx_wait_transition ();
        if (us >= IR_RC5_BIT_PERIOD_US)
        {
            return IR_RC5_RX_TIMEOUT;

        }
        if (ir_rc5_rx_get ())
            data |= 1;
    }

    return data;
}


/** Initialise IR serial receiver driver.  */
void ir_rc5_rx_init (void)
{
    /* Ensure PIO clock activated for reading.  */
	//pio_init (IR_RC5_RX_PIO);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    /* Configure IR receiver PIO as input.  */
    //pio_config_set (IR_RC5_RX_PIO, PIO_INPUT);
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* RC5 pin configuration: input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}





