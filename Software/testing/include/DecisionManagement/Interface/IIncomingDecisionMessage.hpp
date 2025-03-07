#pragma once

#include "DecisionManagement/Datatype/Message.hpp"

namespace DecisionManagement {

    class IIncomingDecisionMessage {
        public:
            virtual void receiveDecisionMsg(Message_t msg) = 0;
    };

} // namespace DecisionManagement
