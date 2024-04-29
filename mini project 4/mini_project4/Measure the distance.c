/*
 * Measure the distance.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Kimo Store
 */


#include"lcd.h"
#include"ultrasonic.h"
#include <avr/io.h>

extern sint16 g_distance ;
extern uint8 g_edgeCount ;

int main(void){
	SREG |= (1<<7);
	Ultrasonic_init();
	LCD_init();
	LCD_moveCursor(1,0);
	LCD_displayString("Distance = ");

	for(;;){
		Ultrasonic_readDistance();
		if(g_edgeCount == 2)
		{
			g_edgeCount = 0;
		LCD_moveCursor(1,11);
		LCD_intgerToString(g_distance);
		LCD_displayString("Cm");
		LCD_displayString(" ");
		}
	}

}

