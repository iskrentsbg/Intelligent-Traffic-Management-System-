/*
 * ExternalMessageManager header file which contains the functions used in system for communication between nodes
 *
 * Developed by: Adrián Pacera, Iskren Zhechev, modified by Wouter Swinkels.
 */
#pragma once

#include <queue>
#include "ESPNOW.hpp"

#include "DecisionManagement/Interface/IIncomingExternalMessage.hpp"
#include "DecisionManagement/Interface/IOutgoingExternalMessage.hpp"

namespace NodeCommunication {
    
    class ExternalMessageManager : public DecisionManagement::IOutgoingExternalMessage {
        private:
            std::queue<DecisionManagement::Message_t> incomingMessageQueue;
            std::queue<DecisionManagement::Message_t> outgoingMessageQueue;

            DecisionManagement::IIncomingExternalMessage& incomingMessageObserver;

            int sendMessage(const DecisionManagement::Message_t &message);
            void enqueueMessage(const DecisionManagement::Message_t &message);
            bool dequeueMessage(DecisionManagement::Message_t &message);

        public:
            ExternalMessageManager(DecisionManagement::IIncomingExternalMessage& IIMO);

            void receiveIncomingExternalMessage(DecisionManagement::Message_t &message);
            void receiveOutgoingExternalMessage(DecisionManagement::Message_t outMessage) override;
    };
}