/*
 * ESPNOW header file which contains the function bodies for using ESPNOW protocol in mesh
 *
 * Developed by: Adrián Pacera, Iskren Zhechev (T3-CB02)
*/
#pragma once

#ifdef ARDUINO_ARCH_ESP32
#include <esp_now.h>
#include <WiFi.h>
#endif

#include "DecisionManagement/Datatype/Message.hpp"

#include "DecisionManagement/Interface/IIncomingExternalMessage.hpp"

#define MAC_SIZE 6

namespace NodeCommunication {

    void changeExternalSendObserver(DecisionManagement::IIncomingExternalMessage* newExternalSend);

    #ifdef ARDUINO_ARCH_ESP32
    void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
    int ESPNOWAddPeer(esp_now_peer_info peer, uint8_t MAC[]);

    void ESPNOWInitialize();
    int sendProtocolMessage(DecisionManagement::Message_t message);
    #endif
}