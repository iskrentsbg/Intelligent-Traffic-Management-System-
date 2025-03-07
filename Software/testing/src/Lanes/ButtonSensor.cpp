#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif
#include "ButtonSensor.hpp"

namespace Lanes
{
    ButtonSensor::ButtonSensor(int pin, int GivenDelta, Lanes::Lane *NewLane)
    {   
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("ButtonSensor.cpp, ButtonSensor(): Start of constructor");
#endif
        button_pin = pin;
        lane = NewLane;
#ifdef ARDUINO_ARCH_ESP32
        pinMode(pin, INPUT_PULLUP);
#endif
        delta = GivenDelta;
    }

    int ButtonSensor::detectButtonPress(int prevDelta)
    {
        int buttonStateIn = 0;
#ifdef ARDUINO_ARCH_ESP32
        currentTime = millis();
        buttonStateIn = digitalRead(button_pin);
        if (currentTime - prevBtnTime > debounceTime)
        {
            if (buttonStateIn == LOW)
            {
                once = true;
            }
            if (buttonStateIn == HIGH && once == true)
            {
                Serial.println("ButtonSensor.cpp, detectButtonPress(): Detected a button press");
                once = false;
                prevBtnTime = currentTime;
                return delta;
            }
        }
#endif
        return prevDelta;
    }

    void ButtonSensor::editLane(Lanes::Lane *NewLane)
    {   
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("ButtonSensor.cpp, editLane(): Start of function");
#endif
        lane = NewLane;
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("ButtonSensor.cpp, editLane(): End of function");
#endif
    }
}
