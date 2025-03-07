#pragma once

#include "DecisionManagement/Datatype/Message.hpp"
#include "DecisionManagement/Interface/IOutgoingMessage.hpp"
#include "DecisionManagement/Interface/IOutgoingExternalMessage.hpp"

namespace DecisionManagement {

    class OutgoingMessageDelegator : public IOutgoingMessage {
        private:
            IOutgoingExternalMessage& externalSend;

            void delegateMessage(Message_t message);

        public:
            OutgoingMessageDelegator(IOutgoingExternalMessage& IOEM) : externalSend(IOEM) {};

            void receiveMessage(Message_t outMessage) override;
    };
} // namespace DecisionManagement