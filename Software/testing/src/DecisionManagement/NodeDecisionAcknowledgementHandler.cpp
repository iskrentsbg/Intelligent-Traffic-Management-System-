
#include "DecisionManagement/NodeDecisionAcknowledgementHandler.hpp"
#include "DecisionManagement/NodeInfo.hpp"

namespace DecisionManagement {
    
    /* Private: */

    void NodeDecisionAcknowledgementHandler::notifyAckObservers(Decision_t acknowledgedDecision) {
        // Currently only one observer, which is the NodeDecisionManager:
        incomingdecisionAcknowledgement.confirmDecisionAcknowledgement();
    }

    /* Public: */

    void NodeDecisionAcknowledgementHandler::ReceiveDecisionAckMessage(Message_t incomingMessage) {
        ackCount++; // this is temporary and will be replaced with (1) checking and (2) ack object
        if (ackCount == NodeInfo::getNodeAmount() - 1) { // all nodes but this one need to acknowledge.
            notifyAckObservers(pendingDecision);
            ackCount = 0;
        }
    }

    void NodeDecisionAcknowledgementHandler::newAcknowledgementAwaital(Decision_t newPendingDecision) {
        ackCount = 0; // Todo change this to robust ack object in the future
        pendingDecision = newPendingDecision;
    }

} // namespace DecisionManagement