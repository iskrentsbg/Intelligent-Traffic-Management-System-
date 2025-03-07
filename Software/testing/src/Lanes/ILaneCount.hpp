#include <iostream>
#pragma once

namespace Lanes
{
    class ILaneCount
    {
    public:
        virtual int newCarCount(int count) = 0;
        virtual void newCarCountDelta(int delta) = 0;
    };
}