#pragma once

namespace DecisionManagement
{
    class IIncomingPoints
    {
        public:
            virtual void receiveUpdatedPoints(int points) = 0;
    };  
} // namespace DecisionManagement