#pragma once

namespace DecisionManagement{

class IFlowable {
    public:
        virtual void handler() = 0;
        virtual void Flow() = 0;
        virtual void StopFlow() = 0;
};

} // namespace DecisionManagement