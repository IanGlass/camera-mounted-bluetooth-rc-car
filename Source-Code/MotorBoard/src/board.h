/*
 * board.h
 *
 *  Created on: May 20, 2013
 *      Author: nbb23
 *        Note: Ensure you choose the right one!
 */

#ifndef BOARD_H_
#define BOARD_H_


//#define BOARD_CAMERA
#define BOARD_MOTOR
//#define BOARD_COMMS

#ifdef BOARD_CAMERA
#define MASTER
#else
#define SLAVE
#endif



#endif /* BOARD_H_ */
