#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement {

    class IIncomingDecisionAcknowledgement {
        public:
            virtual void receiveDecisionAckmsg(Message_t msg) = 0;
    };

} // namespace DecisionManagement