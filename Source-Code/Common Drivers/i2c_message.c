/*
 * i2c_message.c
 *
 *  Created on: May 23, 2013
 *      Author: nbb23
 */

#include "i2c_message.h"

char inBuffer[21];
char outBuffer[21];
char subBuff1[5];
char subBuff2[16];

extern long int batteryVoltage;
extern long int emfVoltage;
extern long int bemfVoltage;

//long int batteryFlag = 0;
long int speedFlag = 0;
long int angleFlag = 0;
long int speedValue = 0;
long int angleValue = 0;

void set_bat_str(void) {
	sprintf(outBuffer, "%s%c%c", BAT_MSG, (char)(batteryVoltage>>8), (char)(batteryVoltage));
}
void set_speed_read_str(void) {
	sprintf(outBuffer, "%s%c", SPEED_READ_MSG, (char)batteryVoltage);
}
void set_speed_write_str(void) {
	sprintf(outBuffer, "%s%s", SPEED_WRITE_MSG, ACK_MSG);
}
void set_angle_read_str(void) {
	sprintf(outBuffer, "%s%c", ANGLE_READ_MSG, (char)batteryVoltage);
}
void set_angle_write_str(void) {
	sprintf(outBuffer, "%s%s", ANGLE_WRITE_MSG, ACK_MSG);
}
void set_nack_str(void) {
	sprintf(outBuffer, "%s", NACK_MSG);
}

void i2c_setFlags(void) {
	memcpy( subBuff1, inBuffer, 4 );
	subBuff1[4] = '\0';
	if (strcmp (inBuffer,BAT_MSG) == 0) {
		set_bat_str();
	}
	else if (strcmp (inBuffer,SPEED_READ_MSG) == 0) {
		set_speed_read_str();
	}
	else if (strcmp (subBuff1,SPEED_WRITE_MSG) == 0) {
		speedFlag = 1;
		set_speed_write_str();
		speedValue = (long)inBuffer[4]-100;
	}
	else if (strcmp (inBuffer,ANGLE_READ_MSG) == 0) {
		set_angle_read_str();
	}
	else if (strcmp (subBuff1,ANGLE_WRITE_MSG) == 0) {
		angleFlag = 1;
		set_angle_write_str();
		angleValue = (long)inBuffer[4]-100;
	}
	else {
		set_nack_str();
	}
}

//strtol();
//sprintf;

