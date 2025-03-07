/**
 * @file CarLaneTrafficLight.hpp
 * @author Vladislav Stefanov
 * @version 2.0
 */

#pragma once

#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif
#include "ITrafficLight.hpp"

namespace TrafficLights
{
    class CarLaneTrafficLight : public ITrafficLight
    {
    public:
        CarLaneTrafficLight(int pinRed, int pinOrange, int pinGreen);
        void turnGreen() override;
        void turnOrange() override;
        void turnRed() override;

    private:
        int pinGreenLight;
        int pinOrangeLight;
        int pinRedLight;
    };
}