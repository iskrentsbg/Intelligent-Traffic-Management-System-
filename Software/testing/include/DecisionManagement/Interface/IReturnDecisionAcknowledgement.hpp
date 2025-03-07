#pragma once

#include "DecisionManagement/Datatype/Decision.hpp"

namespace DecisionManagement
{
    
    class IReturnDecisionAcknowledgement {
        public:
            virtual void receiveDecisionToAcknowledge(Decision_t decisionToAcknowledge) = 0;
    };

} // namespace DecisionManagement