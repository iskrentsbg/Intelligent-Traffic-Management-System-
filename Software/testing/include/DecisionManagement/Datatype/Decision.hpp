#pragma once

namespace DecisionManagement
{

    typedef enum {
        NEWPOINTS,
        NEXTREQUEST,
        STARTFLOWREQUEST,
        STOPPEDNOTIFY,
        EMPTYDECISION    // used internally to indicate the absence of a decison
    } DecisionTopic;

    typedef struct {
        DecisionTopic topic;
        int fromNodeID;
        int argument; // Is not used for most cases, but can be used.

    } Decision_t;

} // namespace DecisionManagement