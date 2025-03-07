#pragma once

#include "DecisionManagement/NodeDecisionManager.hpp"
#include "DecisionManagement/Datatype/Message.hpp"
#include "DecisionManagement/Interface/IIncomingAcknowledgement.hpp"
#include "DecisionManagement/Interface/IIncomingDecisionAcknowledgement.hpp"

namespace DecisionManagement {
    
    class NodeDecisionAcknowledgementHandler : public IIncomingAcknowledgement {
        private:
            IDecisionAcknowledgementObserver& incomingdecisionAcknowledgement;

            Decision_t pendingDecision;
            int ackCount = 0; // currently very simple and hard coded.
            void notifyAckObservers(Decision_t acknowledgedDecision);
        public:

            NodeDecisionAcknowledgementHandler(IDecisionAcknowledgementObserver& idao) : incomingdecisionAcknowledgement(idao) {};

            void ReceiveDecisionAckMessage(Message_t incomingMessage) override;
            void newAcknowledgementAwaital(Decision_t newPendingDecision);
    };

} // namespace DecisionManagement