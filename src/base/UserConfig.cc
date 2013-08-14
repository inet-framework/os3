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

#include "UserConfig.h"

Define_Module(UserConfig);

void UserConfig::initialize() {
    userParameters.numOfSats = par("numOfSats");
    userParameters.frequency = par("frequency");
    userParameters.min_snr = par("min_snr");

    userParameters.mapX = atoi(this->getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 0));
    userParameters.mapY = atoi(this->getParentModule()->getParentModule()->getDisplayString().getTagArg("bgb", 1));

    initializeSatMobility();
}

void UserConfig::handleMessage(cMessage *msg) {
    error("Error in UserConfig::handleMessage(): This module is not able to handle messages");
}

void UserConfig::initializeSatMobility()
{
    // Initialize satellite mobilities for view
    for (int i = 0; i < userParameters.numOfSats; i++) {
        char name[128];
        sprintf(name, "satellite[%d].mobility", i);
        SatSGP4Mobility* mob = check_and_cast< SatSGP4Mobility* >(simulation.getModuleByPath(name));

        if (mob != NULL)
            satmoVector.push_back(mob);
        else {
            sprintf(name, "Error in UserConfig::initializeSatFisheyeMobility(): Could not find module \"%s\".", name);
            error(name);
        }
    }
}

