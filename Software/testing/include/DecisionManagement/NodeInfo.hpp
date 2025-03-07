/*
 * NodeInfo header file, this is a temporary header file for converting nodes between their
 * corresponding ID and their MAC address; this will be done through
 * Author: Wouter Swinkels
*/

#pragma once

#include <array>
#include <memory>
#include <algorithm> // for std::copy

// TODO move this out of the DecisionManagement namespace?
namespace DecisionManagement {

    // Hardcoded node ID for this node; will have to change when compiling+uploading to a node
    static const uint8_t ThisNodeID = 1;
    static const uint8_t NodeIDs[] = {0, 1};

    // Hardcoded node MAC addresses.
    // Will eventually be done 

    static const uint8_t MAC_1[] = {0x78, 0xE3, 0x6D, 0x09, 0xE1, 0xD0}; // North
    static const uint8_t MAC_2[] = {0x94, 0xB9, 0x7E, 0xE6, 0x2E, 0xA4}; // South
    //static const uint8_t MAC_3[] = {0xB0, 0xB2, 0x1C, 0x0A, 0xF3, 0x88}; // West
    //static const uint8_t MAC_4[] = {0xC8, 0xF0, 0x9E, 0x75, 0x28, 0x08}; // East

class NodeInfo {
    private:
        static uint8_t NodeMACs[2][6]; // 2D array to store MAC addresses

    public:
        NodeInfo() {
            std::copy(std::begin(MAC_1), std::end(MAC_1), NodeMACs[0]);
            std::copy(std::begin(MAC_2), std::end(MAC_2), NodeMACs[1]);
            //std::copy(std::begin(MAC_3), std::end(MAC_3), NodeMACs[2]);
            //std::copy(std::begin(MAC_4), std::end(MAC_4), NodeMACs[3]);
        }

        static int getCurrentNodeID() {
            return ThisNodeID;
        }

        static uint8_t* getNodeMAC(uint8_t nodeID) {
            if (nodeID >= 0 && nodeID < getNodeAmount()) {
                return NodeMACs[nodeID]; // Return the address of the MAC array for the given nodeID
            } else {
                return nullptr; // Return null pointer for invalid nodeID
            }
        }

        static int getNodeAmount() {
            return sizeof(NodeIDs)/sizeof(NodeIDs[0]);
        }
};

} // namespace DecisionManagement