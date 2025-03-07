#include <vector>

#include "ILane.hpp"
#include "Lane.hpp"
#include "Lanes\ButtonSensor.hpp"
#include "LaneManagerState.hpp"

#include "DecisionManagement/Interface/IFlowable.hpp"
#include "DecisionManagement\Interface\IIncomingPoints.hpp"
#include "DecisionManagement/Interface/IControlsFlow.hpp"
#pragma once

using namespace std;

namespace Lanes
{
    class LaneManager : public DecisionManagement::IFlowable 
    {
    private:
        LaneManagerState currentstate;
        Lanes::Lane *currentLane;
        DecisionManagement::IIncomingPoints& pointsObserver;
        DecisionManagement::IControlsFlow& flowController;

        vector<Lanes::Lane*> laneManagerLanes;
        vector<Lanes::ButtonSensor*> laneManagerButtons;
        vector<int> activePins;

        bool prevEmpty = true;

        int currentLaneNum = 0;

        int currentDelta = 0;

        unsigned long currentTime = 0;
        
        bool stoppingBool = false;
    public:
        LaneManager(vector<Lanes::Lane*> *myLanes,vector<Lanes::ButtonSensor*> *myButtons,vector<int> activePins, 
                    DecisionManagement::IIncomingPoints& IIP, DecisionManagement::IControlsFlow& IFC);
        void handler() override;
        void Flow() override;
        void StopFlow() override;
        int getTotalCarCount();
    };
}
