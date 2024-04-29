/*
 * timer1.c
 *
 *  Created on: Mar 14, 2024
 *      Author: Kimo Store
 */
#include"timer1.h"
#include<avr/io.h>
#include <avr/interrupt.h>
#include"std_types.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

Timer1_ConfigType TIMER1_Configerations ={0,7813,F_CPU_1024,CTC_OCR1A_NONPWM};

void Timer1_init(const Timer1_ConfigType * Config_Ptr){

	uint8 mode_selector =(Config_Ptr->mode);
	if(mode_selector==NORMALMODE){
		TCCR1A=0;
		TCCR1B=0|((Config_Ptr->prescaler));
		TCNT1 = Config_Ptr-> initial_value;
		TIMSK =(1<<TOIE1);
	}
	else if(mode_selector==CTC_OCR1A_NONPWM){
		TCCR1A = (1<<FOC1A);    //راجع ال data sheet
		TCCR1B=(1<<WGM12)|((Config_Ptr->prescaler));
		OCR1A= Config_Ptr-> compare_value;
		TIMSK =(1<<OCIE1A);
	}
	else if(mode_selector==CTC_OCR1A_FASTPWM){
		TCCR1A=(0XF0); /*Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM, (inverting mode)*/
		TCCR1B=(1<<WGM12)|(1<<WGM13)|((Config_Ptr->prescaler));
		OCR1A= Config_Ptr-> compare_value;
		TIMSK =(1<<OCIE1A);
	}
	else if(mode_selector==CTC_OCR1B_NONPWM){
			TCCR1A = (1<<FOC1B);   //راجع ال data sheet
			TCCR1B=(1<<WGM12)|((Config_Ptr->prescaler));
			OCR1B= Config_Ptr-> compare_value;
			TIMSK =(1<<OCIE1B);
		}
	else if(mode_selector==CTC_OCR1B_FASTPWM){
			TCCR1A=(0XF0); /*Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM, (inverting mode)*/
			TCCR1B=(1<<WGM12)|(1<<WGM13)|((Config_Ptr->prescaler));
			OCR1B= Config_Ptr-> compare_value;
			TIMSK =(1<<OCIE1B);
		}

}

void Timer1_deInit(void){
	TCCR1A=0;
	TCCR1B=0;
	TIMSK =0;
}

void Timer1_setCallBack(void(*a_ptr)(void)){
	g_callBackPtr = a_ptr;
}

ISR(TIMER1_OVF_vect){
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
ISR(TIMER1_COMPA_vect){
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
ISR(TIMER1_COMPB_vect){
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
