/*
 * motor.h
 *
 *  Created on: Feb 23, 2024
 *      Author: Kimo Store
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

#define IN1_PORT PORTB_ID
#define IN1_PIN	PIN0_ID
#define IN2_PORT PORTB_ID
#define IN2_PIN PIN1_ID


void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* MOTOR_H_ */
