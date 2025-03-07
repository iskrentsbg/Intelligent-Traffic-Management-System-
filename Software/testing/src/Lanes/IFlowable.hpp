#include <iostream>
#pragma once
namespace Lanes
{
    class IFlowable
    {
    private:
    public:
        virtual void handler() = 0;
        virtual void StopFlow() = 0;
        virtual void Flow() = 0;
    };
}