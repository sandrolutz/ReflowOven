/*
 * Project: ReflowOven
 * Author:  Sandro Lutz
 */

#include "Utils.h"
#include "RTDSensor.h"
#include "Heating.h"
#include "Led.h"
#include "Button.h"

#define BAUD_RATE 9600        // 9600 bps  57600 bps 115200 bps

#define STATE_INACTIVE 0
#define STATE_PRE_HEATING 1
#define STATE_SOAKING 2
#define STATE_SOLDERING 3
#define STATE_COOLING_DOWN 4

Led Led1(3);
Led Led2(4);
Led Led3(5);

Button button;

RTDSensor rtdSensor;
Heating heating;
unsigned long stateChangeTime[5];
unsigned long lastHeatingUpdate;
int temperatureHistory[100];
uint8_t historyPosition = 0;
uint8_t state;

void setState(int newState) {
    if (state != newState && newState < 5) {
        state = newState;
        stateChangeTime[state] = millis();
        if (state > 0) {
            Led2.on();
        } else {
            Led2.off();
        }
        Serial.print("State changed to ");
        Serial.println(state);
    }
}

void updateHeating() {
    if (getTimeDifference(lastHeatingUpdate, millis()) > 99) {
        lastHeatingUpdate = millis();

        unsigned long elapsedTime = getTimeDifference(stateChangeTime[state - 1], millis());
        temperatureHistory[historyPosition] = rtdSensor.getTemperature();
        switch (state) {
            case STATE_PRE_HEATING:
                if (rtdSensor.getTemperature() < 180) {
                    int8_t dTemperature = rtdSensor.getTemperature() -
                                          ((historyPosition == 99) ? temperatureHistory[0] : temperatureHistory[
                                                  historyPosition + 1]);
                    if (dTemperature < 2) { // deltaT = 2 °C
                        heating.increasePower();
                    } else if (dTemperature > 2) {
                        heating.decreasePower();
                    }
                    break;
                }
                setState(STATE_SOAKING);
                heating.setPower(Heating::MAX_POWER / 2);
                // no break! fall trough to the next state
            case STATE_SOAKING:
                if (getTimeDifference(stateChangeTime[STATE_SOAKING], millis()) < 40000) {
                    // TODO: hold the temperature at 180°C ...
                    break;
                }
                setState(STATE_SOLDERING);
                // no break! fall trough to the next state
            case STATE_SOLDERING:
                if (rtdSensor.getTemperature() < 210) {
                    heating.setPower(Heating::MAX_POWER);
                    break;
                }
                setState(STATE_COOLING_DOWN);
                // no break! fall trough to the next state
            case STATE_COOLING_DOWN:
                if (rtdSensor.getTemperature() < 150) {
                    setState(STATE_INACTIVE);
                }
                heating.setPower(0);
                break;
            case STATE_INACTIVE:
                heating.setPower(0);
                break;
        }
        if (historyPosition == 99) {
            historyPosition = 0;
        } else {
            ++historyPosition;
        }
    }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char) Serial.read();
        if (inChar == '$') {
            Serial.print(rtdSensor.getTemperature());
            Serial.println(" °C");
        }
    }
}

void setup() {
    state = STATE_INACTIVE;
    lastHeatingUpdate = 0;
    //initialize temperature history
    for (uint8_t i = 0; i < 100; ++i) {
        temperatureHistory[i] = 55;
    }
    //pinMode(2, INPUT_PULLUP);       // Button on PD2
    button.attach(2);
    rtdSensor.attach(A0);
    heating.attach(A5);
    Serial.begin(BAUD_RATE);        // opens serial port
    Serial.setTimeout(20);          // Timeout for serial reading step
}

void loop() {
    button.update();
    if (button.released()) {
        if (state == STATE_INACTIVE) {
            setState(STATE_PRE_HEATING);
        } else {
            setState(STATE_INACTIVE);
        }
    }
    updateHeating();
    serialEvent();
    Led1.update();
    Led2.update();
    Led3.update();
}