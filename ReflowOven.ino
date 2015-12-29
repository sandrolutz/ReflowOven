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
        switch(state) {
            case STATE_PRE_HEATING:
                Led1.setPattern(200, 200);
                Led2.off();
                Led3.off();
                break;
            case STATE_SOAKING:
                Led1.on();
                Led2.setPattern(200, 200);
                Led3.off();
                break;
            case STATE_SOLDERING:
                Led1.on();
                Led2.on();
                Led3.setPattern(200, 200);
                break;
            case STATE_COOLING_DOWN:
                Led1.on();
                Led2.on();
                Led3.on();
                break;
            default:
                Led1.off();
                Led2.off();
                Led3.off();
                Led1.setPattern(200, 200);
                Led2.setPattern(200, 200);
                Led3.setPattern(200, 200);
        }
        Serial.print("State changed to ");
        Serial.println(state);
    }
}

void updateHeating() {
    if (getTimeDifference(lastHeatingUpdate, millis()) > 9) {
        lastHeatingUpdate = millis();

        unsigned long elapsedTime = getTimeDifference(stateChangeTime[state], millis());
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
            Serial.println(" degree (C)");
        }
    }
}

void setup() {
    lastHeatingUpdate = 0;
    //initialize temperature history
    for (uint8_t i = 0; i < 100; ++i) {
        temperatureHistory[i] = 55;
    }
    button.attach(2);
    rtdSensor.attach(A0);
    heating.attach(A5);
    Serial.begin(BAUD_RATE);        // opens serial port
    Serial.setTimeout(20);          // Timeout for serial reading step
    // print cause of the RESET
    if(MCUSR & (1<<WDRF)) {
        Serial.println("RESET caused by watchdog timer");
    } else if(MCUSR & (1<<BORF)) {
        Serial.println("RESET caused by brown-out detector");
    } else if(MCUSR & (1<<EXTRF)) {
        Serial.println("RESET caused by external reset on RESET pin");
    } else if(MCUSR & (1<<PORF)) {
        Serial.println("RESET caused by power on");
    }
    MCUSR = 0x00;
    Led1.on();
    delay(300);
    Led1.off();
    Led2.on();
    delay(300);
    Led2.off();
    Led3.on();
    delay(300);
    Led3.off();
    Serial.println("ReflowOven initialized...");
    state = 5;
    setState(STATE_INACTIVE);
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