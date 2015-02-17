
#include "os3/base/UserConfig.h"

#include "os3/mobility/SatSGP4Mobility.h"
#include "os3/mobility/SatSGP4FisheyeMobility.h"

#include <cmath>
#include <cstdio>

Define_Module(UserConfig);

void UserConfig::initialize()
{
    userParameters.numOfSats = par("numOfSats");
    userParameters.frequency = par("frequency");
    userParameters.min_snr = par("min_snr");

    userParameters.mapX = std::atoi(getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 0));
    userParameters.mapY = std::atoi(getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 1));

    initializeSatMobility();
}

void UserConfig::handleMessage(cMessage* msg)
{
    error("Error in UserConfig::handleMessage(): This module is not able to handle messages");
}

void UserConfig::initializeSatMobility()
{
    // initialize satellite mobilities for view
    for (int i = 0; i < userParameters.numOfSats; i++) {
        char name[128];
        std::sprintf(name, "satellite[%d].mobility", i);
        SatSGP4Mobility* mob = check_and_cast< SatSGP4Mobility* >(simulation.getModuleByPath(name));

        if (mob != nullptr)
            satmoVector.push_back(mob);
        else {
            std::sprintf(name, "Error in UserConfig::initializeSatFisheyeMobility(): Could not find module \"%s\".", name);
            error(name);
        }
    }
}

