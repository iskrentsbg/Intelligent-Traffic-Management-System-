#pragma once

using namespace std;
#include <vector>

#include "DecisionManagement/Datatype/Message.hpp"
#include "DecisionManagement/NodeDecisionManager.hpp"
#include "DecisionManagement/Interface/IOutgoingDecision.hpp"

#include "DecisionManagement/Interface/IOutgoingMessage.hpp"

namespace DecisionManagement {
    
    class OutgoingDecisionHandler : public IOutgoingDecision {
        private:

            IOutgoingMessage& outgoingMessage;

            Message_t serializeDecisionToMessage(Decision_t& decisionToSerialize);
            void sendDecisionMessages(std::vector<Message_t> decisionMessages);           // send a collection of messages
            std::vector<Message_t> convertDecisionToMessages(Decision_t outoingDecision); // converts decision to message(s) for other nodes

        public:
            OutgoingDecisionHandler(IOutgoingMessage& IOM) : outgoingMessage(IOM) {};

            void receiveOutgoingDecision(Decision_t outgoingDecision) override;
    };

} // namespace DecisionManagement