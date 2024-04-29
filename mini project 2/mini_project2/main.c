/*
 * main.c
 *
 *  Created on: Feb 3, 2024
 *      Author: Ahmed Tarek Hanafi
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void seven_segmant_CA(void);
void seven_segmant_increment(void);
void Interrupts(void);
void Timer1_CTC(void);



unsigned char g_flag=0;
unsigned char sec = 0;
unsigned char min = 0;
unsigned char hour = 0;
unsigned char sec1_counter=0;
unsigned char sec2_counter=0;
unsigned char min1_counter=0;
unsigned char min2_counter=0;
unsigned char hour1_counter=0;
unsigned char hour2_counter=0;




int main(){
	DDRD &= ~(1<<PD2) & ~(1<<PD3);
	DDRB &= ~(1<<PB2);
	PORTD |= (1<<PD2);
	PORTB |= (1<<PB2);
	DDRA |= 0x3F;
	seven_segmant_CA();
	Interrupts();
	Timer1_CTC();


	for(;;){




		/*disable all 7-segments except the first
		 * 7-segment to the right (PA5) and sending data
		 * (t_sec1) to it through PORTC
		 */
				PORTA &= ~(1<<PA4) & ~(1<<PA3) & ~(1<<PA2) & ~(1<<PA1) & ~(1<<PA0);
				PORTA |= (1<<PA5);
				PORTC = sec1_counter;
				_delay_ms(2);

				/*disable all 7-segments except the second
				* 7-segment to the right (PA4) and sending data
				* (t_sec2) to it through PORTC
				*/
				PORTA &= ~(1<<PA5) & ~(1<<PA3) & ~(1<<PA2) & ~(1<<PA1) & ~(1<<PA0);
				PORTA |= (1<<PA4);
				PORTC = sec2_counter;
				_delay_ms(2);

				/*disable all 7-segments except the third
				* 7-segment to the right (PA3) and sending data
				* (t_min1) to it through PORTC
				*/
				PORTA &= ~(1<<PA5) & ~(1<<PA4) & ~(1<<PA2) & ~(1<<PA1) & ~(1<<PA0);
				PORTA |= (1<<PA3);
				PORTC = min1_counter;
				_delay_ms(2);

				/*disable all 7-segments except the fourth
				* 7-segment to the right (PA2) and sending data
				* (t_min2) to it through PORTC
				*/
				PORTA &= ~(1<<PA5) & ~(1<<PA4) & ~(1<<PA3) & ~(1<<PA1) & ~(1<<PA0);
				PORTA |= (1<<PA2);
				PORTC = min2_counter;
				_delay_ms(2);

				/*disable all 7-segments except the fifth
				* 7-segment to the right (PA1) and sending data
				* (t_hour1) to it through PORTC
				*/
				PORTA &= ~(1<<PA5) & ~(1<<PA4) & ~(1<<PA3) & ~(1<<PA2) & ~(1<<PA0);
				PORTA |= (1<<PA1);
				PORTC = hour1_counter;
				_delay_ms(2);

				/*disable all 7-segments except the sixth
				* 7-segment to the right (PA0) and sending data
				* (t_hour2) to it through PORTC
				*/
				PORTA &= ~(1<<PA5) & ~(1<<PA4) & ~(1<<PA3) & ~(1<<PA2) & ~(1<<PA1);
				PORTA |= (1<<PA0);
				PORTC = hour2_counter;
				_delay_ms(2);

				if(g_flag == 1)
				{
					seven_segmant_increment();
					g_flag = 0;

				}





	}

}




void Interrupts(void){
	MCUCR |= (1<<ISC11)|(1<<ISC10) |(1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	MCUCSR &= ~(1<<ISC2);
	GICR|= (1<<INT1) | (1<<INT0) |(1<<INT2);
	SREG |= (1<<7);
}
void Timer1_CTC(void){
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);
	TCNT1 = 0;
	TIMSK |= (1<<OCIE1A);
	OCR1A =1000;


}
void seven_segmant_CA(void){
	DDRC |= 0x0F;
	PORTC &= 0xF0;
}

void seven_segmant_increment(void){


		if(sec>59)
		{
			sec=0;
			min++;
		}
		if(min>59)
		{
			min=0;
			hour++;
		}
		if(hour>24)
		{
			hour=0;
		}

		sec2_counter = sec / 10;
		sec1_counter = sec - (sec2_counter*10);

		min2_counter = min / 10;
		min1_counter = min - (min2_counter*10);

		hour2_counter = hour / 10;
		hour1_counter = hour - (hour2_counter*10);

		sec++;
}





ISR(INT0_vect){
	PORTC &= 0xF0;
	sec = 0;
	min = 0;
	hour = 0;
	sec1_counter=0;
	sec2_counter=0;
	min1_counter=0;
	min2_counter=0;
	hour1_counter=0;
	hour2_counter=0;
}
ISR(INT1_vect){
	TIMSK &= ~(1<<OCIE1A);
}
ISR(INT2_vect){
	TIMSK |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect){
	g_flag=1;

}







