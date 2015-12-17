/*
 * Project: ReflowOven
 * File:    Led.cpp
 * Author:  Sandro Lutz
 */

#include "Led.h"

Led::Led(uint8_t pin)
{
    _pin = pin;
    pinMode(pin, OUTPUT);
    off();
}

void Led::on()
{
    _onTime = 0;
    _offTime = 0;
    digitalWrite(_pin, LOW);
}

void Led::off()
{
    _onTime = 0;
    _offTime = 0;
    digitalWrite(_pin, HIGH);
}

void Led::toggle()
{
    _onTime = 0;
    _offTime = 0;
    digitalWrite(_pin, !(digitalRead(_pin) & HIGH));
}

void Led::setPattern(unsigned int onTime, unsigned int offTime) {
    if(onTime == 0) {
        return off();
    }
    if(offTime == 0) {
        return on();
    }
    _onTime = onTime;
    _offTime = offTime;
}
void Led::update() {
    if(_onTime != 0 && _offTime != 0) {
        if (digitalRead(_pin) == HIGH) {
            if (getTimeDifference(_changeTime, millis()) >= _onTime) {
                digitalWrite(_pin, LOW);
                _changeTime = millis();
            }
        } else {
            if (getTimeDifference(_changeTime, millis()) >= _offTime) {
                digitalWrite(_pin, HIGH);
                _changeTime = millis();
            }
        }
    }
}