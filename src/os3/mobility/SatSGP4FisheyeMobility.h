
#ifndef __OS3_SatSGP4FisheyeMobility_H__
#define __OS3_SatSGP4FisheyeMobility_H__

#include "os3/mobility/SatSGP4Mobility.h"

//-----------------------------------------------------
// Class: SatSGP4FisheyeMobility
//
// Realizes the SatSGP4Fisheye mobility module - provides methods to get and set
// the position of a satellite module and resets the satellite position when
// it gets outside the playground.
//-----------------------------------------------------
class SatSGP4FisheyeMobility : public SatSGP4Mobility
{
public:

    // sets a new reference point for the fisheye
    void setRefCenterPoint(const double& latitude, const double& longitude, const double& altitude);

    // returns the reference point's latitude
    double getRefCenterLatitude();

    // returns the reference point's longitude
    double getRefCenterLongitude();

    // returns the reference point's altitude
    double getRefCenterAltitude();

protected:

    // initialize Fisheye
    // - this method is same as SatSGP4Mobility::initialize() except that it satisfies
    // - mapX == mapY for a proper display. It also sets the center reference point.
    virtual void initialize(int stage);

    virtual void setTargetPosition();

private:
    // fisheye reference point
    double refCenterLatitude;
    double refCenterLongitude;
    double refCenterAltitude;

    double transmitPower;
};

#endif
