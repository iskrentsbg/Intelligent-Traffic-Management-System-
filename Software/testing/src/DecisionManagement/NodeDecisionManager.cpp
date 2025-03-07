#ifdef ARDUINO_ARCH_ESP32
#include <Arduino.h>
#endif

#include "DecisionManagement/NodeDecisionManager.hpp"
#include "DecisionManagement/NodeInfo.hpp"

namespace DecisionManagement
{

    /* Private: */

    int NodeDecisionManager::decideNextNode()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Starting to decide next node");
#endif
        // Node with the highest amount of points is determined as next.
        int highestNodePoints = currentNodePoints[0];
        int highestPointsNodeID = 0;

        int nodeAmount = NodeInfo::getNodeAmount();
#ifdef ARDUINO_ARCH_ESP32
        Serial.print("Total node amount is ");
        Serial.println(nodeAmount);
#endif

        for (int nodeID = 0; nodeID < nodeAmount; nodeID++)
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.print("node ");
            Serial.print(nodeID);
            Serial.print(" has ");
            Serial.print(currentNodePoints[nodeID]);
            Serial.println(" points.");
#endif
            // When a node is already flowing, it cannot be next.
            if (currentNodePoints[nodeID] > highestNodePoints && nodeID != currentNode)
            {
                highestPointsNodeID = nodeID;
                highestNodePoints = currentNodePoints[nodeID];
            }
        }

        if (highestNodePoints == 0)
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("DecideNextNode(): any point is zero, no one next");
#endif
            return -1; // no node is going to be next as no one has points at all.
        }

#ifdef ARDUINO_ARCH_ESP32
        Serial.print("NDM: DecideNextNode(): next node is ");
        Serial.println(highestPointsNodeID);
#endif
        return highestPointsNodeID;
    }

    void NodeDecisionManager::requestSelfToBeNext()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: New decision: Requesting self to be next.");
#endif
        Decision_t requestSelfNextDecision = {NEXTREQUEST, NodeInfo::getCurrentNodeID(), 0};
        sendNewOutgoingDecision(requestSelfNextDecision);
        currentOwnPendingDecision = {NEXTREQUEST, NONE, NONE};
    }

    void NodeDecisionManager::requestSelfToBeNow()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: New decision: Requesting self to be now.");
#endif
        Decision_t requestSelfNowdecision = {STARTFLOWREQUEST, NodeInfo::getCurrentNodeID(), 0};
        sendNewOutgoingDecision(requestSelfNowdecision);
    }

    void NodeDecisionManager::startFlow()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Starting flow for all flowable observers");
#endif
        for (auto flowableObserver : flowableObservers)
        {
            flowableObserver->Flow();
        }
    }

    void NodeDecisionManager::stopFlow()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Stopping flow for all flowable observers");
#endif
        for (auto flowableObserver : flowableObservers)
        {
            flowableObserver->StopFlow();
        }
    }

    // Gets called when any amount of points has updated, be it internal or external.
    // In this function, one would check whether this node would be next.
    void NodeDecisionManager::onPointsUpdated()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: OnPointsUpdated called");
        Serial.println("Checking whether already flowing...");
        Serial.println(currentNode);
#endif
        if (currentNode == NodeInfo::getCurrentNodeID())
            return; // already flowing.
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Checking whether not already scheduled to be next...");
        if (nextNode == NodeInfo::getCurrentNodeID())
            return; // already scheduled to be next.
        Serial.println("Checking whether already requesting...");
        if (currentOwnPendingDecision.topic == NEXTREQUEST)
            return; // already requesting.
        Serial.println("All conditions good, now deciding next node...");
