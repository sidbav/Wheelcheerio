/*
 * mux.c
 *
 *  Created on: Jul 5, 2019
 *      Author: RohanKumar
 */

#include "../include/components.h"
#include "../include/mux.h"

void setupMux() {
    GPIO_setAsOutputPin(MUX_S0);
    GPIO_setAsOutputPin(MUX_S1);

    //set to low at start
    GPIO_setOutputLowOnPin(MUX_S0);
    GPIO_setOutputLowOnPin(MUX_S1);
}

void enableY0() {
    GPIO_setOutputLowOnPin(MUX_S0);
    GPIO_setOutputLowOnPin(MUX_S1);
}

void enableY1() {
    GPIO_setOutputHighOnPin(MUX_S0);
    GPIO_setOutputLowOnPin(MUX_S1);
}

void enableY2() {
    GPIO_setOutputLowOnPin(MUX_S0);
    GPIO_setOutputHighOnPin(MUX_S1);
}

void enableY3() {
    GPIO_setOutputHighOnPin(MUX_S0);
    GPIO_setOutputHighOnPin(MUX_S1);
}
