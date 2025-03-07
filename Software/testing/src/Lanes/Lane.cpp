#include "Lane.hpp"
namespace Lanes
{
    Lane::Lane(int pinGreen, int pinOrange, int pinRed)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, Lane(): Start of constructor");
#endif

        trafficLightManager.registerNewCarLaneTrafficLight(pinGreen, pinOrange, pinRed);
#ifdef ARDUINO_ARCH_ESP32
        delay(20);
#endif
        trafficLightManager.updateAllCarLaneTrafficLights();

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, Lane(): End of constructor");
#endif
    }

    void Lane::toggleState(LaneState state)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, togglestate(): Start of function");
#endif

        if (!stateChangeFlag)
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("The state change flag is false, change state.");
#endif
            switch (state)
            {
            case LaneState::STOPPED_DETERMINE:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = STOPPED_DETERMINE");
#endif
                currentstate = LaneState::STOPPED_DETERMINE;
                break;
            case LaneState::STOPPED_EMPTY:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = STOPPED_EMPTY");
#endif
                currentstate = LaneState::STOPPED_EMPTY;
                break;
            case LaneState::STOPPED_CARS_WAITING:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = STOPPED_CARS_WAITING");
#endif
                currentstate = LaneState::STOPPED_CARS_WAITING;
                break;
            case LaneState::FLOW_DETERMINE:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = FLOW_DETERMINE");
#endif
                currentstate = LaneState::FLOW_DETERMINE;
                break;
            case LaneState::FLOW_EMPTY:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = FLOW_EMPTY");
#endif
                currentstate = LaneState::FLOW_EMPTY;
                break;
            case LaneState::FLOW_FLOWING:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, togglestate(): laneState = FLOW_FLOWING");
#endif
                currentstate = LaneState::FLOW_FLOWING;
                break;
            default:
                break;
            }
#ifdef ARDUINO_ARCH_ESP32
            currentTime = millis();
#endif
            stateChangeFlag = true;
        }
        else
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("The state change flag is true, no state changed.");
#endif
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, togglestate(): End of function");
#endif
    }

    int Lane::newCarCount(int count)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, newCarCount(): Start of function");
        Serial.println("Lane.cpp, newCarCount(): Set new car count for a lane");
#endif

        CarCount = count;

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, newCarCount(): End of function");
#endif
        return CarCount;
    }

    void Lane::newCarCountDelta(int delta)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, newCarCountDelta(): Start of function");
        Serial.println("Lane.cpp, newCarCountDelta(): Edit car count");
#endif

        if (CarCount == 0 && delta == -1)
        {
            return;
        }
        CarCount += delta;

        if (currentstate == LaneState::FLOW_EMPTY)
        {
            toggleState(LaneState::FLOW_FLOWING);
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, newCarCountDelta(): End of function");
#endif
    }

    void Lane::handleState()
    {
        if (stateChangeFlag)
        {
            if (currentstate == LaneState::STOPPED_DETERMINE)
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, handleState(): STOPPED_DETERMINE -> STOPPED_EMPTY or STOPPED_CARS_WAITING");
#endif
                currentstate = (CarCount <= 0) ? LaneState::STOPPED_EMPTY : LaneState::STOPPED_CARS_WAITING;
            }
            if (currentstate == LaneState::FLOW_DETERMINE)
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, handleState(): FLOW_DETERMINE -> FLOW_EMPTY or FLOW_FLOWING");
#endif
                currentstate = (CarCount <= 0) ? LaneState::FLOW_EMPTY : LaneState::FLOW_FLOWING;
            }

            switch (currentstate)
            {
            case LaneState::FLOW_FLOWING:
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lane.cpp, handleState(): Execute state FLOW_FLOWING");
#endif
                trafficLightManager.turnGreen();
                trafficLightManager.updateAllCarLaneTrafficLights();
                stateChangeFlag = false;
                prevGreen = true;
                break;
            case LaneState::FLOW_EMPTY:
                if (prevGreen)
                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state FLOW_EMPTY");
#endif
                    trafficLightManager.turnOrange();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    prevGreen = false;
                }
#ifdef ARDUINO_ARCH_ESP32
                if (millis() - currentTime > 2000)
#endif
                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state FLOW_EMPTY stateChangeFlag False");
#endif
                    trafficLightManager.turnRed();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    stateChangeFlag = false;
                }
                break;
            case LaneState::STOPPED_CARS_WAITING:
                if (prevGreen)
                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state STOPPED_CARS_WAITING");
#endif
                    trafficLightManager.turnOrange();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    prevGreen = false;
                }
#ifdef ARDUINO_ARCH_ESP32
                if (millis() - currentTime > 2000)
#endif
                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state STOPPED_CARS_WAITING stateChangeFlag False");
#endif
                    trafficLightManager.turnRed();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    stateChangeFlag = false;
                }
                // notify
                break;
            case LaneState::STOPPED_EMPTY:
                if (prevGreen)
                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state STOPPED_EMPTY");
#endif
                    trafficLightManager.turnOrange();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    prevGreen = false;
                }
#ifdef ARDUINO_ARCH_ESP32
                if (millis() - currentTime > 2000)
#endif

                {
#ifdef ARDUINO_ARCH_ESP32
                    Serial.println("Lane.cpp, handleState(): Execute state STOPPED_EMPTY stateChangeFlag False");
#endif
                    trafficLightManager.turnRed();
                    trafficLightManager.updateAllCarLaneTrafficLights();
                    stateChangeFlag = false;
                }
                break;
            default:
                break;
            }
        }
    }

    int Lane::getCarCount()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Lane.cpp, getCarCount(): Start of function");
        Serial.println("Lane.cpp, getCarCount(): End of function");
#endif
        return CarCount;
    }

}