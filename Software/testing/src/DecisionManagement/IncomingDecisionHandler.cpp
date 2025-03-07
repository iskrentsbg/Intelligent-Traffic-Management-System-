
#include <sstream>
#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif

#include "DecisionManagement/IncomingDecisionHandler.hpp"
#include "DecisionManagement/Datatype/Decision.hpp"

namespace DecisionManagement
{
    /* Private: */
        
    Decision_t IncomingDecisionHandler::convertDecisionMsg(Message_t decisionMsg) {
        Decision_t decision;
        std::istringstream ss(std::string(decisionMsg.arguments, 32));

        // Verbose testing first
        // Print all bytes to deserialize before debugging:
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("Serialized Data (Hex):");
        for (int i = 0; i < sizeof(decisionMsg.arguments); i++) {
            Serial.print("0x");
            if ((unsigned char)decisionMsg.arguments[i] < 16) {
                Serial.print("0"); // Print leading zero for single digit hex values
            }
            Serial.print((unsigned char)decisionMsg.arguments[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        #endif

        // Read each part of the decision structure (deserialize):
        ss.read(reinterpret_cast<char*>(&decision.topic), sizeof(decision.topic));
        ss.ignore(1); // Ignore underscore ('_') delimiter
        ss.read(reinterpret_cast<char*>(&decision.fromNodeID), sizeof(decision.fromNodeID));

        //ss.ignore(1); // Ignore underscore ('_') delimiter
        //ss.read(reinterpret_cast<char*>(&decision.toNode), sizeof(decision.toNode));
        
        ss.ignore(1); // Ignore underscore ('_') delimiter
        ss.read(reinterpret_cast<char*>(&decision.argument), sizeof(decision.argument));

        return decision;
    }

    void IncomingDecisionHandler::sendDecision(Decision_t newDecision) {
        incomingDecision.receiveIncomingDecision(newDecision);
    }

    /* Public: */

    void IncomingDecisionHandler::receiveDecisionMsg(Message_t decisionMessage) {
        Decision_t newDecision;
        newDecision = convertDecisionMsg(decisionMessage);
        #ifdef ARDUINO_ARCH_ESP32
        Serial.print("received incoming decision message");
        Serial.print(" with topic:");
        Serial.print(newDecision.topic);
        Serial.print(", from node ID:");
        Serial.print(newDecision.fromNodeID);
        Serial.print(" and with arguments:");
        Serial.print(newDecision.argument);
        #endif
        sendDecision(newDecision);
    }

} // namespace DecisionManagement