#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>
#include "pinConfig.h"

void initPortMotor(void);
void motorControl1(int speed);
void motorControl1(int drirectionFWD, int speed);
void motorControl2(int speed);
void motorControl2(int drirectionFWD, int speed);
void motorControl3(int speed);
void motorControl3(int drirectionFWD, int speed);

#endif