/*
 * Project: ReflowOven
 * File:    RTDSensor.h
 * Author:  Sandro Lutz
 */

#ifndef RTD_SENSOR
#define RTD_SENSOR

#include "Arduino.h"
#include <avr/interrupt.h>

extern "C" void ADC_vect(void) __attribute__ ((signal));

class RTDSensor
{
    friend void ADC_vect(void);
public:
    void attach(int analogPin);
    int getTemperature();
protected:
    int temperature;
    long voltage;
};

extern RTDSensor rtdSensor;

#endif