/*
 * ultrasonic.c
 *
 *  Created on: Mar 4, 2024
 *      Author: Kimo Store
 */

#include"ultrasonic.h"
#include"icu.h"
#include"gpio.h"
#include<util/delay.h>

sint16 g_distance = 0;
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_timePeriod = 0;
uint16 g_timePeriodPlusHigh = 0;
ICU_ConfigType ICU_Configurations = {F_CPU_8,RAISING};

void Ultrasonic_init(void){
	ICU_init(&ICU_Configurations);
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);
}

void Ultrasonic_Trigger(void){
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);

}

uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();
	g_distance=(g_timeHigh/SOUNDSPEEDCOFF);
	return g_distance;
}

void Ultrasonic_edgeProcessing(void){
	g_edgeCount++;
		if(g_edgeCount == 1)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */
			ICU_clearTimerValue();
			/* Detect falling edge */
			ICU_setEdgeDetectionType(FALLING);
		}
		else if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = ICU_getInputCaptureValue();
			/* Detect rising edge */
			ICU_setEdgeDetectionType(RAISING);
			/* Clear the timer counter register to start measurements again */
			ICU_clearTimerValue();
		}

}
