#pragma once

#include <vector>
using std::vector;

#include "DecisionManagement/OutgoingMessageDelegator.hpp"
#include "DecisionManagement/Datatype/Decision.hpp"
#include "DecisionManagement/Datatype/Message.hpp"

#include "DecisionManagement/Interface/IReturnDecisionAcknowledgement.hpp"

namespace DecisionManagement
{

    class DecisionAckReturner : public IReturnDecisionAcknowledgement
    {
    private:
        
    protected:
        Message_t createAckMessageFromDecision(Decision_t &decision);
        virtual void sendAcknowledgementMessage(Message_t ackMessage);
        

    public:
        /// @todo constructor
        DecisionAckReturner(IOutgoingMessage &iom) : outgoingMessage(iom){};
        IOutgoingMessage &outgoingMessage;
        void receiveDecisionToAcknowledge(Decision_t decisionToAcknowledge) override;
    };

} // namespace DecisionManagement