#include "LaneManager.hpp"

// TODO these definitions could be moved to a better place in the future.

#define CAR_DELTA_MINUS -1
#define CAR_DELTA_PLUS 1
#define CAR_DELTA_CHANGE 2

namespace Lanes
{
    LaneManager::LaneManager(vector<Lanes::Lane *> *myLanes, vector<Lanes::ButtonSensor *> *myButtons,
                             vector<int> activeLedPins, DecisionManagement::IIncomingPoints &IIP,
                             DecisionManagement::IControlsFlow &IFC)
        : pointsObserver(IIP), flowController(IFC)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, LaneManager(): Start of constructor");
#endif

        laneManagerLanes = *myLanes;
        currentLane = laneManagerLanes[0];
        currentLaneNum = 0;
        laneManagerButtons = *myButtons;
        activePins = activeLedPins;
        currentstate = LaneManagerState::FLOW;

#ifdef ARDUINO_ARCH_ESP32
        for (const auto &i : activePins)
        {
            pinMode(i, OUTPUT);
            delay(20);
        }
#endif

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, LaneManager(): End of constructor");
#endif
    }

    void LaneManager::Flow()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, Flow(): Start of function");
#endif

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, Flow(): Execute FLOW state for all the lanes");
#endif

        currentstate = LaneManagerState::FLOW;

        for (const auto &i : laneManagerLanes)
        {
            i->toggleState(LaneState::FLOW_DETERMINE);
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, Flow(): End of function");
#endif
    }

    void LaneManager::StopFlow()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, StopFlow(): Start of function");
#endif

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, StopFlow(): Execute STOPPING state for all the lanes");
#endif

        currentstate = LaneManagerState::STOPPING;
#ifdef ARDUINO_ARCH_ESP32
        currentTime = millis();
#endif
        stoppingBool = true; // this will turn the lights red, see the handler() function

        for (const auto &i : laneManagerLanes)
        {
            i->toggleState(LaneState::STOPPED_DETERMINE);
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, StopFlow(): End of function");
#endif
    }

    void LaneManager::handler()
    {
        if (stoppingBool)
        {   
            #ifdef ARDUINO_ARCH_ESP32
            if (millis() - currentTime >= 2000)
            #endif
            {
                flowController.flowHasStopped();

#ifdef ARDUINO_ARCH_ESP32
                Serial.println("Lanemanager.cpp, flow has stopped.");
#endif

                currentstate = LaneManagerState::STOPPED;
                stoppingBool = false;
            }
        }

        for (auto &i : laneManagerLanes)
        {
            i->handleState();
        }

        for (int i = 0; i < laneManagerButtons.size(); i++)
        {
            currentDelta = laneManagerButtons[i]->detectButtonPress(currentDelta);
        }

        if (currentDelta != 0)
        {
            switch (currentDelta)
            {

                // TODO remove this switch in the future as it's not needed.

            case CAR_DELTA_MINUS:
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("LaneManager.cpp, handler(): delta -");
#endif

                currentLane->newCarCountDelta(currentDelta);
                pointsObserver.receiveUpdatedPoints(getTotalCarCount());
            }
            break;

            case CAR_DELTA_PLUS:
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("LaneManager.cpp, handler(): delta +");
#endif

                currentLane->newCarCountDelta(currentDelta);
                pointsObserver.receiveUpdatedPoints(getTotalCarCount());
                prevEmpty = false;
            }
            break;

            case CAR_DELTA_CHANGE:
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("LaneManager.cpp, handler(): delta switch");
#endif

                currentLaneNum++;
                if (currentLaneNum >= laneManagerLanes.size())
                {
                    currentLaneNum = 0;
                }

                for (const auto &i : laneManagerButtons)
                {
                    i->editLane(laneManagerLanes[currentLaneNum]);
                }

                currentLane = laneManagerLanes[currentLaneNum];

                // Change the current lane indication LED:
                for (const auto &i : activePins)
                {
#ifdef ARDUINO_ARCH_ESP32
                    digitalWrite(i, LOW);
#endif
                }
#ifdef ARDUINO_ARCH_ESP32
                digitalWrite(activePins[currentLaneNum], HIGH);
#endif
            }
            break;

                break;

            default:
                break;
            }
            currentDelta = 0;
            if (getTotalCarCount() == 0 && prevEmpty == false)
            {
                prevEmpty == true;
                for (auto &i : laneManagerLanes)
                {
                    i->toggleState(LaneState::STOPPED_DETERMINE);
                }
            }
        }
    }
    int LaneManager::getTotalCarCount()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, getTotalCarCount(): Start of function");
#endif

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, getTotalCarCount(): Get total car count of all lanes");
#endif

        int totalCarCount = 0;
        for (const auto &i : laneManagerLanes)
        {
            totalCarCount += i->getCarCount();
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("LaneManager.cpp, getTotalCarCount(): End of function");
#endif

        return totalCarCount;
    }
}
