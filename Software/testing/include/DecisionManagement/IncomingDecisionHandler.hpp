/*
 * Incoming decision handler, mediator responsible for:
 * 1. Converting an incoming message of type decision to a decision
 * 2. Delegating the converted decision to the node decision manager (would be an interface in the future)
 * Author: Wouter Swinkels
 */
#pragma once
#include "DecisionManagement/Interface/IIncomingDecisionMessage.hpp" // to receive from
#include "DecisionManagement/Datatype/Message.hpp"
#include "DecisionManagement/Interface/IIncomingDecision.hpp" // to send to

namespace DecisionManagement {
    
    class IncomingDecisionHandler : IIncomingDecisionMessage {
        private:
            IIncomingDecision& incomingDecision; // would be an inteface in the future

            Decision_t convertDecisionMsg(Message_t decisionMsg);
            void sendDecision(Decision_t newDecision); // to incomingDecision

        public:
            IncomingDecisionHandler(IIncomingDecision& IID) : incomingDecision(IID) {};

            void receiveDecisionMsg(Message_t decisionMessage) override;

    };
} // namespace DecisionManagement