#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif

#include "DecisionManagement/DecisionAckReturner.hpp"
#include "DecisionManagement/NodeInfo.hpp"

namespace DecisionManagement {
    
    /* Private: */
    Message_t DecisionAckReturner::createAckMessageFromDecision(Decision_t& decision) {
        uint8_t ownNodeID = NodeInfo::getCurrentNodeID();
        Message_t ackMessage;
        ackMessage.type = Message_type::Acknowledgement;

        #ifdef ARDUINO_ARCH_ESP32
        Serial.print("constructing ack message for node ");
        Serial.println(decision.fromNodeID);
        #endif

        // Setup sender (this) and recipient MAC addresses:

        uint8_t* ownMAC = NodeInfo::getNodeMAC(ownNodeID);
        std::copy(ownMAC, ownMAC + MAC_SIZE, ackMessage.sourceMAC);

        uint8_t* recipientMAC = NodeInfo::getNodeMAC(decision.fromNodeID);
        #ifdef ARDUINO_ARCH_ESP32
        std::copy(recipientMAC, recipientMAC + MAC_SIZE, ackMessage.destinationMAC);
        #endif

        return ackMessage;
    }
    
    void DecisionAckReturner::sendAcknowledgementMessage(Message_t ackMessage) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("DAR: Sending acknowledgement to observants");
        #endif
        outgoingMessage.receiveMessage(ackMessage);
    }

    /* Public: */

    void DecisionAckReturner::receiveDecisionToAcknowledge(Decision_t decisionToAck) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("DAR: Received a decision to create ack back for");
        #endif
        Message_t AckMessage = createAckMessageFromDecision(decisionToAck);

        sendAcknowledgementMessage(AckMessage);
    }

} // namespace DecisionManagement
