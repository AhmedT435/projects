/*
 * mainECU.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Kimo Store
 */

#include"external_eeprom.h"
#include"uart.h"
#include <util/delay.h>
#include<avr/io.h> /*for sreg enabla intruppts*/
#include"functions.h"
#include <avr/interrupt.h>
#include"motor.h"
#include"timer1.h"
#include"buzzer.h"
uint8 counter = 0, door_flag = 0;
void callback_function(void);
extern uint8 key[PASS_SIZE];
extern uint8 i;
extern uint8 key_check[PASS_SIZE];
extern uint8 val;
extern uint8 pass_flag;

//uint8 compare_Pass(void);
int main(void) {

	TWI_init(&TWI_configerations);
	UART_init(&UART_configerations);
	DcMotor_Init();
	Buzzer_init();
	while (UART_recieveByte() != MC2_READY) {
	}

	while (pass_flag != 0) {

		for (i = 0; i < PASS_SIZE; i++) {
			key[i] = UART_recieveByte();
			EEPROM_writeByte(PASS_ADRESS + i, key[i]);
			_delay_ms(10);
		}

		for (i = 0; i < PASS_SIZE; i++) {
			while (UART_recieveByte() != MC2_READY) {
			}
			key_check[i] = UART_recieveByte();
			EEPROM_writeByte(PASS_ADRESS2 + i, key_check[i]);
			_delay_ms(10);
		}

		for (i = 0; i < PASS_SIZE; i++) {
			EEPROM_readByte(PASS_ADRESS + i, &key[i]);
			EEPROM_readByte(PASS_ADRESS2 + i, &key_check[i]);
			if (key[i] == key_check[i]) {

				val = 1;
			} else {
				val = 0;
				break;
			}

		}
		while (UART_recieveByte() != MC2_READY) {
		}
		UART_sendByte(val);
		if (val == 1) {
			while (UART_recieveByte() != MC2_READY) {
			}
			pass_flag = UART_recieveByte();
		}
	}
	for (;;) {
		while (UART_recieveByte() != MC2_READY) {
		}
		if (UART_recieveByte() == '+') {

			for (i = 0; i < PASS_SIZE; i++) {
				while (UART_recieveByte() != MC2_READY) {
				}
				key_check[i] = UART_recieveByte();
				EEPROM_writeByte(PASS_ADRESS2 + i, key_check[i]);
				_delay_ms(10);
			}

			for (i = 0; i < PASS_SIZE; i++) {
				EEPROM_readByte(PASS_ADRESS + i, &key[i]);
				EEPROM_readByte(PASS_ADRESS2 + i, &key_check[i]);
				if (key[i] == key_check[i]) {

					val = 1;
				} else {
					val = 0;
					break;
				}

			}
			//while (UART_recieveByte() != MC2_READY) {}
			UART_sendByte(val);

			pass_flag = val;

			if (pass_flag == 1) {

				while (UART_recieveByte() != 1) {}
				DcMotor_Rotate(CW,255);

				while (UART_recieveByte() != 2) {}
				DcMotor_Rotate(STOP,0);

				while (UART_recieveByte() != 3) {}
				DcMotor_Rotate(A_CW,255);
				while (UART_recieveByte() != 4) {}
				DcMotor_Rotate(STOP,0);

			} else if(pass_flag == 0) {
				while (UART_recieveByte() != 5) {}
				Buzzer_on();
				_delay_ms(60000);
				Buzzer_off();

			}
		}

		if (UART_recieveByte() == '-') {
			for (i = 0; i < PASS_SIZE; i++) {
						while (UART_recieveByte() != MC2_READY) {
						}
						key_check[i] = UART_recieveByte();
						EEPROM_writeByte(PASS_ADRESS2 + i, key_check[i]);
						_delay_ms(10);
					}
			for (i = 0; i < PASS_SIZE; i++) {
							EEPROM_readByte(PASS_ADRESS + i, &key[i]);
							EEPROM_readByte(PASS_ADRESS2 + i, &key_check[i]);
							if (key[i] == key_check[i]) {

								val = 1;
							} else {
								val = 0;
								break;
							}

						}
						while (UART_recieveByte() != MC2_READY) {}
						UART_sendByte(val);

						pass_flag = val;
				if (pass_flag == 1) {
					while (UART_recieveByte() != MC2_READY) {
						}

						while (pass_flag != 0) {

							for (i = 0; i < PASS_SIZE; i++) {
								key[i] = UART_recieveByte();
								EEPROM_writeByte(PASS_ADRESS + i, key[i]);
								_delay_ms(10);
							}

							for (i = 0; i < PASS_SIZE; i++) {
								while (UART_recieveByte() != MC2_READY) {
								}
								key_check[i] = UART_recieveByte();
								EEPROM_writeByte(PASS_ADRESS2 + i, key_check[i]);
								_delay_ms(10);
							}

							for (i = 0; i < PASS_SIZE; i++) {
								EEPROM_readByte(PASS_ADRESS + i, &key[i]);
								EEPROM_readByte(PASS_ADRESS2 + i, &key_check[i]);
								if (key[i] == key_check[i]) {

									val = 1;
								} else {
									val = 0;
									break;
								}

							}
							while (UART_recieveByte() != MC2_READY) {
							}
							UART_sendByte(val);
							if (val == 1) {
								while (UART_recieveByte() != MC2_READY) {
								}
								pass_flag = UART_recieveByte();
							}
						}
				}
		}
	}
	return 0;
}

void callback_function(void) {
	counter++;

}

