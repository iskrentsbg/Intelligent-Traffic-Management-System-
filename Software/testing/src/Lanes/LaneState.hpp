#pragma once
namespace Lanes
{
    enum class LaneState {
        STOPPED_DETERMINE,
        STOPPED_EMPTY,
        STOPPED_CARS_WAITING,
        FLOW_DETERMINE,
        FLOW_EMPTY,
        FLOW_FLOWING
    };
}