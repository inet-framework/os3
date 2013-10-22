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

#ifndef __CNI_OS3_SATSGP4MOBILITY_H_
#define __CNI_OS3_SATSGP4MOBILITY_H_

#include <omnetpp.h>
#include <cstdio>
#include <cmath>
#include <LineSegmentsMobilityBase.h>
#include "Norad.h"

/**
 * @class SatSGP4Mobility
 * @brief Realizes the SatSGP4 mobility module.
 * This class realizes the SatSGP4 mobility module. It provides methods to get and set the position of a satellite module and resets the satellite position when it gets outside the playground.
 * @author Andras Varga, Marian P. Felder, Florian Schweikowksi, Daniel Merget
 * @version 0.1
 * Defined class
 * @version 0.2
 * This is now the base class for SatSGP4FisheyeMobility.
 */
class   SatSGP4Mobility : public LineSegmentsMobilityBase
{
protected:

    Norad *noradModule;
    int mapX, mapY;
    double transmitPower;

    /**
     * @brief initialization of module
     * This method initializes the mobility module and creates a reference to the Norad moudule. Also, timestamps and initial position on playground are managed here.
     * @param stage Stage of initialization (1: initial positioning)
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Now uses the newly introduced move() method
     */
     virtual void initialize(int stage);

    /**
     * @brief Sets the position of satellite. Overridden from LineSegmentsMobilityBase.
     * This method sets the target position for the satellite. The position is fetched from the Norad module with reference to the current timestamp.
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Now uses the newly introduced move() method
     */
    virtual void setTargetPosition();

    /**
     * @brief Resets the position of the satellite. Overridden from LineSegmentsMobilityBase.
     * This method wraps around the position of the satellite if it reaches the end of the playground.
     * @author Marian P. Felder, Florian Schweikowski
     * @version 0.1
     * Method implemented
     */
    virtual void fixIfHostGetsOutside();

    /**
     * @brief Implements movement of the satellite on the map
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    virtual void move();

public:
    /**
     * @brief Get x position of satellite
     * This method returns the x position of the satellite.
     * @return x-position of satellite on playground (not longitude!)
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual, inline and const
     */
    inline virtual double getPositionX() const {return lastPosition.x;};

    /**
     * @brief Get y position of satellite
     * This method returns the y position of the satellite.
     * @return y-position of satellite on playground (not latitude!)
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual, inline and const
     */
    inline virtual double getPositionY() const {return lastPosition.y;};

    /**
     * @brief Get altitude of satellite
     * This method returns the altitude of the satellite.
     * @return Altitude of the satellite
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual and const
     */
    virtual double getAltitude() const;

    /**
     * @brief Get elevation for satellite
     * This method returns the elevation for the satellite.
     * @return Elevation for the satellite in degrees
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual and const
     */
    virtual double getElevation(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999) const;

    /**
     * @brief Calculate the azimuth from satellite to reference point
     * @return Azimuth in degrees
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    virtual double getAzimuth(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999) const;

    /**
     * @brief Calculate the distance from satellite to reference point
     * @return Euclidean distance
     * @author Daniel Merget
     * @version 0.1
     * Method implemented
     */
    virtual double getDistance(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999) const;

    /**
     * @brief Get elevation for satellite
     * This method returns the elevation for the satellite.
     * @return Elevation for the satellite
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual and const
     */
    virtual double getLatitude() const;

    /**
     * @brief Get elevation for satellite
     * This method returns the elevation for the satellite.
     * @return Elevation for the satellite
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now virtual and const
     */
    virtual double getLongitude() const;
};

#endif
