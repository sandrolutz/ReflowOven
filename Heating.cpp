/*
 * Project: ReflowOven
 * File:    Heating.cpp
 * Author:  Sandro Lutz
 */

#include "Heating.h"

void Heating::attach(int pin)
{
    counter = 0;
    cc = 0;
    power = 0;
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    // Timer2
    // Prescaler: 256, OCR2 = 156.25
    // => Overflow frequency = 100Hz
    TCCR2 = (1<<CS22) | (1<<CS21);
    TIMSK |= (1<<OCIE2);
    OCR2 = 156;
    sei();
}

void Heating::increasePower() {
    if(this->power < this->MAX_POWER) {
        ++this->power;
    }
}

void Heating::decreasePower() {
    if(this->power > 0) {
        --this->power;
    }
}

void Heating::setPower(uint8_t power)
{
    if(power > this->MAX_POWER)
        power = this->MAX_POWER;

    if(this->power != power) {
        this->power = power;
    }
}

uint8_t Heating::getPower()
{
    return power;
}

ISR(TIMER2_COMP_vect)
{
    // correct OCR2 register to to eliminate the
    // deviation produced through the last 3 cycles.
    if(heating.cc == 3) {
        OCR2 = 156;
        heating.cc = 0;
    } else {
        if(heating.cc == 2) {
            OCR2 = 157;
        }
        ++heating.cc;
    }

    if(heating.counter < heating.power) {
        digitalWrite(heating.pin, LOW);
    } else {
        digitalWrite(heating.pin, HIGH);
    }
    if(heating.counter == Heating::MAX_POWER-1) {
        heating.counter = 0;
    } else {
        ++heating.counter;
    }
}