#endif
        if (decideNextNode() == NodeInfo::getCurrentNodeID())
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("self is next, creating new next request...");
#endif
            requestSelfToBeNext();
        }
        else
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("self is not next, setting idle...");
#endif
            currentOwnPendingDecision = {EMPTYDECISION, NONE, NONE};
        }
    }

    // Method that will be called whenever the current pending activity is done.
    void NodeDecisionManager::PendingDecisionDoneActivity(Decision_t doneDecision)
    {

#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: pending decision is done. Choosing activity based on done decision...");
#endif

        switch (doneDecision.topic)
        {

        case NEWPOINTS:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("New points done, check whether self is next");
#endif
            // Check whether this node is next. If so, ask the other nodes

            // Is this ever even called? As in that the current decision is set to current points?
            // Also, would the assignment of this not give conflicts when another decision is taking place?
            // It would probably be a better idea that one would just check the nextNode on any given point
            // calculation and when so, create this next request.
            // Still, when this node is already active, this should then not happen.
            // So this logic would need to be moved to a function called onPointsUpdated probably.
            // with some internal logic.
        }
        break;

        case NEXTREQUEST:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("Next request done, check whether one can go now...");
#endif
            // Received approval to be next, so assign self to next
            nextNode = NodeInfo::getCurrentNodeID();

            if (currentNode == NONE)
            { // no node is next, so immediately ask a request to get permission now
                requestSelfToBeNow();
                currentOwnPendingDecision = {STARTFLOWREQUEST, NONE, NONE};
            }
            else
            {
                currentOwnPendingDecision = {EMPTYDECISION, NONE, NONE};
            }
        }
        break;

        case STARTFLOWREQUEST:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("Start flow request done. Now starting flow.");
#endif
            currentNode = nextNode;
            nextNode = NONE;
            // All nodes have acknowledged, and now this node may start their flow
            startFlow();
            currentOwnPendingDecision = {EMPTYDECISION, NONE, NONE};
        }
        break;

        case STOPPEDNOTIFY:
        {
            currentNode = NONE;
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("Stopped done. Emptying decision.");
#endif
            currentOwnPendingDecision = {EMPTYDECISION, NONE, NONE};
        }
        break;

        case EMPTYDECISION:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("ERR: decision acked but was empty");
#endif
        }

        default:
            break;
        };
    }

    void NodeDecisionManager::acknowledgeDecision(Decision_t decisionToAcknowledge)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Entering acknowledged decision");
#endif
        outgoingAckDecision.receiveDecisionToAcknowledge(decisionToAcknowledge);
    }

    void NodeDecisionManager::sendNewOutgoingDecision(Decision_t newOutgoingDecision)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: new outgoing decision");
#endif
        outgoingDecision.receiveOutgoingDecision(newOutgoingDecision);
    }

    // TODO rename this to "incoming external activity to denote the that this comes from an external sources"
    void NodeDecisionManager::incomingDecisionActivity(Decision_t newIncomingDecision)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Receiving incoming activity from other node");
#endif
        switch (newIncomingDecision.topic)
        {

        case NEWPOINTS:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("received new points. Updating...");
#endif
            // Update the points of the given node:
            receivedExternalPoints(newIncomingDecision.fromNodeID, newIncomingDecision.argument);
            // acknowledgeDecision(newIncomingDecision); // Currently do not do this because there is no
            // good acknowledgement mechanism in place for the off-activity of synchronization.
            onPointsUpdated();
        }
        break;

        case NEXTREQUEST:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("received next request. Checking...");
