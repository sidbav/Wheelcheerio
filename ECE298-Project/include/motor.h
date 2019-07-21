#ifndef INCLUDE_MOTOR_H_
#define INCLUDE_MOTOR_H_

enum wheelDirection {
    CW,
    CCW
};

enum tripStat {
    RPM,
    direction
};

void setupMotor();
void startMotor(int direction, int speed);
void enableBrakes();
void stopWheel();
void displayStat();

#endif /* INCLUDE_MOTOR_H_ */
