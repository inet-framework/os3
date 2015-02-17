
#ifndef __OS3_Observer_H__
#define __OS3_Observer_H__

#include <omnetpp.h>

#include <iostream>
#include <fstream>
#include <ctime>

class SatSGP4Mobility;
class SatSGP4FisheyeMobility;
class Calculation;

//-----------------------------------------------------
// Class: Observer
// Base Station with example function to check when a satellite is in
// view or to calculate the C/N0 for GPS satellites
//-----------------------------------------------------
class Observer : public cSimpleModule
{
protected:

    // initializes Observer module and calls setPosition()
    virtual void initialize();

    // checks every update interval if a certain satellite is in view and writes results to an output file
    virtual void handleMessage(cMessage* msg);

    virtual void finish();

    // sets the position on the map
    void setPosition(double latitude, double longitude);

    // calculates SNR for given satellite
    // - satindex index of satellite
    // - bandwidth bandwidth of used channel
    double checksnr(int satindex, double bandwidth);

private:
    SatSGP4Mobility* Sat;                // Reference to observed satellite
    SatSGP4FisheyeMobility* gpsSats[31]; // GPS satellites for C/N0 validation
    Calculation* calculation;
    cMessage* timer;                     // Self message to trigger observation
    double longitude;                    // Longitude of Observer
    double latitude;                     // Latitude of Observer
    double altitude;                     // Altitude of Observer
    double lastelv;                      // Last observed elevation is saved
    double interval;                     // Update interval for timer
    std::ofstream outfile;               // File where results are written/saved
    std::time_t timestamp;               // Time stamp for starting simulation
    int numgps;                          // Number of GPS satellites for C/N0 validation
    bool gps;                            // bool to check whether gps or ISS validation should run
};

#endif