#endif
            // this is from another node, recalculate and check.
            nextNode = decideNextNode();
            if (nextNode == newIncomingDecision.fromNodeID)
            {
                acknowledgeDecision(newIncomingDecision);
            }
            else
            {
                // return NACK (this should not happen)
            }
        }
        break;

        case STOPPEDNOTIFY:
        {
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("Received other node stopped. Checking if self next...");
#endif
            currentNode = NONE;

// Check whether this node is next proposed to be next. If so, one could ask and make the decision pending.
#ifdef ARDUINO_ARCH_ESP32
            Serial.println("next node is");
            Serial.println(nextNode);
            Serial.println("current node points is");
            Serial.println(currentNodePoints[NodeInfo::getCurrentNodeID()]);
#endif
            outgoingAckDecision.receiveDecisionToAcknowledge(newIncomingDecision);

            // TODO verify whether this should be really here
            nextNode = decideNextNode();
            if (nextNode == NodeInfo::getCurrentNodeID() && currentNodePoints[NodeInfo::getCurrentNodeID()] != 0)
            {
#ifdef ARDUINO_ARCH_ESP32
                Serial.println("self is next. Making a new request");
#endif
                requestSelfToBeNext();
            }
        }
        break;

        default:
            break;
        };
    }

    void NodeDecisionManager::receivedExternalPoints(int nodeID, int points)
    {
        currentNodePoints[nodeID] = points;

// Verbose print statements:
#ifdef ARDUINO_ARCH_ESP32
        Serial.print("NDM: Received external points of ");
        Serial.println(points);
        Serial.print(" from nodeID: ");
        Serial.println(nodeID);
        Serial.println("All points are now:");
        for (int i = 0; i < NodeInfo::getNodeAmount(); i++)
        {
            Serial.print("Node ");
            Serial.print(i);
            Serial.print(": ");
            Serial.print(currentNodePoints[i]);
            Serial.println(" points.");
        }
#endif
    }

    void NodeDecisionManager::receivedInternalPoints(int internalPoints)
    {

        int thisNodeID = NodeInfo::getCurrentNodeID();

#ifdef ARDUINO_ARCH_ESP32
        Serial.print("NDM: received internal points of");
        Serial.println(internalPoints);
#endif
        currentNodePoints[thisNodeID] = internalPoints;
    }

    /* Public: */

    void NodeDecisionManager::flowHasStopped()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: received that flow has stopped.");
#endif
        Decision_t flowStoppedDecision = {STOPPEDNOTIFY, NodeInfo::getCurrentNodeID(), 0};
        currentOwnPendingDecision = flowStoppedDecision;
        sendNewOutgoingDecision(flowStoppedDecision); // notifying all other nodes
    }

    void NodeDecisionManager::confirmDecisionAcknowledgement()
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Current decision acknowledged by ack mgr");
#endif
        /// @todo Some verification check should be done here in the future, this auto approves the current pending decision.
        PendingDecisionDoneActivity(currentOwnPendingDecision);
    }

    // TODO rename this function to explicitly tell that this is an incoming decision from an external node
    void NodeDecisionManager::receiveIncomingDecision(Decision_t incomingDecision)
    {
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: New incoming decision received");
#endif
        incomingDecisionActivity(incomingDecision);
    }

    void NodeDecisionManager::addFlowableObserver(IFlowable *newFlowableObserver)
    {
        flowableObservers.push_back(newFlowableObserver);
    }

    void NodeDecisionManager::removeFlowableObserver(IFlowable *flowableObserverToRemove)
    {
        flowableObservers.erase(std::remove(flowableObservers.begin(), flowableObservers.end(),
                                            flowableObserverToRemove),
                                flowableObservers.end());
    }

    // TODO rename this function (also in virtuals) that this denotes that these points are gained internally
    void NodeDecisionManager::receiveUpdatedPoints(int points)
    {
        int thisNodeID = NodeInfo::getCurrentNodeID();
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("NDM: Updated internal points received.");
#endif
        // Update internal points and send synchronization message to other nodes:
        receivedInternalPoints(points);
        Decision_t internalPointsSynchronizeDecision = {NEWPOINTS, thisNodeID, points};
        sendNewOutgoingDecision(internalPointsSynchronizeDecision);

        // TODO replace this with the `flowHasStopped()` method that is up
        // This would be added in the interface.
#ifdef ARDUINO_ARCH_ESP32
        Serial.println("Checking whether the flow should stop");
#endif
        if (currentNodePoints[thisNodeID] <= 0 && currentNode == thisNodeID)
        {
            stopFlow();
        }

        // TODO Does immediately requesting a next request be incorrect for updates?
        // Probably not as this works queued, but do check!
        onPointsUpdated();
    }

} // namespace DecisionManagement