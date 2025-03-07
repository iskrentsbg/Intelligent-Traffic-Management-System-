#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement
{
    class IIncomingAcknowledgement {
        public:
            virtual void ReceiveDecisionAckMessage(Message_t incomingMessage) = 0;
    };
    
} // namespace DecisionManagement