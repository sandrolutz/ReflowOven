/*
 * Project: ReflowOven
 * File:    RTDSensor.cpp
 * Author:  Sandro Lutz
 */

#include "RTDSensor.h"


void RTDSensor::attach(int analogPin)
{
    ADMUX = (1<<REFS0) | (1<<REFS1) | (analogPin - 14); // select internal reference source and input channel
    ADCSR = (1<<ADEN)  | (1<<ADIE)  | (1<<ADFR)
          | (1<<ADPS2) | (1<<ADPS1) | (1<<ADSC);        // enables ADC with prescaler 64 and activates interrupt
    sei();
    position = 0;
}

int RTDSensor::getTemperature()
{
    if(updated) {
        long meanVoltage = (voltage[0] + voltage[1] + voltage[2]) / 3;
        // calculate temperature
        long resistance = ((meanVoltage)*10000) / (5000 - meanVoltage);   // resistance in Ohm

        // formula: T=(R/1000-1)/alpha
        if(resistance < 1385) {                           // < 100 °C
            temperature = (resistance-1000)*100/385;      // alpha = 0.00385
        } else if(resistance < 1573) {                    // < 150 °C
            temperature = (resistance-1000)*10/38;        // alpha = 0.00380
        } else if(resistance < 1758) {                    // < 200 °C
            temperature = (resistance-1000)*100/379;      // alpha = 0.00379
        } else if(resistance < 1940) {                    // < 250 °C
            temperature = (resistance-1000)*100/378;      // alpha = 0.00378
        } else {                                          // > 250 °C
            temperature = (resistance-1000)*100/373;      // alpha = 0.00373
        }
        if(position < 2) {
            ++position;
        } else {
            position = 0;
        }
        updated = false;
    }
    return temperature;
}

/*
 * Interrupt routine executed when ADC has converted a new value.
 */
ISR(ADC_vect)
{
    rtdSensor.voltage[rtdSensor.position] = ((((long)ADC)*302)/1000) + 541;
    rtdSensor.updated = true;
}