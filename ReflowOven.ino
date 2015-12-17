/*
 * Project: ReflowOven
 * Author:  Sandro Lutz
 */

#include "RTDSensor.h"
#include "Heating.h"
#include "Led.h"
//#include "Bounce2.h"

#define BAUD_RATE 9600        // 9600 bps  57600 bps 115200 bps

#define STATE_INACTIVE 0
#define STATE_PRE_HEATING 1
#define STATE_SOAKING 2
#define STATE_SOLDERING 3
#define STATE_COOLING_DOWN 4
#define STATE_HEATING_TEST 5

Led Led1(3);
Led Led2(4);
Led Led3(5);

//Bounce button;

RTDSensor rtdSensor;
Heating heating;
uint8_t state;
unsigned long stateChangeTime[5];
unsigned long lastHeatingUpdate;
int temperatureHistory[100];
uint8_t historyPosition = 0;

void setup()
{
   state = STATE_INACTIVE;
   lastHeatingUpdate = 0;
   //initialize temperature history
   for(uint8_t i=0;i<100;++i) {
       temperatureHistory[i] = 55;
   }
   pinMode(2, INPUT_PULLUP);       // Button on PD2
   //button.attach(2);
   rtdSensor.attach(A0);
   heating.attach(A5);
   Serial.begin(BAUD_RATE);        // opens serial port
   Serial.setTimeout(20);          // Timeout for serial reading step
   setState(STATE_HEATING_TEST);
}

void loop()
{
  Serial.println(state);
    //button.update();

    /*if(button.rose()) {
        if(state == STATE_INACTIVE) {
            setState(STATE_HEATING_TEST);
        } /*else {
            setState(STATE_INACTIVE);
        }*/
    //}
    /*if(digitalRead(2) == LOW) {
      setState(STATE_HEATING_TEST);
      heating.setPower(3);
    } else {
      setState(STATE_INACTIVE);
      heating.setPower(0);
    }*/
    if(state > 0) {
        updateHeating();
    }
    serialEvent();
}

void setState(uint8_t newState)
{
    if(state != newState && newState < 6) {
        state = newState;
        stateChangeTime[state] = millis();
        if(state > 0) {
            Led2.on();
        } else {
            Led2.off();
        }
        Serial.print(state);
        Serial.print(" (new state)");
        Serial.println(newState);
    }
}

unsigned long getTimeDifference(unsigned long time1, unsigned long time2)
{
    if(time1 > time2) { // an overflow occured
        return 4294967295 - time1 + time2;
    }
    return time2 - time1;
}

void updateHeating()
{
    if(getTimeDifference(lastHeatingUpdate, millis()) >= 10) {
        lastHeatingUpdate = millis();

        unsigned long elapsedTime = getTimeDifference(stateChangeTime[state-1], millis());
        temperatureHistory[historyPosition] = rtdSensor.getTemperature();
        switch (state) {
            case STATE_HEATING_TEST:
                heating.setPower(3);
                break;
            case STATE_PRE_HEATING:
                if(rtdSensor.getTemperature() < 180) {
                    int8_t dTemperature = rtdSensor.getTemperature() - ((historyPosition == 99) ? temperatureHistory[0] : temperatureHistory[historyPosition+1]);
                    if(dTemperature < 2) { // deltaT = 2 °C
                        //heating.increasePower();
                    } else if(dTemperature > 18) {
                        //heating.decreasePower();
                    }
                    break;
                }
                setState(STATE_SOAKING);
                //heating.setPower(Heating::MAX_POWER/2);
                // no break! fall trough to the next state
            case STATE_SOAKING:
                if(getTimeDifference(stateChangeTime[STATE_PRE_HEATING], millis()) < 40000) {
                    // TODO: hold the temperature at 180°C ...
                    break;
                }
                setState(STATE_SOLDERING);
                // no break! fall trough to the next state
            case STATE_SOLDERING:
                if(rtdSensor.getTemperature() < 210) {
                    //heating.setPower(Heating::MAX_POWER);
                    break;
                }
                setState(STATE_COOLING_DOWN);
                // no break! fall trough to the next state
            case STATE_COOLING_DOWN:
                if(rtdSensor.getTemperature() < 150) {
                    setState(STATE_INACTIVE);
                }
                //heating.setPower(0);
                break;
            case STATE_INACTIVE:
                //heating.setPower(0);
                break;
        }
        if(historyPosition == 99) {
            historyPosition = 0;
        } else {
            ++historyPosition;
        }
    }
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
