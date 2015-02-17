
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
