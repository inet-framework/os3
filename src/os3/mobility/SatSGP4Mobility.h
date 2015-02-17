
#ifndef __OS3_SatSGP4Mobility_H__
#define __OS3_SatSGP4Mobility_H__

#include "mobility/common/LineSegmentsMobilityBase.h"    // inet

class Norad;

//-----------------------------------------------------
// Class: SatSGP4Mobility
//
// Realizes the SatSGP4 mobility module - provides methods to get and set
// the position of a satellite module and resets the satellite position when
// it gets outside the playground.
//-----------------------------------------------------
class SatSGP4Mobility : public LineSegmentsMobilityBase
{
public:
    SatSGP4Mobility();

    // returns x-position of satellite on playground (not longitude!)
    virtual double getPositionX() const                  { return lastPosition.x; };

    // returns y-position of satellite on playground (not latitude!)
    virtual double getPositionY() const                  { return lastPosition.y; };

    // returns the altitude of the satellite.
    virtual double getAltitude() const;

    // returns the elevation for the satellite in degrees
    virtual double getElevation(const double& refLatitude, const double& refLongitude, const double& refAltitude = -9999) const;

    // returns the azimuth from satellite to reference point in degrees
    virtual double getAzimuth(const double& refLatitude, const double& refLongitude, const double& refAltitude = -9999) const;

    // returns the Euclidean distance from satellite to reference point
    virtual double getDistance(const double& refLatitude, const double& refLongitude, const double& refAltitude = -9999) const;

    // returns satellite latitude
    virtual double getLatitude() const;

    // returns satellite longitude
    virtual double getLongitude() const;

protected:
    Norad* noradModule;
    int mapX, mapY;
    double transmitPower;

    // initialize module
    // - creates a reference to the Norad moudule
    // - timestamps and initial position on playground are managed here.
    virtual void initialize(int stage);

    // sets the position of satellite
    // - sets the target position for the satellite
    // - the position is fetched from the Norad module with reference to the current timestamp
    virtual void setTargetPosition();

    // resets the position of the satellite
    // - wraps around the position of the satellite if it reaches the end of the playground
    virtual void fixIfHostGetsOutside();

    // implements basic satellite movement on map
    virtual void move();
};

#endif
