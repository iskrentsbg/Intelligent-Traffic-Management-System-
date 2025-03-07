/**
 * @file ITrafficLightManager.hpp
 * @author Vladislav Stefanov
 * @version 2.0
 */

#pragma once

#include <vector>
#include <memory>
#include "CarLaneTrafficLightState.hpp"
#include "CarLaneTrafficLight.hpp"

namespace TrafficLights
{
    class ITrafficLightManager
    {
    public:
        virtual void registerNewCarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed) = 0;
        virtual void turnGreen() = 0;
        virtual void turnOrange() = 0;
        virtual void turnRed() = 0;
        virtual void updateAllCarLaneTrafficLights() = 0;
    };
}