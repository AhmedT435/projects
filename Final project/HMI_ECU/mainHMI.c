/*
 * mainHMI.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Kimo Store
 */

#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include <util/delay.h>
#include<avr/io.h> /*for sreg enabla intruppts*/
#include"timer1.h"
#define PASS_SIZE 5
#define MC2_READY 0x10
#define DOOR_OPENING 10
void callback_function(void);
uint8 Str[20];
uint8 val_check = 0;
uint8 pass_flag = 1;
uint8 i;
uint8 counter = 0;
uint8 ticks = 0;
int main(void) {
	//uint8 Str[20];

	LCD_init();
	UART_init(&UART_configerations);
	Timer1_setCallBack(callback_function);

	UART_sendByte(MC2_READY);
	while (pass_flag != 0) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Pls Enter Pass:");
		LCD_moveCursor(1, 0); /* Move the cursor to the second row */

		for (i = 0; i < PASS_SIZE; i++) {

			UART_sendByte(KEYPAD_getPressedKey());
			_delay_ms(600); /*Key pressed delay*/
			LCD_displayCharacter('*');

		}
		while (KEYPAD_getPressedKey() != 13)
			;

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Pls Re-Enter the");
		LCD_displayStringRowColumn(1, 0, "same Pass:");

		for (i = 0; i < PASS_SIZE; i++) {
			UART_sendByte(MC2_READY);
			UART_sendByte(KEYPAD_getPressedKey());
			_delay_ms(600); /*Key pressed delay*/
			LCD_displayCharacter('*');
		}
		while (KEYPAD_getPressedKey() != 13)
			;

		UART_sendByte(MC2_READY);

		val_check = UART_recieveByte();

		if (val_check == 1) {
			pass_flag = 0;
			UART_sendByte(MC2_READY);
			UART_sendByte(pass_flag);
		}

	}
	pass_flag = 0;
	for (;;) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change Pass");
		if (KEYPAD_getPressedKey() == '+') {

			UART_sendByte(MC2_READY);
			UART_sendByte(KEYPAD_getPressedKey());

			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Pls Enter Pass:");
			LCD_moveCursor(1, 0); /* Move the cursor to the second row */

			for (i = 0; i < PASS_SIZE; i++) {
				UART_sendByte(MC2_READY);
				UART_sendByte(KEYPAD_getPressedKey());
				_delay_ms(700); /*Key pressed delay*/
				LCD_displayCharacter('*');

			}
			while (KEYPAD_getPressedKey() != 13)
				;

			//UART_sendByte(MC2_READY);
			val_check = UART_recieveByte();

			if(counter<2){
			if (val_check == 0) {
				counter++;
				//pass_flag = 1;
				LCD_clearScreen();
				LCD_moveCursor(0,0);
				LCD_intgerToString((3-counter));
				LCD_displayStringRowColumn(0, 3, "trial left");
				_delay_ms(2000);

			} else if (val_check == 1) {
				Timer1_init(&TIMER1_Configerations);
				SREG |= (1 << 7);

				while(ticks == 0){}
				_delay_ms(200);
				while(ticks != 0){
					_delay_ms(600);
					if (ticks == 2) {

						LCD_clearScreen();
						LCD_displayStringRowColumn(0, 0, "Door is Unlocking");

						UART_sendByte(1);
					} else if (ticks == 17) {
						LCD_clearScreen();
						LCD_displayStringRowColumn(0, 0, "hold");

						UART_sendByte(2);
					} else if (ticks == 20) {
						LCD_clearScreen();
						LCD_displayStringRowColumn(0, 0, "Door is locking");
						//_delay_ms(500);
						UART_sendByte(3);
					} else if (ticks == 35) {

						SREG &= ~(1 << 7);
						ticks = 0;
						UART_sendByte(4);
					}
				}

			}
			//pass_flag = 0;
			}
			else if (counter>=2)
			{

				UART_sendByte(5);
				LCD_displayStringRowColumn(0, 0,"ERROR");
				_delay_ms(60000);
			}
		}

		else if (KEYPAD_getPressedKey() == '-') {

			UART_sendByte(MC2_READY);
						UART_sendByte(KEYPAD_getPressedKey());

						LCD_clearScreen();
						LCD_displayStringRowColumn(0, 0, "Pls Enter Pass:");
						LCD_moveCursor(1, 0); /* Move the cursor to the second row */

						for (i = 0; i < PASS_SIZE; i++) {
							UART_sendByte(MC2_READY);
							UART_sendByte(KEYPAD_getPressedKey());
							_delay_ms(1000); /*Key pressed delay*/
							LCD_displayCharacter('*');

						}
						while (KEYPAD_getPressedKey() != 13)
							;

						UART_sendByte(MC2_READY);
						val_check = UART_recieveByte();


			if (val_check == 1) {pass_flag = 1;
				while (pass_flag != 0) {
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 0, "Pls Enter Pass:");
					LCD_moveCursor(1, 0); /* Move the cursor to the second row */

					for (i = 0; i < PASS_SIZE; i++) {

						UART_sendByte(KEYPAD_getPressedKey());
						_delay_ms(1000); /*Key pressed delay*/
						LCD_displayCharacter('*');

					}
					while (KEYPAD_getPressedKey() != 13)
						;

					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 0, "Pls Re-Enter the");
					LCD_displayStringRowColumn(1, 0, "same Pass:");

					for (i = 0; i < PASS_SIZE; i++) {
						UART_sendByte(MC2_READY);
						UART_sendByte(KEYPAD_getPressedKey());
						_delay_ms(1000); /*Key pressed delay*/
						LCD_displayCharacter('*');
					}
					while (KEYPAD_getPressedKey() != 13)
						;

					UART_sendByte(MC2_READY);

					val_check = UART_recieveByte();

					if (val_check == 1) {
						pass_flag = 0;
						UART_sendByte(MC2_READY);
						UART_sendByte(pass_flag);
					}

				}
				pass_flag = 0;
			}
			else{
				LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "wrong");
			_delay_ms(2000);}
		}
	}
	return 0;
}

void callback_function(void) {
	ticks++;
}

/*
 if(val_check==1){
 && pass_flag == 0

 }*/

