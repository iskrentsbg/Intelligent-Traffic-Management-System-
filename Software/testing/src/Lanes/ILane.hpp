#include <iostream>
#include "LaneState.hpp"
#pragma once
namespace Lanes
{
    class ILane
    {
    public:
        virtual void toggleState(Lanes::LaneState State) = 0;
    };
}