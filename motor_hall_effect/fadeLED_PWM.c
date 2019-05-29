/* DESCRIPTION
 * Sample code structure for motor control
*/


#include "driverlib.h"
#include <stdint.h>
#include "stdbool.h"
#include "Board.h"
#include <stdio.h>

/**
 * main.c
 */

void startWheel(size_t polarity) {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN7);
    if (polarity) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
    } else {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
    }
}

void stopWheel() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN7);
}

void wheel() {
    /* clockwise */

    volatile int reading = -1;
    size_t polarity = 0;
    for (;;) {
        startWheel(polarity);
        reading = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6);
        if (!reading) {
            stopWheel();
            uint64_t counter;
            for (counter = 0; counter < 100000; counter++) {
                continue;
            }
            polarity = polarity ^ 1;
        }
    }
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //Setup
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5); // AI2
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4); // AI1
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7); // pwm out
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN6);  // hall effect sensor

    wheel();

    return 0;
}
