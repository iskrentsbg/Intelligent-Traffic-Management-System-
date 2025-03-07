#pragma once

namespace DecisionManagement
{
    
    class IControlsFlow
    {
        public:
            virtual void flowHasStopped() = 0;
            //virtual void isCurrentlyFlowing() = 0;
    };

} // namespace DecisionManagement