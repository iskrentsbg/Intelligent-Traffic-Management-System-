/**
 * @file TrafficLightManager.hpp
 * @author Vladislav Stefanov
 * @version 2.0
 */

#pragma once

#include "ITrafficLightManager.hpp"

namespace TrafficLights
{
    class TrafficLightManager : ITrafficLightManager
    {
    public:
        TrafficLightManager();
        void registerNewCarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed);
        void turnGreen() override;
        void turnOrange() override;
        void turnRed() override;
        void updateAllCarLaneTrafficLights();

    private:
        CarLaneTrafficLightState currentState;
        void changeState(CarLaneTrafficLightState newState);
        std::unique_ptr<ITrafficLight> createNewCarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed);
        std::vector<std::unique_ptr<ITrafficLight>> trafficLights;
    };
}