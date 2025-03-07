#define BAUDRATE 115200

#include <Arduino.h>

// Debug to test message sending:
#include <iostream>
#include <sstream>
#include <iomanip>

/* Node Communication Namespace: */
#include "NodeCommunication/ExternalMessageManager.hpp"
#include "NodeCommunication/ESPNOW.hpp"

/* Decision Namespace: */

    /* Incoming stuff:*/
    #include "DecisionManagement/IncomingMessageDelegator.hpp"
    #include "DecisionManagement/IncomingDecisionHandler.hpp"
    #include "DecisionManagement/NodeDecisionAcknowledgementHandler.hpp"

    /* Management-related stuff: */
    #include "DecisionManagement/NodeDecisionManager.hpp"
    #include "DecisionManagement/CarCountToPointsConverter.hpp"

    /* Outgoing stuff: */
    #include "DecisionManagement/OutgoingDecisionHandler.hpp"
    #include "DecisionManagement/DecisionAckReturner.hpp"
    #include "DecisionManagement/OutgoingMessageDelegator.hpp"

#include "DecisionManagement/NodeInfo.hpp"

/* Lanes Namespace: */
#include "Lanes/LaneManager.hpp"
#include "Lanes/Lane.hpp"
#include "Lanes/ButtonSensor.hpp"

/* Traffic Light namespace: */
// #include "TrafficLights/TrafficLightManager.hpp"

/*---------------------*/
/* Object Definitions: */
/*---------------------*/

/* Node Communication: */
NodeCommunication::ExternalMessageManager* messageManager = nullptr;

/* Decision Management: */

DecisionManagement::NodeInfo* nodeInfo = nullptr;

DecisionManagement::IncomingMessageDelegator* incomingMessageDelegator = nullptr;
DecisionManagement::IncomingDecisionHandler* incomingDecisionHandler = nullptr;
DecisionManagement::NodeDecisionAcknowledgementHandler* incomingAckHandler = nullptr;

DecisionManagement::NodeDecisionManager* nodeDecisionManager = nullptr; // The class where the magic happens
DecisionManagement::CarCountToPointsConverter* carCountToPointsConverter = nullptr;

DecisionManagement::OutgoingDecisionHandler* outgoingDecisionHandler = nullptr;
DecisionManagement::DecisionAckReturner* decisionAckReturner = nullptr;
DecisionManagement::OutgoingMessageDelegator* outgoingMessageDelegator = nullptr;

/* Lanes: */

const int LightPins[3][3] 
{
{14, 12, 13}, // Traffic light pins for lane 1
{25, 26, 27}, // Traffic light pins for lane 2
{17, 32, 33}  // Traffic light pins for lane 3
};
vector<int> activePins {5,22,23};
int totalCarCount;

Lanes::Lane* LaneOne = nullptr;
Lanes::Lane* LaneTwo = nullptr;
Lanes::Lane* LaneThree = nullptr;

Lanes::ButtonSensor* BtnUp = nullptr;
Lanes::ButtonSensor* BtnDown = nullptr;
Lanes::ButtonSensor* BtnSwitch = nullptr;
Lanes::LaneManager* laneManager = nullptr;

/*----------------*/
/* Functionality: */
/*----------------*/

void setupNodeCommunication() {
    Serial.println("Main.cpp: Setting up ExternalMessageManager");
    // TODO check whether this pointer works, as it's given before
    // heap initialization
    messageManager = new NodeCommunication::ExternalMessageManager(*incomingMessageDelegator);
    Serial.println("Main.cpp: Done setting up ExternalMessageManager");
}

// This should be setup lastly.
void setupDecisionManagement() {
    Serial.println("Main.cpp: Setting up Decision Management namespace");

    nodeInfo = new DecisionManagement::NodeInfo();
    
    // Outgoing stuff:
    outgoingMessageDelegator = new DecisionManagement::OutgoingMessageDelegator(*messageManager);
    decisionAckReturner = new DecisionManagement::DecisionAckReturner(*outgoingMessageDelegator);
    outgoingDecisionHandler = new DecisionManagement::OutgoingDecisionHandler(*outgoingMessageDelegator);

    // Management stuff:
    nodeDecisionManager = new DecisionManagement::NodeDecisionManager(*outgoingDecisionHandler, *decisionAckReturner);

    // Incoming Stuff:
    incomingDecisionHandler = new DecisionManagement::IncomingDecisionHandler(*nodeDecisionManager);
    incomingAckHandler = new DecisionManagement::NodeDecisionAcknowledgementHandler(*nodeDecisionManager);
    incomingMessageDelegator = new DecisionManagement::IncomingMessageDelegator(*incomingDecisionHandler, *incomingAckHandler);
    Serial.println("Main.cpp: Done setting up Decision Management namespace");

}

