/*
 * NodeDecisionManager class, main manager class for decisions related to the intersection node role system.
 * Does the following:
 * - Keeps track of node points, both from local info and received messages
 * - Will handle the routine system with the other connected nodes of what nodes gets the control
 *   based on the amount of points.
 * @todo Can this class be split up? Maybe separating decisions and point conservation
 * Author: Wouter Swinkels
 */
#pragma once

#include <array>
#include <vector>
using std::vector;

#include "DecisionManagement/Interface/IIncomingPoints.hpp"
#include "DecisionManagement/Interface/IIncomingDecision.hpp"
#include "DecisionManagement/Interface/IControlsFlow.hpp"
#include "DecisionManagement/Interface/IDecisionAcknowledgementObserver.hpp"
#include "DecisionManagement/Interface/IFlowable.hpp"

#include "DecisionManagement/Interface/IOutgoingDecision.hpp" // to delegate outgoing decision to
#include "DecisionManagement/OutgoingDecisionHandler.hpp"
#include "DecisionManagement/DecisionAckReturner.hpp"

/// @todo This node amount should be kept track of in separate API-ish namespace.
/// For now, an array is used; this could use dynamism (as in list, etc.) in the future.
#define NODE_AMOUNT 4
#define NONE -1

namespace DecisionManagement {
    
    class NodeDecisionManager : public IControlsFlow, public IIncomingDecision, public IIncomingPoints, public IDecisionAcknowledgementObserver {
        private:
            vector<IFlowable*> flowableObservers;
            IOutgoingDecision& outgoingDecision;
            IReturnDecisionAcknowledgement& outgoingAckDecision;            

            int currentNodePoints[NODE_AMOUNT] = {0};  // For MVP, this is just the car count per node -> will be dynamic in the future.

            int currentNode = NONE;               // The current node that has control of the system. -1 means no node has control. 
            int nextNode = NONE;                  // Calculated based on the synchronized points. -1 means no node is next.
            Decision_t currentOwnPendingDecision; // Own made decision that is pending acknowledgement from other nodes

            int decideNextNode();
            void requestSelfToBeNext();
            void requestSelfToBeNow();
            void PendingDecisionDoneActivity(Decision_t doneDecision);
            void startFlow();
            void stopFlow();
            void onPointsUpdated();
            void sendNewOutgoingDecision(Decision_t newOutgoingDecision);
            void incomingDecisionActivity(Decision_t newIncomingDecision);
            void acknowledgeDecision(Decision_t decisionToAcknowledge);
            void receivedExternalPoints(int nodeID, int points);
            void receivedInternalPoints(int internalPoints);

        public:
            NodeDecisionManager(IOutgoingDecision& IOD, IReturnDecisionAcknowledgement& IRDA) 
                                : outgoingDecision(IOD), outgoingAckDecision(IRDA) {};

            void flowHasStopped() override;
            void receiveUpdatedPoints(int points) override;
            void confirmDecisionAcknowledgement() override;
            void receiveIncomingDecision(Decision_t IIncomingDecision) override;

            void addFlowableObserver(IFlowable* newFlowableObserver);
            void removeFlowableObserver(IFlowable* FlowableObserverToRemove);
    };

} // namespace DecisionManagement