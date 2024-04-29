/*
 * functions.c
 *
 *  Created on: Mar 27, 2024
 *      Author: Kimo Store
 */
#include "functions.h"
#include"external_eeprom.h"
#include"uart.h"
#include <util/delay.h>

uint8 key[PASS_SIZE];
uint8 i;
uint8 key_check[PASS_SIZE];
uint8 val = 1;
uint8 pass_flag = 1;

uint8 compare_Pass(void){
	for (i = 0; i < PASS_SIZE; i++) {
				while (UART_recieveByte() != MC2_READY) {}
				key_check[i] = UART_recieveByte();
				EEPROM_writeByte(PASS_ADRESS2 + i, key_check[i]);
				_delay_ms(10);
			}

			for (i = 0; i < PASS_SIZE; i++) {
				EEPROM_readByte(PASS_ADRESS + i, &key[i]);
				EEPROM_readByte(PASS_ADRESS2 + i, &key_check[i]);
				if ( key[i] == key_check[i])
				{

					val=1;
				}
				else{
					val=0;
					break;
				}

			}
			while (UART_recieveByte() != MC2_READY){}
			UART_sendByte(val);
			if(val==1)
			{
				//while (UART_recieveByte() != MC2_READY) {}
				//pass_flag = UART_recieveByte();
				return 1;
			}
			else{
				//while (UART_recieveByte() != MC2_READY) {}
				//pass_flag = UART_recieveByte();
				return 0;
			}
}
