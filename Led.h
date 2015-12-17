/*
 * Project: ReflowOven
 * File:    Led.h
 * Author:  Sandro Lutz
 */

#ifndef LED_h
#define LED_h

#include <Arduino.h>
#include "Utils.h"

class Led
{
public:
    Led(uint8_t pin);
    void on();
    void off();
    void toggle();
    void setPattern(unsigned int onTime, unsigned int offTime);
    void update();
private:
    uint8_t _pin;
    unsigned int _onTime;
    unsigned int _offTime;
    unsigned long _changeTime;
};

#endif