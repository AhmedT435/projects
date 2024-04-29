/*
 * motor.c
 *
 *  Created on: Feb 23, 2024
 *      Author: Kimo Store
 */
#include "motor.h"
#include "pwm.h"
#include "gpio.h"





void DcMotor_Init(void)
{
	GPIO_setupPinDirection(IN1_PORT,IN1_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(IN2_PORT,IN2_PIN,PIN_OUTPUT);
	GPIO_writePin(IN1_PORT,IN1_PIN,LOGIC_LOW);
	GPIO_writePin(IN2_PORT,IN2_PIN,LOGIC_LOW);
}
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	uint8 state_check=state;
	PWM_Timer0_Init(speed);
	if(state_check==STOP)
	{
		GPIO_writePin(IN1_PORT,IN1_PIN,LOGIC_LOW);
		GPIO_writePin(IN2_PORT,IN2_PIN,LOGIC_LOW);
	}
	else if(state_check==A_CW)
	{
		GPIO_writePin(IN1_PORT,IN1_PIN,LOGIC_HIGH);
		GPIO_writePin(IN2_PORT,IN2_PIN,LOGIC_LOW);
	}
	else if(state_check==CW)
	{

		GPIO_writePin(IN1_PORT,IN1_PIN,LOGIC_LOW);
		GPIO_writePin(IN2_PORT,IN2_PIN,LOGIC_HIGH);
	}

}





