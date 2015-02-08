//-----------------------------------------------------
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
//-----------------------------------------------------

#include "SatSGP4Mobility.h"

#include <ctime>
#include "Norad.h"

Define_Module(SatSGP4Mobility);

void SatSGP4Mobility::initialize(int stage)
{
    // noradModule must be initialized before LineSegmentsMobilityBase calling setTargetPosition() in its initialization at stage 1
    if (stage == 1) {
        noradModule->initializeMobility(nextChange);
    }
    LineSegmentsMobilityBase::initialize(stage);

    noradModule = check_and_cast< Norad* >(this->getParentModule()->getSubmodule("NoradModule"));
    if (noradModule == NULL) {
        error("Error in SatSGP4Mobility::initializeMobility(): Cannot find module Norad.");
    }

    tm* currentTime;
    time_t timestamp = std::time(0);
    currentTime = std::gmtime(&timestamp);

    // Convert to julian time
    noradModule->setJulian(currentTime);

    mapX = atoi(this->getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 0));
    mapY = atoi(this->getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 1));

    transmitPower = this->getParentModule()->par("transmitPower");

    ev << "initializing SatSGP4Mobility stage " << stage << endl;
    WATCH(lastPosition);

    updateVisualRepresentation();
}

double SatSGP4Mobility::getAltitude() const
{
    return noradModule->getAltitude();
}

double SatSGP4Mobility::getElevation(const double &refLatitude, const double &refLongitude,
                                     const double &refAltitude) const
{
    return noradModule->getElevation(refLatitude, refLongitude, refAltitude);
}

double SatSGP4Mobility::getAzimuth(const double &refLatitude, const double &refLongitude,
                                   const double &refAltitude) const
{
    return noradModule->getAzimuth(refLatitude, refLongitude, refAltitude);
}

double SatSGP4Mobility::getDistance(const double &refLatitude, const double &refLongitude,
                                    const double &refAltitude) const
{
    return noradModule->getDistance(refLatitude, refLongitude, refAltitude);
}

double SatSGP4Mobility::getLongitude() const
{
    return noradModule->getLongitude();
}

double SatSGP4Mobility::getLatitude() const
{
    return noradModule->getLatitude();
}

void SatSGP4Mobility::setTargetPosition()
{
    nextChange += updateInterval.dbl();
    noradModule->updateTime(nextChange);

    lastPosition.x = mapX * noradModule->getLongitude() / 360 + (mapX / 2);
    lastPosition.x = static_cast<int>(lastPosition.x) % static_cast<int>(mapX);
    lastPosition.y = ((-mapY * noradModule->getLatitude()) / 180) + (mapY / 2); // from the "original" move function.

    targetPosition.x = lastPosition.x;
    targetPosition.y = lastPosition.y;
}

void SatSGP4Mobility::move()
{
    LineSegmentsMobilityBase::move();
    raiseErrorIfOutside();
}

void SatSGP4Mobility::fixIfHostGetsOutside()
{
    raiseErrorIfOutside();
}
