/*
 * Project: ReflowOven
 * File:    Led.h
 * Author:  Sandro Lutz
 */

#ifndef LED
#define LED

#include "Arduino.h"

class Led
{
public:
    Led(int pin);
    void on();
    void off();
    void toggle();
private:
    int pin;
};

#endif