#pragma once

#include "DecisionManagement/Datatype/Decision.hpp"

namespace DecisionManagement
{
    class IIncomingDecision
    {
        public:
            virtual void receiveIncomingDecision(Decision_t incomingDecision) = 0;
    };

} // namespace DecisionManagement
