/*
 * Project: ReflowOven
 * File:    RTDSensor.cpp
 * Author:  Sandro Lutz
 */

#include "RTDSensor.h"

#define MAX_VALUE_CHANGE 5 // in mV

void RTDSensor::attach(int analogPin)
{
    ADMUX = (1<<REFS0) | (1<<REFS1) | (analogPin - 14); // select internal reference source and input channel
    ADCSR = (1<<ADEN)  | (1<<ADIE)  | (1<<ADFR)
          | (1<<ADPS2) | (1<<ADPS1) | (1<<ADSC);        // enables ADC with prescaler 64 and activates interrupt
    sei();
}

int RTDSensor::getTemperature()
{
    if(updated) {
        long voltage = ((((long)adcValue[0])*302)/1000) + 541;
        // calculate temperature
        long resistance = ((voltage)*10000) / (5000 - voltage);   // resistance in Ohm
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
        updated = false;
    }
    return temperature;
}

/*
 * Interrupt routine executed when ADC has converted a new value.
 */
ISR(ADC_vect)
{
    rtdSensor.adcValue[1] = rtdSensor.adcValue[0];
    rtdSensor.adcValue[0] = ADC;
    if(rtdSensor.filterCounter > 4 || (rtdSensor.adcValue[0] < rtdSensor.adcValue[1] && rtdSensor.adcValue[1] - rtdSensor.adcValue[0] <= MAX_VALUE_CHANGE) ||
        (rtdSensor.adcValue[0] > rtdSensor.adcValue[1] && rtdSensor.adcValue[0] - rtdSensor.adcValue[1] <= MAX_VALUE_CHANGE)) {
        rtdSensor.updated = true;
        rtdSensor.filterCounter = 0;
    } else {
        rtdSensor.adcValue[0] = rtdSensor.adcValue[1];
        ++rtdSensor.filterCounter;
    }
}