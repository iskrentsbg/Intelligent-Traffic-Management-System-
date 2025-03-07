/**
 * @file ITrafficLight.hpp
 * @author Vladislav Stefanov
 * @version 2.0
 */

#pragma once

namespace TrafficLights
{
    class ITrafficLight
    {
    public:
        virtual void turnGreen() = 0;
        virtual void turnOrange() = 0;
        virtual void turnRed() = 0;
    };
}