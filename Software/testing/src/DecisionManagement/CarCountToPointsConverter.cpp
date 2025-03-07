
#include <algorithm>

#include "DecisionManagement/CarCountToPointsConverter.hpp"

namespace DecisionManagement
{

    /* Private: */

    void CarCountToPointsConverter::notifyPointObservers(int points)
    {
        for (auto newPointObserver : pointObservers)
        {
            newPointObserver->receiveUpdatedPoints(points);
        }
    }
    int CarCountToPointsConverter::calculatePointsFromCarCount(int totalCarCount)
    {

        if (totalCarCount < 0)
        {
            return 0; // or any other logic for negative counts
        }
        return totalCarCount; // Simplest form, the points exactly equal the car count.

        /// @todo In the future, one uses the arithmetic helper class for calculations.
    }
    /* Public: */

    void CarCountToPointsConverter::newLocalCarCount(int totalCount)
    {
        if (totalCount < 0)
        {
            // Handle negative input, for example, set points to 0
            notifyPointObservers(0);
        }
        else
        {
            int calculatedPoints = calculatePointsFromCarCount(totalCount);
            notifyPointObservers(calculatedPoints);
        }
    }

    /// @todo abstract away the observer logic (see # snippet in GitLab)

    void CarCountToPointsConverter::addPointObserver(IIncomingPoints *newObserver)
    {
        pointObservers.push_back(newObserver);
    }

    void CarCountToPointsConverter::removePointObserver(IIncomingPoints *observerToRemove)
    {
        pointObservers.erase(std::remove(pointObservers.begin(), pointObservers.end(),
                                         observerToRemove),
                             pointObservers.end());
    }

} // namespace DecisionManagement