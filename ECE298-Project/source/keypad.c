/*
 * keypad.c
 *
 *  Created on: Jul 5, 2019
 *      Author: RohanKumar
 */
#include "../include/components.h"
#include "../include/keypad.h"

static const char hexaKeys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void initSetupKeypad() {
    /* ROWS ARE OUTPUTS */
    // ROW 1
    GPIO_setAsOutputPin(KEYPAD_ROW1);
    GPIO_setOutputLowOnPin(KEYPAD_ROW1);

    // ROW 2
    GPIO_setAsOutputPin(KEYPAD_ROW2);
    GPIO_setOutputLowOnPin(KEYPAD_ROW2);

    // ROW 3
    GPIO_setAsOutputPin(KEYPAD_ROW3);
    GPIO_setOutputLowOnPin(KEYPAD_ROW3);

    // ROW 4
    GPIO_setAsOutputPin(KEYPAD_ROW4);
    GPIO_setOutputLowOnPin(KEYPAD_ROW4);


    /* COLUMNS ARE INTERUPTS (INPUTS) */
    // Column 1
    GPIO_setAsPeripheralModuleFunctionInputPin(KEYPAD_COL1, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(KEYPAD_COL1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(KEYPAD_COL1);
    GPIO_clearInterrupt(KEYPAD_COL1);
    GPIO_enableInterrupt(KEYPAD_COL1);

    // Column 2
    GPIO_setAsPeripheralModuleFunctionInputPin(KEYPAD_COL2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(KEYPAD_COL2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(KEYPAD_COL2);
    GPIO_clearInterrupt(KEYPAD_COL2);
    GPIO_enableInterrupt(KEYPAD_COL2);

    // Column 3
    GPIO_setAsPeripheralModuleFunctionInputPin(KEYPAD_COL3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(KEYPAD_COL3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(KEYPAD_COL3);
    GPIO_clearInterrupt(KEYPAD_COL3);
    GPIO_enableInterrupt(KEYPAD_COL3);
}

//if 1 - set the row to high, else set to low
static void setRows(int R1, int R2, int R3, int R4) {

    if (R1) { 
        GPIO_setOutputHighOnPin(KEYPAD_ROW1);
    } else { 
        GPIO_setOutputLowOnPin(KEYPAD_ROW1);
    }

    if (R2) { 
        GPIO_setOutputHighOnPin(KEYPAD_ROW2);
    } else { 
        GPIO_setOutputLowOnPin(KEYPAD_ROW2);
    }

    if (R3) { 
        GPIO_setOutputHighOnPin(KEYPAD_ROW3);
    } else { 
        GPIO_setOutputLowOnPin(KEYPAD_ROW3);
    }

    if (R4) { 
        GPIO_setOutputHighOnPin(KEYPAD_ROW4);
    } else { 
        GPIO_setOutputLowOnPin(KEYPAD_ROW4);
    }

}

char keyPressed() {
    char pressedKey;

    /* ROW 1 */
    // '1'
    setRows(0, 1, 1, 1); //r1 - low, r2- high, r3- high, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL1) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[0][0];
    }
    setRows(1, 0, 0, 0); //r1- high, r2 - low, r3 - low, r4 - low

    // '2'
    setRows(0, 1, 1, 1); //r1 - low, r2- high, r3- high, r4- high
    if (GPIO_getInputPinValue(KEYPAD_COL2) == GPIO_INPUT_PIN_LOW) {
        pressedKey = hexaKeys[0][1];
    }
    setRows(1, 0, 0, 0); //r1- high, r2 - low, r3 - low, r4 - low

    // '3'
    setRows(0, 1, 1, 1); //r1 - low, r2- high, r3- high, r4- high
    if (GPIO_getInputPinValue(KEYPAD_COL3) == GPIO_INPUT_PIN_LOW) {
        pressedKey = hexaKeys[0][2];
    }
    setRows(1, 0, 0, 0); //r1- high, r2 - low, r3 - low, r4 - low


    /* ROW 2 */
    // '4'
    setRows(1, 0, 1, 1); //r1 - high, r2- low, r3- high, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL1) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[1][0];
    }
    setRows(0, 1, 0, 0); //r1- low, r2 - high, r3 - low, r4 - low

    // '5'
    setRows(1, 0, 1, 1); //r1 - high, r2- low, r3- high, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL2) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[1][1];
    }
    setRows(0, 1, 0, 0); //r1- low, r2 - high, r3 - low, r4 - low

    // '6'
    setRows(1, 0, 1, 1); //r1 - high, r2- low, r3- high, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL3) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[1][2];
    }
    setRows(0, 1, 0, 0); //r1- low, r2 - high, r3 - low, r4 - low


    /* ROW 3 */
    // '7'
    setRows(1, 1, 0, 1); //r1 - high, r2- high, r3- low, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL1) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[2][0];
    }
    setRows(0, 0, 1, 0); //r1- low, r2 - low, r3 - high, r4 - low

    // '8'
    setRows(1, 1, 0, 1); //r1 - high, r2- high, r3- low, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL2) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[2][1];
    }
    setRows(0, 0, 1, 0); //r1- low, r2 - low, r3 - high, r4 - low

    // '9'
    setRows(1, 1, 0, 1); //r1 - high, r2- high, r3- low, r4- high
    if ((GPIO_getInputPinValue(KEYPAD_COL3) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[2][2];
    }
    setRows(0, 0, 1, 0); //r1- low, r2 - low, r3 - high, r4 - low


    /* ROW 4 */
    // '*' 
    setRows(1, 1, 1, 0); //r1 - high, r2- high, r3- high, r4- low
    if ((GPIO_getInputPinValue(KEYPAD_COL1) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[3][0];
    }
    setRows(0, 0, 0, 1); //r1- low, r2 - low, r3 - low, r4 - high

    // '0'
    setRows(1, 1, 1, 0); //r1 - high, r2- high, r3- high, r4- low
    if ((GPIO_getInputPinValue(KEYPAD_COL2) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[3][1];
    }
    setRows(0, 0, 0, 1); //r1- low, r2 - low, r3 - low, r4 - high

    // '#'
    setRows(1, 1, 1, 0); //r1 - high, r2- high, r3- high, r4- low
    if ((GPIO_getInputPinValue(KEYPAD_COL3) == GPIO_INPUT_PIN_LOW)) {
        pressedKey = hexaKeys[3][2];
    }
    setRows(0, 0, 0, 1); //r1- low, r2 - low, r3 - low, r4 - high


    /* RESET */
    setRows(0, 0, 0, 0); //r1- low, r2 - low, r3 - low, r4 - low

    return pressedKey;
}


