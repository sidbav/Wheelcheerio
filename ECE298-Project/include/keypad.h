/*
 * keypad.h
 *
 *  Created on: Jul 5, 2019
 *      Author: RohanKumar
 */

#ifndef INCLUDE_KEYPAD_H_
#define INCLUDE_KEYPAD_H_

void initSetupKeypad();
char keyPressed();
static void setRows(int R1, int R2, int R3, int R4);
__interrupt void KeyBoard_ISR();

#endif /* INCLUDE_KEYPAD_H_ */
