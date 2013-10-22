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

#ifndef __CNI_OS3_SATSGP4FISHEYEMOBILITY_H_
#define __CNI_OS3_SATSGP4FISHEYEMOBILITY_H_

#include <omnetpp.h>
#include "SatSGP4Mobility.h"
#include <cmath>
#include "Norad.h"

/**
 * @class SatSGP4FisheyeMobility
 * @brief realizes the SatSGP4Fisheye mobility module.
 * This class realizes the SatSGP4Fisheye mobility module. It provides methods to get and set the position of a satellite module and resets the satellite position when it will get out of the playground.
 * @author Marian P. Felder, Florian Schweikowksi, Daniel Merget
 * @version 0.1
 * Defined class
 */
class   SatSGP4FisheyeMobility : public SatSGP4Mobility {
private:
    double refCenterLatitude, refCenterLongitude, refCenterAltitude; // Reference point for fisheye
    double transmitPower;

protected:
    /**
     * @brief Initialization of Fisheye
     * This method is equal to SatSGP4Mobility::initialize() except that it satisfies mapX == mapY for a proper display. It also sets the center reference point.
     */
    virtual void initialize(int stage);

    /**
     * @brief Implements movement of the satellite on the map. Overrides SatSGP4Mobility::move().
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    //void move();

    virtual void setTargetPosition();

public:
    /**
     * @brief Sets a new reference point for the fisheye
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    void setRefCenterPoint(const double &latitude, const double &longitude, const double& altitude);

    /**
     * @brief Returns the reference point's latitude
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    double getRefCenterLatitude();

    /**
     * @brief Returns the reference point's longitude
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    double getRefCenterLongitude();

    /**
     * @brief Returns the reference point's altitude
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    double getRefCenterAltitude();
};

#endif
