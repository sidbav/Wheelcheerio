/*
 * pushButtons.c
 *
 *  Created on: Jul 9, 2019
 *      Author: sidbav
 */

#include "../include/pushButtons.h";
#include "../include/components.h";

void setupPushButtons() {

    GPIO_setAsPeripheralModuleFunctionInputPin(PB_LEFT, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(PB_LEFT, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(PB_LEFT);
    GPIO_clearInterrupt(PB_LEFT);
    GPIO_enableInterrupt(PB_LEFT);

    GPIO_setAsPeripheralModuleFunctionInputPin(PB_RIGHT, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(PB_RIGHT, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(PB_RIGHT);
    GPIO_clearInterrupt(PB_RIGHT);
    GPIO_enableInterrupt(PB_RIGHT);

    _EINT();
}

