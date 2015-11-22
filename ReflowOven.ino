/*
 * Project: ReflowOven
 * Author:  Sandro Lutz
 */

#include "RTDSensor.h"

#define BAUD_RATE 9600        // 9600 bps  57600 bps 115200 bps

RTDSensor rtdSensor;

void setup()
{
   //pinMode(5, OUTPUT);             // LED pin 5
   DDRD = (1<<PD3) | (1<<PD4) | (1<<PD5);      // LED output
   rtdSensor.attach(A0);
   Serial.begin(BAUD_RATE);        // opens serial port
   Serial.setTimeout(20);          // Timeout for serial reading step
   PORTD |= (1<<PD5);
   /*PORTD &= ~(1<<PD5);
   delay(500);
   PORTD |= (1<<PD5);
   delay(500);
   PORTD &= ~(1<<PD5);
   delay(500);
   PORTD |= (1<<PD5);*/
   /*digitalWrite(5, LOW);
   delay(500);
   digitalWrite(5, HIGH);
   delay(500);
   digitalWrite(5, LOW);
   delay(500);
   digitalWrite(5, HIGH);*/
}

void loop()
{
    serialEvent();
    /*Serial.println("This is the ReflowOven Project!");
    digitalWrite(5, HIGH);
    delay(500);
    digitalWrite(5, LOW);
    delay(500);*/
}

void serialEvent()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if(inChar == '$') {
        Serial.print(rtdSensor.getTemperature());
        Serial.println(" °C");
    }
  }
}
