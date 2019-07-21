#include <msp430.h>
#include "driverlib.h"
#include "include/Board.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "include/components.h"
#include "include/mux.h"
#include "include/motor.h"
#include "include/keypad.h"
#include "include/hal_lcd.h"
#include "include/hall_effect.h"
#include "include/pwm.h"
#include "include/pushButtons.h"
#include "include/rpm.h"

void setup() {
    setupMux();
    setupHallEffect();
    setupMotor();
    initSetupKeypad();
    Init_LCD();
    setupPushButtons();
}

void hallEffectCalc();

char chr;
bool IE = false;
bool pushButtonRight = false;
bool pushButtonLeft = false;
int distance;
int dir;
bool startedMotor = false;

void main () {
    WDT_A_hold(WDT_A_BASE);     // Stop watchdog timer
    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
    */
    PMM_unlockLPM5();
    setup();
    _EINT();

    enableY3();

    while( true ) {
        displayScrollText("PRESS PUSH BUTTONS BELOW OR KEYPAD TO START");
        if (IE) {
            _DINT();
            IE = false;
            break;
        }
    }

    char dist[3];
    _EINT();
    pushButtonRight = false;
    int idx = 0;
    displayScrollText("ENTER A DISTANCE ON THE KEYPAD CONFIRM USING RIGHT PB");
    while (!pushButtonRight) {
        _EINT();
        if (pushButtonRight) {
            pushButtonRight = false;
            _DINT();
            IE = false;
            break;
        } else if (IE) {

            if (chr >= '0' && chr <= '9') {
                dist[idx] = chr;

                if (idx == 0) {
                    showChar('M', pos6);
                    showChar(dist[0], pos1);
                } else if (idx == 1) {
                    showChar(dist[1], pos2);
                } else if (idx == 2) {
                    showChar(dist[2], pos3);
                }
                idx++;
            }

            IE = false;
            _DINT();

            if (idx >= 3) {
                break;
            }
        }
    }

    char speed[2];
    _EINT();
    idx = 0;
    pushButtonRight = false;
    displayScrollText("ENTER A SPEED BETWEEN 1 TO 99 ON THE KEYPAD CONFIRM USING RIGHT PB");
    while (!pushButtonRight) {
        _EINT();
        if (pushButtonRight) {
            pushButtonRight = false;
            _DINT();
            IE = false;
            break;
        } else if (IE) {

            if (chr >= '0' && chr <= '9') {
                speed[idx] = chr;

                if (idx == 0) {
                    showChar('P', pos4);
                    showChar('E', pos5);
                    showChar('R', pos6);
                    showChar(speed[0], pos1);
                } else if (idx == 1) {
                    showChar(speed[1], pos2);
                }
                idx++;
            }

            IE = false;
            _DINT();

            if (idx >= 2) {
                break;
            }
        }
    }

    int direction;
    _EINT();
    pushButtonRight = false;
    displayScrollText("ENTER DIRECTION ON THE PBS LEFT IS CCW RIGHT IS CW");
    while (!pushButtonRight && !pushButtonLeft) {
        if (pushButtonRight) {
           direction = CW;
           _DINT();
           IE = false;
           showChar('C', pos1);
           showChar('W', pos2);
           break;
       } else if (pushButtonLeft) {
           direction = CCW;
           IE = false;
           _DINT();
           showChar('C', pos1);
           showChar('C', pos2);
           showChar('W', pos3);
           break;
       }
    }

    clearLCD();

    distance = atoi(dist);
    float motor_speed = atoi(speed);

    int pwm_speed = motor_speed/100 * 511;

    pushButtonRight = false;
    pushButtonLeft = false;

    startedMotor = true;
    startMotor(direction, pwm_speed);

    while (startedMotor) {
        continue;
    }

    __bis_SR_register(LPM4_bits + GIE);
}

#pragma vector = PORT1_VECTOR       // Using PORT1_VECTOR interrupt because P1.4 and P1.5 are in port 1
__interrupt void PORT1_VECTOR_ISR()
{
    if (!GPIO_getInputPinValue(PB_LEFT)) {
        IE = true;
        pushButtonLeft = true;
        GPIO_clearInterrupt(PB_LEFT);
        GPIO_clearInterrupt(HALL_EFFECT_2);

        GPIO_clearInterrupt(KEYPAD_COL1); // Column 1
        GPIO_clearInterrupt(KEYPAD_COL2); // Column 2
        GPIO_clearInterrupt(KEYPAD_COL3); // Column 3

        GPIO_clearInterrupt(KEYPAD_ROW1); // Row 1
        GPIO_clearInterrupt(KEYPAD_ROW2); // Row 2
        GPIO_clearInterrupt(KEYPAD_ROW3); // Row 3
        GPIO_clearInterrupt(KEYPAD_ROW4); // Row 4

    } else if (HE2()) {
        IE = true;

        if (startedMotor) {
            hallEffectCalc();
        }

        GPIO_clearInterrupt(HALL_EFFECT_2);
        GPIO_clearInterrupt(PB_LEFT);

    } else {
        chr = keyPressed();

        IE = true;
        GPIO_clearInterrupt(PB_LEFT);
        GPIO_clearInterrupt(HALL_EFFECT_2);

        GPIO_clearInterrupt(KEYPAD_COL1); // Column 1
        GPIO_clearInterrupt(KEYPAD_COL2); // Column 2
        GPIO_clearInterrupt(KEYPAD_COL3); // Column 3

        GPIO_clearInterrupt(KEYPAD_ROW1); // Row 1
        GPIO_clearInterrupt(KEYPAD_ROW2); // Row 2
        GPIO_clearInterrupt(KEYPAD_ROW3); // Row 3
        GPIO_clearInterrupt(KEYPAD_ROW4); // Row 4
    }

}

#pragma vector = PORT2_VECTOR       // Using PORT1_VECTOR interrupt because P1.4 and P1.5 are in port 1
__interrupt void pushButtonRight_ISR()
{
    IE = true;
    pushButtonRight = true;

    GPIO_clearInterrupt(PB_RIGHT);
}

void hallEffectCalc() {
    static int cur_distance;
    static int num_revolutions;
    volatile int curRPM = 0;
    const int wheel_circ = 27;

    num_revolutions++;

    float curTime = TIMEFACTOR * Timer_A_getCounterValue(TIMER_A1_BASE);
    curRPM = 60 * num_revolutions / curTime;

    int threshold = distance / 3;

    cur_distance = num_revolutions * wheel_circ;

    if (num_revolutions == 2) {
        while (1) {
            if (HE1()) {
                dir = CW;
                break;
            }
            if (HE3()) {
                dir = CCW;
                break;
            }
        }
    }

    char rpm_string[3];
    if (num_revolutions > 1) {

        if (dir == CCW) {
            showChar('C', pos1);
            showChar('C', pos2);
            showChar('W', pos3);
        } else if (dir == CW) {
            showChar('C', pos1);
            showChar('W', pos2);
        }

        sprintf(&rpm_string, "%d", curRPM);
        showChar(rpm_string[0], pos4);
        showChar(rpm_string[1], pos5);
        showChar(rpm_string[2], pos6);
    }

    /* LEDS */
    if (cur_distance < threshold) {
       enableY3();
    } else if (cur_distance < threshold*2) {
       enableY0();
    } else if (cur_distance < threshold*3) {
       enableY1();
    } else {
       enableY2();
       startedMotor = false;
       stopWheel();
    }
}


