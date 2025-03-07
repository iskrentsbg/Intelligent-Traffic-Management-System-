/*
 * ExternalMessageManager source file which contains the implementation of functions from header file ExternalMessageManager.hpp
 * 
 * Developed by: Adrián Pacera, Iskren Zhechev. Modified by Wouter Swinkels.
 */

#include "NodeCommunication/ExternalMessageManager.hpp"
#include <iostream>

namespace NodeCommunication {

    /* Private: */

    void ExternalMessageManager::enqueueMessage(const DecisionManagement::Message_t& message) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Entry enqueueMessage");
        #endif
        outgoingMessageQueue.push(message);
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Exit enqueueMessage");
        #endif
    }

    bool ExternalMessageManager::dequeueMessage(DecisionManagement::Message_t &message) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Entry dequeueMessage");
        #endif

        if (!outgoingMessageQueue.empty()) {
            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("NCOM: Queue is not empty, pop & return true");
            #endif

            message = outgoingMessageQueue.front();
            outgoingMessageQueue.pop();

            #ifdef ARDUINO_ARCH_ESP32
            Serial.println("NCOM: Exit dequeueMessage/success");
            #endif
            return true; // TODO magic number/boolean, replace with meaningful value.
        }
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Exit dequeue message: queue empty");
        #endif
        return false; // TODO magic number/boolean, replace with meaningful value.
    }

    int ExternalMessageManager::sendMessage(const DecisionManagement::Message_t &message) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Entry sendMessage");
        #endif
        enqueueMessage(message);
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: Message enqueued");
        #endif

        DecisionManagement::Message_t frontMessage = outgoingMessageQueue.front();
        #ifdef ARDUINO_ARCH_ESP32
        int sendResult = sendProtocolMessage(frontMessage);

        if (sendResult != ESP_OK) { // TODO Magic number, change with clarifying name
            Serial.println("NCOM: message failed to send");
            Serial.print("Error ID is: ");
            Serial.println(sendResult, HEX);
            return -1; // TODO Magic number, change with clarifying name
        }

        Serial.println("NCOM: message sent successfully, now popping");
        #endif
        outgoingMessageQueue.pop();
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("NCOM: message popped");
        Serial.println("NCOM: exit sendMessage");
        #endif

        return 0; // TODO Magic number, change with clarifying name
    }

    /* Public: */

    ExternalMessageManager::ExternalMessageManager(DecisionManagement::IIncomingExternalMessage& IIOM) : incomingMessageObserver(IIOM) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("Entry EMM constructor");
        // Initialise the I/O layer:
        ESPNOWInitialize();
        
        Serial.println("Exit EMM constructor");
        #endif
    }

    void ExternalMessageManager::receiveIncomingExternalMessage(DecisionManagement::Message_t &message) {
        incomingMessageObserver.receiveExternalMessage(message);
    }

    void ExternalMessageManager::receiveOutgoingExternalMessage(DecisionManagement::Message_t outMessage) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("EMM: received external message");
        #endif
        sendMessage(outMessage);
    }
}
