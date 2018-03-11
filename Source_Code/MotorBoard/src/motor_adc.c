/*
 * motor_adc.c
 *
 *  Created on: May 22, 2013
 *      Author: nbb23
 */

#include "motor_adc.h"

extern long int batteryVoltage;
extern long int emfVoltage;
extern long int bemfVoltage;

extern char outstr[200];

int adc1_channel_mode = ADC_MODE_BATTERY;
int adc2_channel_mode = ADC_MODE_EMF;

void motor_adc_init(void){
	ADC_InitTypeDef ADC_init_structure; //Structure for adc configuration
	GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin

	//ADC2 - GPIOA Pin 7 - Channel 7 - EMF
	//Clock configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
	//Analog pin configuration
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_7;//The channel 10 is connected to PA7
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PA7 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOA,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	//ADC2 - GPIOA Pin 6 - Channel 6 - BEMF
	//Clock configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOAEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
	//Analog pin configuration
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_6;//The channel 10 is connected to PA6
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PA6 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOA,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	//ADC1 - GPIOC Pin 4 - Channel 14 - battery
	//Clock configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
	//Analog pin configuration
	GPIO_initStructre.GPIO_Pin = GPIO_Pin_4;//The channel 10 is connected to PC4
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC4 pin is configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

	//ADC1 structure configuration
	ADC_DeInit();
	ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
	ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
	ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
	ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
	ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
	ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
	ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC1 with the previous configuration
	//Enable ADC1 conversion
	ADC_Cmd(ADC1,ENABLE);

	//ADC2 structure configuration
	ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
	ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
	ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
	ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
	ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
	ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
	ADC_Init(ADC2,&ADC_init_structure);//Initialize ADC2 with the previous configuration
	//Enable ADC2 conversion
	ADC_Cmd(ADC2,ENABLE);

	//Select the channels to be read from
	ADC_RegularChannelConfig(ADC1,ADC_BAT_CHANNEL,1,ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC2,ADC_EMF_CHANNEL,1,ADC_SampleTime_144Cycles);
}

int motor_adc1_convert(void){
	ADC_SoftwareStartConv(ADC1);//Restart the conversion
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
	return ADC_GetConversionValue(ADC1); //Return the converted data
}

int motor_adc2_convert(void){
	ADC_SoftwareStartConv(ADC2);//Restart the conversion
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));//Processing the conversion
	return ADC_GetConversionValue(ADC2); //Return the converted data
}

void get_battery_voltage(void){
	if (adc1_channel_mode != ADC_MODE_BATTERY) {
		adc1_channel_mode=ADC_MODE_BATTERY;
		//Select the channel to be read from
		ADC_RegularChannelConfig(ADC1,ADC_BAT_CHANNEL,1,ADC_SampleTime_144Cycles);
		batteryVoltage = motor_adc1_convert();
		timer_ms_delay(250);
	}
	batteryVoltage = motor_adc1_convert()*VOLTAGE_RES_RATIO*BAT_VOLTAGE_DIVIDER;
	// Turn on battery low LED if running below threshold
	if (batteryVoltage < BAT_SUPPLY_MIN) {
		led_on(STATUS_LED_STANDBY);
	}
}

void print_battery_voltage(void){
	get_battery_voltage();
	sprintf(outstr, "Battery Voltage: %imV ...         ", batteryVoltage);
	uart_send(outstr);
}

void get_emf_voltage(void){
	if (adc2_channel_mode != ADC_MODE_EMF) {
		adc2_channel_mode=ADC_MODE_EMF;
		//Select the channel to be read from
		ADC_RegularChannelConfig(ADC2,ADC_EMF_CHANNEL,1,ADC_SampleTime_144Cycles);
		emfVoltage = motor_adc2_convert(); //clear ADC
		timer_ms_delay(250); //wait for capacitance to let voltage settle.
	}
	emfVoltage = (motor_adc2_convert()*VOLTAGE_RES_RATIO+EMF_OFFSET)*EMF_MULTIPLIER;
}

void print_emf_voltage(void){
	get_emf_voltage();
	sprintf(outstr, "EMF Voltage: %imV ...         ", emfVoltage);
	uart_send(outstr);
}

void get_bemf_voltage(void){
	if (adc2_channel_mode != ADC_MODE_BEMF) {
		adc2_channel_mode=ADC_MODE_BEMF;
		//Select the channel to be read from
		ADC_RegularChannelConfig(ADC2,ADC_BEMF_CHANNEL,1,ADC_SampleTime_144Cycles);
		bemfVoltage = motor_adc2_convert(); //clear ADC
		timer_ms_delay(250); //wait for capacitance to let voltage settle.
	}
	bemfVoltage = (motor_adc2_convert()*VOLTAGE_RES_RATIO+EMF_OFFSET)*EMF_MULTIPLIER;
}

void print_bemf_voltage(void){
	get_bemf_voltage();
	sprintf(outstr, "BEMF Voltage: %imV ...         ", bemfVoltage);
	uart_send(outstr);
}


