/*
 * hall_effect.c
 *
 *  Created on: Jul 5, 2019
 *      Author: RohanKumar
 */

#include "../include/components.h"
#include "../include/hall_effect.h"


void setupHallEffect() {
    GPIO_setAsInputPin(HALL_EFFECT_1);
    GPIO_setAsInputPin(HALL_EFFECT_3);

    //setting up hall effect 2 (middle one) with an interupt
    GPIO_setAsPeripheralModuleFunctionInputPin(HALL_EFFECT_2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_selectInterruptEdge(HALL_EFFECT_2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(HALL_EFFECT_2);
    GPIO_clearInterrupt(HALL_EFFECT_2);
    GPIO_enableInterrupt(HALL_EFFECT_2);
}

int HE1() {
    return GPIO_getInputPinValue(HALL_EFFECT_1) ^ 1;
}

int HE2() {
    int val = GPIO_getInputPinValue(HALL_EFFECT_2);
    return val ^ 1;
}

int HE3() {
    return GPIO_getInputPinValue(HALL_EFFECT_3) ^ 1;
}
