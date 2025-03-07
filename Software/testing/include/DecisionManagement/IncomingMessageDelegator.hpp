#pragma once
#include "DecisionManagement/IncomingDecisionHandler.hpp" // currently concrete class, make interface later
#include "DecisionManagement/NodeDecisionAcknowledgementHandler.hpp"
#include "DecisionManagement/Interface/IIncomingExternalMessage.hpp"

namespace DecisionManagement {
    
    class IncomingMessageDelegator : public IIncomingExternalMessage {
        private:
            IncomingDecisionHandler& decisionHandlerObserver;
            NodeDecisionAcknowledgementHandler& acknowledgementHandlerObserver;
        protected:
            void delegateIncomingMessage(Message_t msg);

        public:
            IncomingMessageDelegator(IncomingDecisionHandler& idh, NodeDecisionAcknowledgementHandler& aho)
            : decisionHandlerObserver(idh), acknowledgementHandlerObserver(aho) {};

            // TODO in the future, connect this to the interface
            void receiveExternalMessage(Message_t msg) override;
    };

} // namespace DecisionManagement