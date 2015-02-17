
#include "os3/mobility/SatSGP4FisheyeMobility.h"

#include <cmath>

#include "os3/mobility/Norad.h"

Define_Module(SatSGP4FisheyeMobility);

void SatSGP4FisheyeMobility::initialize(int stage)
{
    // override from SatSGP4Mobility
    SatSGP4Mobility::initialize(stage);

    // Assert that we have a round fisheye circle
    mapY = mapX;

    // Additional parameters
    refCenterLongitude = par("refCenterLongitude");
    refCenterLatitude = par("refCenterLatitude");
    refCenterAltitude = par("refCenterAltitude");
    transmitPower = getParentModule()->par("transmitPower");
}

void SatSGP4FisheyeMobility::setTargetPosition()
{
    nextChange += updateInterval.dbl();
    noradModule->updateTime(nextChange);

    double radius = mapX / 2 - 1;
    double elevation = noradModule->getElevation(refCenterLatitude, refCenterLongitude, refCenterAltitude);
    double azimuth = noradModule->getAzimuth(refCenterLatitude, refCenterLongitude, refCenterAltitude);

    if (elevation > 0)
        radius -= std::abs((elevation / 90.0) * mapX / 2);

    lastPosition.x = -std::cos(deg2rad(azimuth + 90)) * radius + mapX / 2;
    lastPosition.y = -std::sin(deg2rad(azimuth + 90)) * radius + mapY / 2;

    if (lastPosition.x >= mapX)
        lastPosition.x = mapX - 1;
    if (lastPosition.y >= mapY)
        lastPosition.y = mapY - 1;

    targetPosition.x = lastPosition.x;
    targetPosition.y = lastPosition.y;
}

void SatSGP4FisheyeMobility::setRefCenterPoint(const double &latitude, const double &longitude,
                                               const double& altitude)
{
    refCenterLatitude = latitude;
    refCenterLongitude = longitude;
    refCenterAltitude = altitude;

    // Update satellite positions
    move();
}

double SatSGP4FisheyeMobility::getRefCenterLatitude()
{
    return refCenterLatitude;
}

double SatSGP4FisheyeMobility::getRefCenterLongitude()
{
    return refCenterLongitude;
}

double SatSGP4FisheyeMobility::getRefCenterAltitude()
{
    return refCenterAltitude;
}
