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

#ifndef __OS3_UserConfig_H__
#define __OS3_UserConfig_H__

#include <omnetpp.h>

class SatSGP4Mobility;

struct parameters
{
    int numOfSats;
    int mapX;
    int mapY;
    double min_snr;
    double frequency;
};

//-----------------------------------------------------
// Class: UserConfig
// This class manages the interaction with the user parameters and the
// omnet simulation. It also creates satellite mobility vectors.
//-----------------------------------------------------
class UserConfig : public cSimpleModule
{
public:
    /**
     * @brief returns the user settings
     * This method is just a dummy method. No action is taking place here.
     * @return object of type parameters which contains user-specific parameters set in omnetpp.ini
     */
    const parameters& getParameters() const                          { return userParameters; };

    /**
     * @brief Initializes the mobility for SatMobility
     * This method fills the mobility-module-vector (satmoGEO and satmoLEO Vectors) with satellite-mobilities (number of satellites set in omnetpp.ini).
     * @return a vector of type LineSegmentsMobilityBase (parent class of SatSGP4Mobilty and SatSGPFisheyeMobility). If an error occurs, the method returns an empty vector.
     */
    const std::vector< SatSGP4Mobility* >& getSatMobility() const    { return satmoVector;   };

protected:
    // initializes an object of type UserConfig and saves the user variables given in omnetpp.ini
    virtual void initialize();

    virtual void handleMessage(cMessage* msg);

    // fills the mobility-module-vector with satellite-mobilities (number of satellites set in omnetpp.ini).
    // These vectors are needed for the usage of SatSGP4Mobility and SatSGP4FisheyeMobility in other methods
    // (get the mobility of a specific satellite).
    void initializeSatMobility();

private:
    parameters userParameters;
    std::vector< SatSGP4Mobility* > satmoVector;
};

#endif
