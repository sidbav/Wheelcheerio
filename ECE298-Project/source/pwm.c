/*
 * pwm.c
 *
 *  Created on: Jul 8, 2019
 *      Author: sbaveja
 */
#include "../include/timerA.h"
#include "../include/components.h"
#include "../include/pwm.h"
#include "driverlib.h"

void initPWM() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOTOR_PWM, GPIO_PRIMARY_MODULE_FUNCTION);
    initTimerA0();
}

void startPWM(int dutyCycle) {
    //Initialize compare mode to generate PWM
    Timer_A_initCompareModeParam initComp2Param = {0};
    initComp2Param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    initComp2Param.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initComp2Param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    initComp2Param.compareValue = dutyCycle;
    Timer_A_initCompareMode(TIMER_A0_BASE, &initComp2Param);
}
