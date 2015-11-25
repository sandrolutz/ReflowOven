/*
 * Project: ReflowOven
 * File:    Heating.h
 * Author:  Sandro Lutz
 */

#ifndef HEATING
#define HEATING

#include "Arduino.h"
#include <avr/interrupt.h>

extern "C" void TIMER2_COMP_vect(void) __attribute__ ((signal));

class Heating
{
    friend void TIMER2_COMP_vect(void);
public:
    static const uint8_t MAX_POWER = 5;
    void attach(int pin);
    void increasePower();
    void decreasePower();
    void setPower(uint8_t power);   // allowed interval is [0, 5]
    uint8_t getPower();
protected:
    int pin;
    uint8_t power;
    uint8_t counter;
    uint8_t cc;         // correction counter because OCR2 should ideally be 156.25
};

extern Heating heating;

#endif