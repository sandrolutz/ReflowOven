/*
 * Project: ReflowOven
 * File:    RTDSensor.h
 * Author:  Sandro Lutz
 */

#ifndef RTD_SENSOR
#define RTD_SENSOR

#include <avr/interrupt.h>

extern "C" void ADC_vect(void) __attribute__ ((signal));

class RTDSensor
{
    friend void ADC_vect(void);
public:
    void attach(int analogPin);
    int getTemperature();
protected:
    bool updated;
    uint8_t position;
    int temperature;
    long voltage[3];
};

extern RTDSensor rtdSensor;

#endif