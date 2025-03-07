#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif
#include "DecisionManagement/IncomingMessageDelegator.hpp"

namespace DecisionManagement {

    /* Private: */

    void IncomingMessageDelegator::delegateIncomingMessage(Message_t msg) {
        
        switch(msg.type) {
            case Message_type::Decision: {
                #ifdef ARDUINO_ARCH_ESP32
                Serial.println("IMD: Delegating message to decision message observant");
                #endif
                decisionHandlerObserver.receiveDecisionMsg(msg);
            } break;

            case Message_type::Acknowledgement: {
                #ifdef ARDUINO_ARCH_ESP32
                Serial.println("IMD: Delegating message to Acknowledgment message observant");
                #endif                
                acknowledgementHandlerObserver.ReceiveDecisionAckMessage(msg);
            } break;

            default: {
                #ifdef ARDUINO_ARCH_ESP32
                Serial.println("IMD: Received default message, this should not happen");
                #endif
                // This should not happen
            }
            // Add new delegation classes when needed.
        }
    }

    /* Public: */

    void IncomingMessageDelegator::receiveExternalMessage(Message_t msg) {
        #ifdef ARDUINO_ARCH_ESP32
        Serial.println("IMD: Received external message");
        #endif
        delegateIncomingMessage(msg);
    }

} // namespace DecisionManagement