/**
 * @file CarLaneTrafficLight.cpp
 * @author Vladislav Stefanov
 * @version 2.0
 */

#include "CarLaneTrafficLight.hpp"

namespace TrafficLights
{
    CarLaneTrafficLight::CarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed)
    {
        pinGreenLight = pinGreen;
        pinOrangeLight = pinOrange;
        pinRedLight = pinRed;
#ifdef ARDUINO_ARCH_ESP32
        pinMode(pinGreenLight, OUTPUT);
        pinMode(pinOrangeLight, OUTPUT);
        pinMode(pinRedLight, OUTPUT);
#endif
    }

    void CarLaneTrafficLight::turnGreen()
    {
#ifdef ARDUINO_ARCH_ESP32
        digitalWrite(pinGreenLight, HIGH);
        digitalWrite(pinOrangeLight, LOW);
        digitalWrite(pinRedLight, LOW);
#endif
    }

    void CarLaneTrafficLight::turnOrange()
    {
#ifdef ARDUINO_ARCH_ESP32
        digitalWrite(pinGreenLight, LOW);
        digitalWrite(pinOrangeLight, HIGH);
        digitalWrite(pinRedLight, LOW);
#endif
    }

    void CarLaneTrafficLight::turnRed()
    {
#ifdef ARDUINO_ARCH_ESP32
        digitalWrite(pinGreenLight, LOW);
        digitalWrite(pinOrangeLight, LOW);
        digitalWrite(pinRedLight, HIGH);
#endif
    }
}