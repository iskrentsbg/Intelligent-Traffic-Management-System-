
#include <sstream>
#include "DecisionManagement/NodeInfo.hpp"
#include "DecisionManagement/OutgoingDecisionHandler.hpp"
#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif

namespace DecisionManagement {

    /* Private: */

        /*
         * @brief serialize a decision to a message.
         *
         * Example of serialized message payload:
         * "00 00 00 00 5F 03 00 00 00 5F 01 00 00 00"
         * 
         * - The first field ('0 0 0 0') represents topic, i.e., an enum value of 0, i.e., the first possible argument
         * - The second field (3 0 0 0) represents 3, which is the node ID from this node
         * - The third field (1 0 0 0) represents the argument, in this case, being 1
         * - 5F is the underscore ('_') delimiter, in between every field
         * Note: This serialization conversion is therefore to raw bytes, not ASCII characters!
         * */
        Message_t OutgoingDecisionHandler::serializeDecisionToMessage(Decision_t& decisionToSerialize) {
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODM: Starting to serialize message");
            #endif
            Message_t constructedMessage;
            constructedMessage.type = Message_type::Decision;
            std::ostringstream ss;

            // Note: serialization uses Little-Endian!
            ss.write(reinterpret_cast<const char*>(&decisionToSerialize.topic), sizeof(decisionToSerialize.topic));
            ss << "_"; // Add underscore delimiter between every payload field
            ss.write(reinterpret_cast<const char*>(&decisionToSerialize.fromNodeID), sizeof(decisionToSerialize.fromNodeID));

            // Sender MAC is currently not provided in the message, as it's not needed for the MVP. This will likely just become
            // the node ID, as this makes more sense, to which the MAC handling can also be done in a separate class.
            //ss << "_";
            //ss.write(reinterpret_cast<const char*>(&decisionToSerialize.to), sizeof(decisionToSerialize.to));

            ss << "_";
            ss.write(reinterpret_cast<const char*>(&decisionToSerialize.argument), sizeof(decisionToSerialize.argument));

            std::string serializedData = ss.str();

            // Verbose debugging:
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("Serialized payload: ");
            for (size_t i = 0; i < serializedData.size(); ++i) {
                Serial.print((unsigned char)serializedData[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
            Serial.println("Payload size: ");
            Serial.println(serializedData.size());
            #endif

            size_t copySize = std::min(serializedData.size(), static_cast<size_t>(ARGUMENTS_MAXLEN - 1));
            std::copy(serializedData.begin(), serializedData.begin() + copySize, constructedMessage.arguments);
            constructedMessage.arguments[copySize] = '\0'; // add string termination

            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODM: Serialized message and returning");
            #endif
            return constructedMessage;
        }
 
        void OutgoingDecisionHandler::sendDecisionMessages(vector<Message_t> decisionMessages) {
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODM: Sending decision messages to delegator...");
            #endif
            for (auto message : decisionMessages) {
                #ifdef ARDUINO_ARCH_ESP32
                Serial.println("giving delegator a message");
                #endif
                outgoingMessage.receiveMessage(message);
            }
        }

        vector<Message_t> OutgoingDecisionHandler::convertDecisionToMessages(Decision_t outgoingDecision) {
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODM: Starting to convert a message");
            #endif
            uint8_t ownNodeID = NodeInfo::getCurrentNodeID();
            vector<Message_t> constructedMessages;
            #ifdef ARDUINO_ARCH_ESP32
            Serial.print("ODM: Current node ID is ");
            Serial.println(ownNodeID);
            #endif

            Message_t baseMessage = serializeDecisionToMessage(outgoingDecision);

            // Set source MAC address for base message, being this node's address:
            // TODO in the future, this field could be removed to just use the node ID provided in the payload.
            uint8_t* ownMAC = NodeInfo::getNodeMAC(ownNodeID);
            std::copy(ownMAC, ownMAC + MAC_SIZE, baseMessage.sourceMAC);

            // Construct the a message for all but this node, to send:
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("Constructing messages for other nodes...");
            #endif
            for (int nodeID = 0; nodeID < NodeInfo::getNodeAmount(); nodeID++) {
                if (nodeID != ownNodeID) {
                    #ifdef ARDUINO_ARCH_ESP32
                    Serial.print("constructing message for node ");
                    Serial.println(nodeID);
                    #endif
                    uint8_t *recipientMAC = NodeInfo::getNodeMAC(nodeID); 
                    std::copy(recipientMAC, recipientMAC + MAC_SIZE, baseMessage.destinationMAC);

                    constructedMessages.push_back(baseMessage);
                }
            }
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODM: exiting convertDecisionToMessage");
            #endif
            return constructedMessages;
        }

    /* Public: */

        void OutgoingDecisionHandler::receiveOutgoingDecision(Decision_t outgoingDecision) {
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("ODH: Received outgoing decision");
            #endif
            vector<Message_t> messages = convertDecisionToMessages(outgoingDecision);
            sendDecisionMessages(messages);
        }

} // namespace DecisionManagement