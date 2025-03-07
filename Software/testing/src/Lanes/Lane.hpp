#include "IFlowable.hpp"
#include "ILane.hpp"
#include "LaneState.hpp"
#include "ILaneCount.hpp"
#include "TrafficLights\TrafficLightManager.hpp"
#pragma once
namespace Lanes
{
    class Lane : public ILane
    {
    private:
        int CarCount = 0;
        LaneState currentstate;
        TrafficLights::TrafficLightManager trafficLightManager;
        bool stateChangeFlag = false;
        unsigned long currentTime = 0;
        bool prevGreen = false;
    public:
        Lane(int pinGreen, int pinOrange, int pinRed);
        void toggleState(LaneState state);
        int newCarCount(int count);
        void newCarCountDelta(int delta);
        void handleState();
        int getCarCount();
    };
}