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

#ifndef __CNI_OS3_OBSERVER_H_
#define __CNI_OS3_OBSERVER_H_

#include <omnetpp.h>
#include <SatSGP4FisheyeMobility.h>
#include <iostream>
#include <fstream>
#include <Calculation.h>
using namespace std;


/**
 * @class Observer
 * @brief Base Station with example function to check when a satellite is in view or to calculate the C/N0 for GPS satellites
 * @author Sarah Lehnhausen
 * @version 0.1
 */

class Observer : public cSimpleModule {
private:
    SatSGP4Mobility * Sat; // Reference to observed satellite
    SatSGP4FisheyeMobility * gpsSats[31]; // GPS satellites for C/N0 validation
    Calculation * calculation;
    cMessage *timer; // Self message to trigger observation
    double longitude; // Longitude of Observer
    double latitude; // Latitude of Observer
    double altitude; // Altitude of Observer
    double lastelv; // Last observed elevation is saved
    double interval; // Update interval for timer
    ofstream outfile; // File where results are written/saved
    time_t timestamp; // Time stamp for starting simulation
    int numgps; // Number of GPS satellites for C/N0 validation
    bool gps; // Bool to check whether gps or ISS validation should run

protected:

    /**
     * Initializes Observer module and calls setPosition()
     * @author Sarah Lehnhausen
     * @version 0.1
     */
    virtual void initialize();

    /**
     * checks every update interval if a certain satellite is in view and writes results to an output file
     * @param msg Omnetpp-message
     * @author Sarah Lehnhausen
     * @version 0.1
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * The written file is closed
     * @author Sarah Lehnhausen
     * @version 0.1
     */
    virtual void finish();

    /**
     * Sets the position on the map
     * @param latitude Latitude of Observer
     * @param longitude Longitude of Observer
     * @author Sarah Lehnhausen
     * @version 0.1
     */
    void setPosition(double latitude, double longitude);

    /**
     * Calculates SNR for given satellite
     * @param satindex index of satellite
     * @param bandwidth bandwidth of used channel
     * @author Sarah Lehnhausen
     * @version 0.1
     */
    double checksnr(int satindex, double bandwidth);
};

#endif
