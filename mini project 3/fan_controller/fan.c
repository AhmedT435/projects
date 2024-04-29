/*
 * fan.c
 *
 *  Created on: Feb 23, 2024
 *      Author: Kimo Store
 */


#include"motor.h"
#include"lcd.h"
#include"lm35_sensor.h"
#include"adc.h"
#include"std_types.h"

uint8 g_roomtemp=0;
ADC_ConfigType  ADC_Configrations={F_CPU_8,Internal_reference_voltage_256mV};


int main(void){
	LCD_init();
	DcMotor_Init();
	ADC_init(&ADC_Configrations);



	while(1){
		g_roomtemp=LM35_getTemperature();

		if(g_roomtemp<30)
		{
			DcMotor_Rotate(STOP,0);
				LCD_moveCursor(0,2);
				LCD_displayString("FAN is= OFF");
				LCD_moveCursor(1,2);
				LCD_displayString("TEMP = ");
				LCD_intgerToString(g_roomtemp);
				LCD_displayString("C");
				LCD_displayString(" ");
		}
		else if(g_roomtemp>=120)
		{
			DcMotor_Rotate(CW,255);
			LCD_moveCursor(0,2);
			LCD_displayString("FAN is= ON");
			LCD_displayString(" ");
			LCD_moveCursor(1,2);
			LCD_displayString("TEMP = ");
			LCD_intgerToString(g_roomtemp);
			LCD_displayString("C");
			LCD_displayString(" ");
		}
		else if(g_roomtemp>=90)
		{
			DcMotor_Rotate(CW,192);
			LCD_moveCursor(0,2);
			LCD_displayString("FAN is= ON");
			LCD_displayString(" ");
			LCD_moveCursor(1,2);
		    LCD_displayString("TEMP = ");
			LCD_intgerToString(g_roomtemp);
		    LCD_displayString("C");
		    LCD_displayString(" ");
		}
		else if(g_roomtemp>=60)
		{
			DcMotor_Rotate(CW,128);
			LCD_moveCursor(0,2);
			LCD_displayString("FAN is= ON");
			LCD_displayString(" ");
			LCD_moveCursor(1,2);
			LCD_displayString("TEMP = ");
			LCD_intgerToString(g_roomtemp);
			LCD_displayString("C");
			LCD_displayString(" ");
		}
		else if(g_roomtemp>=30)
		{
			DcMotor_Rotate(CW,64);
			LCD_moveCursor(0,2);
			LCD_displayString("FAN is= ON");
			LCD_displayString(" ");
			LCD_moveCursor(1,2);
			LCD_displayString("TEMP = ");
			LCD_intgerToString(g_roomtemp);
			LCD_displayString("C");
			LCD_displayString(" ");

		}





	}
}


