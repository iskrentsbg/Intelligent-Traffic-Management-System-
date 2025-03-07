#pragma once

namespace DecisionManagement
{
    
    class IDecisionAcknowledgementObserver {
    public:
        virtual void confirmDecisionAcknowledgement() = 0;
    };

} // namespace DecisionManagement