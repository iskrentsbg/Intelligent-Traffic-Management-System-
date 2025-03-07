
#include <cstring>
#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif

#include "DecisionManagement/OutgoingMessageDelegator.hpp"

namespace DecisionManagement {

    /* Private: */

    void OutgoingMessageDelegator::delegateMessage(Message_t message) {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Delegating message to external message protocol...");
#endif
        externalSend.receiveOutgoingExternalMessage(message);
    }

    /* Public: */
    
    void OutgoingMessageDelegator::receiveMessage(Message_t message) {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("OMD: Received message to delegate");
#endif
        delegateMessage(message);
    }

} // namespace DecisionManagement