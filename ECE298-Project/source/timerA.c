/*
 * timerA.c
 *
 *  Created on: Jul 8, 2019
 *      Author: sbaveja
 */

#define completePeriod 511
#include "driverlib.h"
#include "stdbool.h"

void initTimerA0() {
    //Start timer
    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = completePeriod;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &param);
}

void initTimerA1() {
    Timer_A_initContinuousModeParam rpm_timer = {0};
    rpm_timer.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    rpm_timer.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    rpm_timer.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    rpm_timer.timerClear = TIMER_A_DO_CLEAR;
    rpm_timer.startTimer = true;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &rpm_timer);
}
