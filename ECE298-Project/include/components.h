/*
 * components.h
 *
 *  Created on: Jul 5, 2019
 *      Author: RohanKumar
 */

#ifndef INCLUDE_COMPONENTS_H_
#define INCLUDE_COMPONENTS_H_

#include "Board.h"
#include "driverlib.h"

#define MOTOR_PWM       GPIO_PORT_P1,GPIO_PIN7
#define MOTOR_AI1       GPIO_PORT_P8,GPIO_PIN3
#define MOTOR_AI2       GPIO_PORT_P8,GPIO_PIN2

#define HALL_EFFECT_1   GPIO_PORT_P8,GPIO_PIN1
#define HALL_EFFECT_2   GPIO_PORT_P1,GPIO_PIN0
#define HALL_EFFECT_3   GPIO_PORT_P2,GPIO_PIN7

#define MUX_S0          GPIO_PORT_P2,GPIO_PIN5
#define MUX_S1          GPIO_PORT_P5,GPIO_PIN1

#define KEYPAD_ROW1     GPIO_PORT_P1,GPIO_PIN4
#define KEYPAD_ROW2     GPIO_PORT_P5,GPIO_PIN2
#define KEYPAD_ROW3     GPIO_PORT_P5,GPIO_PIN0
#define KEYPAD_ROW4     GPIO_PORT_P5,GPIO_PIN3
#define KEYPAD_COL1     GPIO_PORT_P1,GPIO_PIN3
#define KEYPAD_COL2     GPIO_PORT_P1,GPIO_PIN5
#define KEYPAD_COL3     GPIO_PORT_P1,GPIO_PIN6

#define PB_LEFT         GPIO_PORT_P1, GPIO_PIN2
#define PB_RIGHT        GPIO_PORT_P2, GPIO_PIN6

#endif /* INCLUDE_COMPONENTS_H_ */