void setupLaneManagement() {
    Serial.println("Main.cpp: Start setting up Lane Management");
    LaneOne = new Lanes::Lane(LightPins[0][0], LightPins[0][1], LightPins[0][2]);
    LaneTwo = new Lanes::Lane(LightPins[1][0], LightPins[1][1], LightPins[1][2]);
    LaneThree = new Lanes::Lane(LightPins[2][0], LightPins[2][1], LightPins[2][2]);

    vector<Lanes::Lane*> laneManagerLanes{LaneOne,LaneTwo,LaneThree};

    /// @todo maybe separate the individual button pins from here (no magic numbers)?
    BtnUp = new Lanes::ButtonSensor(21,1, LaneOne);
    BtnDown = new Lanes::ButtonSensor(19,-1, LaneOne);
    BtnSwitch = new Lanes::ButtonSensor(18,2, LaneOne);

    vector<Lanes::ButtonSensor*> laneManagerButtons{BtnUp,BtnDown,BtnSwitch};

    laneManager = new Lanes::LaneManager(&laneManagerLanes, &laneManagerButtons, activePins, *nodeDecisionManager, *nodeDecisionManager);
    Serial.println("Main.cpp: Done setting up Lane Management namespace");
}

/* 
 * Below is some testing behaviour to send example messages from here
 * to the incoming message delegator.
*/

unsigned long long tempReceiveDelay = 5000;
#include "DecisionManagement/Datatype/Message.hpp"

// helper Function to convert a two-char hex string to a byte
// Used for sending test message
char hexToByte(const char *hex) {
    return (char) strtol(hex, nullptr, 16);
}


// Function to give an example message to the incoming message delegator for testing.
void DN_send_example_messages() {

    unsigned long long currentTime = millis();
    if (tempReceiveDelay <= currentTime) {
        Serial.println("MAIN: test for sending msg");
        // 1. Send an acknowledgememt message, this will increase the counter

        DecisionManagement::Message_t testReceivedMessage;
        testReceivedMessage.type = DecisionManagement::Message_type::Decision;
        char ownDestinationMAC[6];
        memcpy(ownDestinationMAC, DecisionManagement::NodeInfo::getNodeMAC(DecisionManagement::NodeInfo::getCurrentNodeID()), 6);
        Serial.println("own MAC address from NodeInfo: ");
        for (int i = 0; i < 6; i++) {
            Serial.print(ownDestinationMAC[i], HEX); // This is correct.
        }
        Serial.println();

        // Example message to send:
        // NEWPOINTS message from node 2 with argument of 1:
        std::string hexStr = "000000005F010000005F01000000";

        // Convert and append bytes to the arguments field:
        for (int i = 0; hexStr[i] != '\0' && i < 32; i += 2) {
            char byteStr[3] = {hexStr[i], hexStr[i + 1], '\0'};
            testReceivedMessage.arguments[i / 2] = hexToByte(byteStr);
        }

        // Print the `arguments` bytes for verification here:
        Serial.print("Arguments: ");
        for (int i = 0; i < 32; i++) {
            Serial.print(testReceivedMessage.arguments[i], HEX);
            Serial.print(" ");
        }
        Serial.println();

        incomingMessageDelegator->receiveExternalMessage(testReceivedMessage);

        //incomingMessageDelegator->receiveExternalMessage();
        // 5 second delay for doing this activity
        tempReceiveDelay = currentTime + 5000; 
    }
}

void setup() {
    Serial.begin(BAUDRATE);

    // Setting up the individual namespaces:
    setupNodeCommunication(); 
    setupDecisionManagement();
    setupLaneManagement();

    // Add any runtime observants:
    nodeDecisionManager->addFlowableObserver(laneManager);
    NodeCommunication::changeExternalSendObserver(incomingMessageDelegator);
}

void loop() {
    // Run continuously for updated car count notifications:
    laneManager->handler();

    // DN_send_example_messages(); // send messages to incoming message delegator
}