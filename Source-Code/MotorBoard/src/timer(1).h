/*
 * timer.h
 *
 *  Created on: May 19, 2013
 *      Author: mlk28
 *    Reviewed: nbb23; May 20, 2013
 *        Note: This is generic code, no changes should
 *              be needed to work on any of the micros.
 */

#ifndef TIMER_H_
#define TIMER_H_


void timer_init(void);
void timer_ms_delay(long unsigned int delay_ms);
void TIM2_IRQHandler(void);


#endif /* TIMER_H_ */
