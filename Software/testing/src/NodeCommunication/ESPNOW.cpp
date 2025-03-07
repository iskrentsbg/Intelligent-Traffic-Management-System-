/*
 * ESPNOW source file which implements the functions in ESPNOW.hpp
 *
 * Developed by: Adrián Pacera, Iskren Zhechev, modified by Wouter Swinkels.
 * 
*/

#include <Arduino.h>

#include "NodeCommunication/ESPNOW.hpp"
#include "NodeCommunication/ExternalMessageManager.hpp"


#include "DecisionManagement/NodeInfo.hpp"

using namespace std;

namespace NodeCommunication {

    DecisionManagement::IIncomingExternalMessage* externalSend = nullptr;

    // TODO this is already in NodeInfo
    // Still needed
    uint8_t MAC_N[] = {0x78, 0xE3, 0x6D, 0x09, 0xE1, 0xD0};
    uint8_t MAC_S[] = {0x94, 0xB9, 0x7E, 0xE6, 0x2E, 0xA4};
    uint8_t MAC_W[] = {0xB0, 0xB2, 0x1C, 0x0A, 0xF3, 0x88};
    uint8_t MAC_E[] = {0xC8, 0xF0, 0x9E, 0x75, 0x28, 0x08};

    // TODO move this to another intialization class, maybe main.cpp first.
    // This can go to NodeAdministrationManagement later.

    esp_now_peer_info_t peerInfo_N;
    esp_now_peer_info_t peerInfo_S;
    esp_now_peer_info_t peerInfo_W;
    esp_now_peer_info_t peerInfo_E;

    void changeExternalSendObserver(DecisionManagement::IIncomingExternalMessage* newExternalSend) {
        Serial.println("Assigning new external send");
        externalSend = newExternalSend;
    }
    
    // ESPNow library interrupt function, triggered when sending any message using <esp_now.h>
    void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
        
        if (status == ESP_NOW_SEND_SUCCESS) {
            Serial.print("Succesfully sent message to node with address ");
        } else {
            Serial.print("Failed to send message to node with address ");
        }
        for (int i = 0; i < 6; i++) {
            Serial.print(mac_addr[i], HEX);
        }
        Serial.println();

    }

    // ESPNow library interrupt function, triggered on any message receival from <esp_now.h>, for
    // any added peer.
    void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

        // TODO setup creating an instance of Message_t
        DecisionManagement::Message_t newRecievedMessage;
        
        // C-bound memcpy() has to stay due to ESPNow library arguments, using `const uint8_t`.
        // The arguments are not inherently type-castable as the data structure is not known inherently.
        memcpy(&newRecievedMessage, incomingData, sizeof(newRecievedMessage));

        // TODO debug message (renmove later or use logging):
        Serial.print("Message received from ");
        for (int i = 0; i < 6; i++) {
            Serial.print(newRecievedMessage.sourceMAC[i]);
            Serial.print(" ");
        }

        // TODO send the message to the ExternalMessageManager:
        externalSend->receiveExternalMessage(newRecievedMessage);
    }

    int ESPNOWAddPeer(esp_now_peer_info peer, uint8_t mac_addr[]){
        // this memory copy can be potentially changed to a more cpp function, however std::copy 
        // didnt function properly, reason is unknown
        memcpy(peer.peer_addr, mac_addr, 6);
        peer.channel = 0;  
        peer.encrypt = false;

        if(esp_now_add_peer(&peer) != ESP_OK ){
            return -1;
        }   

        return 0; 
    }

    // TODO magic numbers, make more clear
    int wifiSetup(){
        WiFi.mode(WIFI_STA);

        if (esp_now_init() != ESP_OK) {
            return -1;
        }

        return 0;
    }

    void ESPNOWInitialize() {
        wifiSetup();

        // TODO move this to a different setup, as this
        // is currently hard coded and not really neat.

        ESPNOWAddPeer(peerInfo_N, MAC_N);
        ESPNOWAddPeer(peerInfo_S, MAC_S);
        ESPNOWAddPeer(peerInfo_W, MAC_W);
        ESPNOWAddPeer(peerInfo_E, MAC_E);

        esp_now_register_send_cb(OnDataSent);
        esp_now_register_recv_cb(OnDataRecv);
    }

    // TODO magic numbers, make more clear
    int sendProtocolMessage(DecisionManagement::Message_t message) {
        esp_err_t sendResult = esp_now_send(message.destinationMAC, (uint8_t *) &message, sizeof(message));
        return sendResult;
    }
} 