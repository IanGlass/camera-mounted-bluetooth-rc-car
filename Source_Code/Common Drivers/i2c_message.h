/*
 * i2c_message.h
 *
 *  Created on: May 23, 2013
 *      Author: nbb23
 */

#ifndef I2C_MESSAGE_H_
#define I2C_MESSAGE_H_

#include <stdlib.h>
#include <stdio.h>

#define BAT_MSG "BAT"
#define BAT_MSG_LEN 7
#define SPEED_READ_MSG "MSPR"
#define SPEED_READ_MSG_LEN 8
#define SPEED_WRITE_MSG "MSPW"
#define SPEED_WRITE_MSG_LEN 7
#define ANGLE_READ_MSG "MANR"
#define ANGLE_READ_MSG_LEN 7
#define ANGLE_WRITE_MSG "MANW"
#define ANGLE_WRITE_MSG_LEN 7
#define CAMERA_READ_MSG "CAMR"
#define CAMERA_READ_MSG_LEN 0
#define CAMERA_START_MSG "CAMS"
#define CAMERA_START_MSG_LEN 7
#define CAMERA_POLL_MSG "CAMP"
#define CAMERA_POLL_MSG_LEN 7

#define ACK_MSG "ACK"
#define NACK_MSG "NACK"

#define CAMERA_READ_START_MSG "CSTART"
#define CAMERA_READ_END_MSG "CEND"

void set_bat_str(void);
void set_speed_read_str(void);
void set_speed_write_str(void);
void set_angle_read_str(void);
void set_angle_write_str(void);

void i2c_setFlags(void);

#endif /* I2C_MESSAGE_H_ */
