#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement {

    class IIncomingExternalMessage {
        public:
            virtual void receiveExternalMessage(Message_t msg) = 0;
    };

} // namespace DecisionManagement