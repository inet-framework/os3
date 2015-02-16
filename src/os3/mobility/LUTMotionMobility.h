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

#include "mobility/static/StationaryMobility.h"    // inet

//-----------------------------------------------------
// Class: LUTMotionMobility
//
// Positions a ground station at a specific lat/long
//-----------------------------------------------------
class LUTMotionMobility : public StationaryMobility
{
public:
    // returns longitude
    double getLUTPositionX();

    // returns latitude
    double getLUTPositionY();

    virtual Coord getCurrentPosition();
    virtual Coord getCurrentSpeed();

protected:
    virtual void initialize(int);

    virtual void setInitialPosition();

    double latitude, longitude;   // Geographic coordinates
    double mapx, mapy;            // Coordinates on map
};

#endif
