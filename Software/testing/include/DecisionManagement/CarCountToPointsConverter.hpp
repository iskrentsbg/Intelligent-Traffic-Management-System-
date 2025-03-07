/*
 * CarCountToPointsConverter header file, containing the class that does the following:
 * 1. Take (a given overload function) of a car count as input
 * 2. Convert, using abstracted arithmetics, given data about the car counts to points
 * 3. Notify any observers with the new points
 *
 * Author: Wouter Swinkels
 * */

#pragma once

#include <vector>
using std::vector;

#include "DecisionManagement/Interface/ICarCountObserver.hpp"
#include "DecisionManagement/Interface/IIncomingPoints.hpp"

namespace DecisionManagement
{

    class CarCountToPointsConverter : ICarCountObserver
    {
    private:
        vector<IIncomingPoints *> pointObservers;

        void notifyPointObservers(int points);

    public:
        void newLocalCarCount(int totalCount) override;
        void addPointObserver(IIncomingPoints *newObserver);
        void removePointObserver(IIncomingPoints *observerToRemove);
        int calculatePointsFromCarCount(int totalCarCount);
    };

} // namespace DecisionManagement