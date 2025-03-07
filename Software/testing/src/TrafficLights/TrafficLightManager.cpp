/**
 * @file TrafficLightManager.cpp
 * @author Vladislav Stefanov
 * @version 2.0
*/

#include "TrafficLightManager.hpp"

namespace TrafficLights
{
    TrafficLightManager::TrafficLightManager() {
        currentState = RED;
    }

    void TrafficLightManager::registerNewCarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed) {
        std::unique_ptr<ITrafficLight> newTrafficLight = createNewCarLaneTrafficLight(pinGreen, pinOrange, pinRed);
        trafficLights.push_back(std::move(newTrafficLight)); // Try valgrind to check for memory leaks
    }

    void TrafficLightManager::turnGreen() {
        changeState(CarLaneTrafficLightState::GREEN);
    }

    void TrafficLightManager::turnOrange() {
        changeState(CarLaneTrafficLightState::ORANGE);
    }

    void TrafficLightManager::turnRed() {
        changeState(CarLaneTrafficLightState::RED);
    }

    void TrafficLightManager::updateAllCarLaneTrafficLights() {
        for (const auto &i : trafficLights) {
            switch (currentState) {
            case RED: {
                i.get()->turnRed();
                break;
            }
            case ORANGE: {
                i.get()->turnOrange();
                break;
            }
            case GREEN: {
                i.get()->turnGreen();
                break;
            }
            }
        }
    }

    std::unique_ptr<ITrafficLight> TrafficLightManager::createNewCarLaneTrafficLight(int pinGreen, int pinOrange, int pinRed) {
        // I tried using "return std::make_unique<CarLaneTrafficLight>(pinGreen, pinOrange, pinRed);" to avoid "new" but it did not work.
        return std::unique_ptr<ITrafficLight>(new CarLaneTrafficLight(pinGreen, pinOrange, pinRed));
    }

    void TrafficLightManager::changeState(CarLaneTrafficLightState newState) {
        switch (newState) {
        case RED: {
            currentState = RED;
            break;
        }
        case ORANGE: {
            currentState = ORANGE;
            break;
        }
        case GREEN: {
            currentState = GREEN;
            break;
        }
        }
    }
}