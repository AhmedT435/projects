/*
 * ultrasonic.h
 *
 *  Created on: Mar 4, 2024
 *      Author: Kimo Store
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include"std_types.h"

#define SOUNDSPEEDCOFF 58.8     /*sound speed in cm/sec  */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);


#endif /* ULTRASONIC_H_ */
