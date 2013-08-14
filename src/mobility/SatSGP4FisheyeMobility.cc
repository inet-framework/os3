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

#include "SatSGP4FisheyeMobility.h"

Define_Module(SatSGP4FisheyeMobility);

void SatSGP4FisheyeMobility::initialize(int stage)
{
    // Override from SatSGP4Mobility
    SatSGP4Mobility::initialize(stage);

    // Assert that we have a round fisheye circle
    mapY = mapX;

    // Additional parameters
    refCenterLongitude = par("refCenterLongitude");
    refCenterLatitude = par("refCenterLatitude");
    refCenterAltitude = par("refCenterAltitude");
    transmitPower = this->getParentModule()->par("transmitPower");
}

void SatSGP4FisheyeMobility::setTargetPosition()
{
    nextChange += updateInterval.dbl();
    noradModule->updateTime(nextChange);

    double radius = mapX / 2 - 1;
    double elevation = noradModule->getElevation(refCenterLatitude, refCenterLongitude, refCenterAltitude);
    double azimuth = noradModule->getAzimuth(refCenterLatitude, refCenterLongitude, refCenterAltitude);

    if (elevation > 0)
        radius -= abs((elevation / 90.0) * mapX / 2);

    lastPosition.x = -cos(deg2rad(azimuth + 90)) * radius + mapX / 2;
    lastPosition.y = -sin(deg2rad(azimuth + 90)) * radius + mapY / 2;

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
