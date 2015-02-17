
#include "os3/mobility/LUTMotionMobility.h"

#include <cmath>

Define_Module(LUTMotionMobility);

void LUTMotionMobility::initialize(int stage)
{
    StationaryMobility::initialize(stage);
    EV << "initializing LUTMotionMobility stage " << stage << endl;
    WATCH(lastPosition);
    if (stage == 0) {
        mapx = std::atoi(getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 0));
        mapy = std::atoi(getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 1));
        latitude = par("latitude");
        longitude = par("longitude");
    }
}

double LUTMotionMobility::getLUTPositionX()
{
    return longitude;
}
double LUTMotionMobility::getLUTPositionY()
{
    return latitude;
}

Coord LUTMotionMobility::getCurrentPosition()
{
    return Coord(longitude, latitude);
}

//
// this mobility model does not deal with speed, but it must be implemented
// because it is a pure virtual function declared in IMobility
//
Coord LUTMotionMobility::getCurrentSpeed()
{
    return Coord(0,0,0);
}

void LUTMotionMobility::setInitialPosition()
{
   lastPosition.y = ((-mapy * latitude) / 180) + (mapy / 2);
   lastPosition.x = mapx * longitude / 360 + (mapx / 2);
   lastPosition.x = static_cast<int>(lastPosition.x) % static_cast<int>(mapx);
}
