#pragma once

namespace DecisionManagement
{
    class ICarCountObserver
    {
        public:
            virtual void newLocalCarCount(int totalCount) = 0;
    };

} // namespace DecisionManagement