/*
 * Project: ReflowOven
 * File:    Led.cpp
 * Author:  Sandro Lutz
 */

#include "Led.h"

Led::Led(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->off();
}

void Led::on()
{
    digitalWrite(pin, LOW);
}

void Led::off()
{
    digitalWrite(pin, HIGH);
}

void Led::toggle()
{
    digitalWrite(pin, !(digitalRead(pin) & HIGH));
}