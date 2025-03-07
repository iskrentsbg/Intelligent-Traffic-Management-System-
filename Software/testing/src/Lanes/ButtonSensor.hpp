#include "Lane.hpp"
#pragma once
namespace Lanes
{
    class ButtonSensor
    {
    private:
        int delta = 0;
        int button_pin = 0;
        bool once;

        Lanes::Lane *lane;

        unsigned long currentTime = 0;

        const unsigned long debounceTime = 100;
        
        unsigned long prevBtnTime = 0;
    public:
        ButtonSensor(int pin,int GivenDelta, Lanes::Lane *newLane);
        int detectButtonPress(int prevDelta);
        void editLane(Lanes::Lane *NewLane);
    };
}
