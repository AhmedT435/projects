/*
 * buzzer.h
 *
 *  Created on: Mar 16, 2024
 *      Author: Kimo Store
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PIN    PIN0_ID
#define BUZZER_PORT   PORTB_ID


void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */
