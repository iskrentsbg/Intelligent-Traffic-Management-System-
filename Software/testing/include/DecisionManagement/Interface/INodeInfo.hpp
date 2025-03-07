#pragma once

namespace DecisionManagement
{
    
    class INodeInfo {

        public:
            virtual void getNodeAmount() = 0;
            virtual void getNodeMAC(int nodeID) = 0;
            virtual void getCurrentNodeID() = 0;
    };

} // namespace DecisionManagement