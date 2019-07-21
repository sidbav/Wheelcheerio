#include "../include/components.h"
#include "../include/motor.h"
#include "../include/pwm.h"
#include "../include/rpm.h"

void setupMotor() {
    GPIO_setAsOutputPin(MOTOR_AI1); // AI1
    GPIO_setAsOutputPin(MOTOR_AI2); // AI2
    GPIO_setOutputLowOnPin(MOTOR_AI2); // debug AI1
    GPIO_setOutputLowOnPin(MOTOR_AI2); // debug AI2
    initPWM();
}

void startMotor(int direction, int speed) {
    startPWM(speed);
    if (direction == CW) { // clockwise
        GPIO_setOutputHighOnPin(MOTOR_AI2);
        GPIO_setOutputLowOnPin(MOTOR_AI1);
    } else { // counter clock wise
        GPIO_setOutputLowOnPin(MOTOR_AI2);
        GPIO_setOutputHighOnPin(MOTOR_AI1);
    }
    initRPMTimer();
}

void stopWheel() {
    GPIO_setOutputLowOnPin(MOTOR_AI1); // AI1
    GPIO_setOutputLowOnPin(MOTOR_AI2); // AI2
    GPIO_setOutputHighOnPin(MOTOR_PWM); // PWM
}


