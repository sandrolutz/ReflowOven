/*
 * Project: ReflowOven
 * File:    Button.cpp
 * Author:  Sandro Lutz
 */

#include "Button.h"

#define DEBOUNCED_STATE 0
#define UNSTABLE_STATE  1
#define STATE_CHANGED   3

void Button::attach(uint8_t pin)
{
    state = 0;
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed()
{
    return changed() && (bool)(state & (1<<DEBOUNCED_STATE));
}

bool Button::released()
{
    return changed() && !(bool)(state & (1<<DEBOUNCED_STATE));
}

bool Button::changed()
{
    return (bool)(state & (1<<STATE_CHANGED));
}

bool Button::down()
{
    return (bool)(state & (1<<DEBOUNCED_STATE));
}

bool Button::up()
{
    return !down();
}

bool Button::update()
{
    // clear flag indicating state change
    state &= ~(1<<STATE_CHANGED);
    bool currentState = digitalRead(pin) == LOW;

    if(currentState != (bool)(state & (1<<UNSTABLE_STATE))) {
        time = millis();
        state ^= (1<<UNSTABLE_STATE);
    } else {
        if(getTimeDifference(time, millis()) >= BUTTON_INTERVAL) {
            // BUTTON_INTERVAL time has passed without a state change.
            if((bool)(state & (1<<DEBOUNCED_STATE)) != currentState) {
                time = millis();
                state ^= (1<<DEBOUNCED_STATE);
                state |= (1<<STATE_CHANGED);
                return true;
            }
        }
    }
    return false;
}