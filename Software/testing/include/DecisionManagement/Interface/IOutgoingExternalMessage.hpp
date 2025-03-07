#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement {
    
    class IOutgoingExternalMessage {
        public:
            virtual void receiveOutgoingExternalMessage(Message_t outMessage) = 0;
    };

} // namespace DecisionManagement