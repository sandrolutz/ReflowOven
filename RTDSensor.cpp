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
}

int RTDSensor::getTemperature()
{
    if(temperature == 0) {
        // calculate temperature
        //long voltage = ((((long)analogRead(analogPin))*302)/1000) + 541;
        long resistance = (((long)voltage)*10000) / (5000 - voltage);   // resistance in Ohm

        // formula: T=(R/1000-1)/alpha
        if(resistance < 1385) {                    // < 100 °C
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
    }
    return temperature;
}

/*
 * Interrupt routine executed when ADC has converted a new value.
 */
ISR(ADC_vect)
{
    rtdSensor.voltage = ((((long)ADC)*302)/1000) + 541;
    rtdSensor.temperature = 0;
    PORTD ^= (1<<PD5);
}