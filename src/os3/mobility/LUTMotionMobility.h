//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __OS3_LUTMotionMobility_H__
#define __OS3_LUTMotionMobility_H__

#include <omnetpp.h>

#include "mobility/common/MovingMobilityBase.h"    // inet

/**
 * @class LUTMotionMobility
 * @brief Uses the BonnMotion native file format. See NED file for more info.
 *
 * LUT = Local User Terminal
 *
 * @ingroup mobility
 * @author Andras Varga
 */
class LUTMotionMobility : public MovingMobilityBase
{
public:

    /** @brief Returns longitude.*/
    double getLUTPositionX();

    /** @brief Returns latitude.*/
    double getLUTPositionY();

    virtual Coord getCurrentPosition();
    virtual Coord getCurrentSpeed();

protected:
    /** @brief Initializes mobility model parameters.
     * Positions Mission Control Center on map
     */
    virtual void initialize(int);

    /**
     * @brief dummy
     * This method is just a dummy method. No action is taking place here.
     * @param msg omnetpp-message
     */
    //virtual void handleSelfMessage(cMessage *msg);

    virtual void move();

    virtual void initializePosition();

    double latitude, longitude; // Geographic coordinates
    double mapx, mapy; // Coordinates on map
};

#endif
