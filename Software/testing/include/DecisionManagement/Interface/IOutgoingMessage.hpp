#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement {
    
    class IOutgoingMessage {
        public:
            virtual void receiveMessage(Message_t outMessage) = 0;
    };

} // namespace DecisionManagement