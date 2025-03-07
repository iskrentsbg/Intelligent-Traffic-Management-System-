#pragma once

#include "DecisionManagement/Datatype/Decision.hpp"

namespace DecisionManagement {

    class IOutgoingDecision {
        public:
            virtual void receiveOutgoingDecision(Decision_t outgoingDecision) = 0;
    };

} // namespace DecisionManagement