/*
 * Project: ReflowOven
 * File:    Button.h
 * Author:  Sandro Lutz
 */

#ifndef BUTTON_h
#define BUTTON_h

#include <Arduino.h>
#include "Utils.h"

#ifndef BUTTON_INTERVAL
#define BUTTON_INTERVAL 10 // in ms
#endif

class Button
{
public:
    void attach(uint8_t pin);
    bool pressed();
    bool released();
    bool changed();
    bool down();
    bool up();
    bool update(); // returns true - button state has changed; false - otherwise
private:
    uint8_t pin;
    uint8_t state;
    unsigned long time;
};

#endif