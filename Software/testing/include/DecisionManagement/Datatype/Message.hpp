#pragma once
#include <array>
#include <cstdint>

/// @todo The max argument length should not really be defined here, move this to the future 
#define ARGUMENTS_MAXLEN 32
#define MAC_SIZE 6

namespace DecisionManagement {

    enum class Message_type {
        Decision,
        Acknowledgement,
        // TODO implement shush, etc.
    };

    typedef struct {
        Message_type type;
        char arguments[ARGUMENTS_MAXLEN];
        uint8_t sourceMAC[MAC_SIZE];
        uint8_t destinationMAC[MAC_SIZE];
    } Message_t;

} // namespace DecisionManagement