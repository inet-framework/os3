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

#include "os3/base/Observer.h"

Define_Module(Observer);

#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "os3/base/Calculation.h"

#include "os3/mobility/SatSGP4Mobility.h"
#include "os3/mobility/SatSGP4FisheyeMobility.h"

Observer::Observer()
{
   Sat         = nullptr;
   calculation = nullptr;
   timer       = nullptr;
   longitude   = 0.0;
   latitude    = 0.0;
   altitude    = 0.0;
   lastelv     = 0.0;
   interval    = 0.0;
   numgps      = 0;
   gps         = false;
}

void Observer::initialize()
{
    longitude = par("ObserverLongitude");
    latitude = par("ObserverLatitude");
    altitude = par("ObserverAltitude");
    interval = par("TimerInterval");

    Sat = dynamic_cast<SatSGP4Mobility*>(getParentModule()->getSubmodule("satellite", 0)->getSubmodule("mobility", 0));
    if (Sat == nullptr) {
        error("Error in Observer::initialize(): Could not find Satellite module.");
    } else {
        std::cout << "Satellite module found!" << std::endl;
    }

    calculation = dynamic_cast<Calculation*>(getParentModule()->getSubmodule("cni_os3", 0)->getSubmodule("calculation", 0));
    if (calculation == nullptr) {
        error("Error in Observer::initialize(): Could not find Calculation module.");
    } else {
        std::cout << "Calculation module found!" << std::endl;
    }

    gps = par("gps");
    numgps = par("numgps");

    if (gps == true) {

        for (int i = 0; i < numgps; i++) {
            gpsSats[i] = dynamic_cast<SatSGP4FisheyeMobility*>(getParentModule()->getSubmodule("satellite", i)->getSubmodule("mobility", 0));
            if (Sat == nullptr) {
                error("Error in Observer::initialize(): Could not find Satellite module.");
            } else {
                std::cout << "Satellite module found!" << std::endl;
            }
        }

        // start timer to toggle C/N0 calculation
        timer = new cMessage("timer");
        scheduleAt(simTime() + interval, timer);

        // generate outfile
        char text[100];
        char* cstr = new char[100];
        std::sprintf(text, "CN0forGPSsatellites_%f,%f.txt", latitude, longitude);
        std::strcpy(cstr, text);

        // open outfile
        outfile.open(cstr, std::ofstream::trunc);
        outfile << "Satellite\tC/N0\tElevation\tAzimuth" << std::endl;

    } else {

        timer = new cMessage("timer");
        scheduleAt(simTime() + interval, timer);

        // generate outfile
        char text[100];
        char* cstr = new char[51];
        std::sprintf(text, "SatelliteMovementAt%f,%f.txt", latitude, longitude);
        std::strcpy(cstr, text);

        // open outfile
        outfile.open(cstr, std::ofstream::trunc);
        outfile << "Date\tTime\tElevation\tAzimuth" << std::endl;

        // initialize time and elevation
        timestamp = std::time(0);
        lastelv = 0;

        // set Position on map
        setPosition(latitude, longitude);
    }
}

void Observer::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        if (gps == true) {
            const double bandwidth = 2000000; // GPS bandwidth
            for (int i = 0; i < numgps; i++) {
                const double tempsnr = checksnr(i, bandwidth);
                const double tempcn0 = tempsnr + 10 * std::log10(bandwidth);

                outfile
                        << i
                        << "\t"
                        << tempcn0
                        << "\t"
                        << gpsSats[i]->getElevation(latitude, longitude,
                                altitude) << "\t"
                        << gpsSats[i]->getAzimuth(latitude, longitude, altitude)
                        << "\t" << std::endl;
            }

        } else {

            const double elevation = Sat->getElevation(latitude, longitude, altitude);

            if ((elevation > 10) | (lastelv > 10)) { // Satellite in view or just out of view

                const double azimuth = Sat->getAzimuth(latitude, longitude, altitude);

                std::time_t runtime = timestamp + simTime().dbl();
                std::tm* currentTime = std::localtime(&runtime);

                if ((elevation > 10) & (lastelv < 10)) // First time satellite is in view
                    outfile << currentTime->tm_mday << "."
                            << currentTime->tm_mon + 1 << "."
                            << currentTime->tm_year + 1900 << "\t"
                            << currentTime->tm_hour << ':'
                            << currentTime->tm_min << ":" << currentTime->tm_sec
                            << "\t" << elevation << "\t" << azimuth << std::endl;

                if ((elevation < 10) & (lastelv > 10)) { // Last time satellite is in view
                    runtime = runtime - interval; // Get last time stamp back
                    currentTime = std::localtime(&runtime);
                    outfile << currentTime->tm_mday << "."
                            << currentTime->tm_mon + 1 << "."
                            << currentTime->tm_year + 1900 << "\t"
                            << currentTime->tm_hour << ':'
                            << currentTime->tm_min << ":" << currentTime->tm_sec
                            << "\t" << lastelv << "\t" << azimuth << std::endl;
                }

                lastelv = elevation;

            }
            scheduleAt(simTime() + interval, timer);
        }
    } else {
        error("Observer should not receive Message other than self message");
    }
}

void Observer::setPosition(double latitude, double longitude)
{
    const double mapx = std::atoi(getParentModule()->getDisplayString().getTagArg("bgb", 0));
    const double mapy = std::atoi(getParentModule()->getDisplayString().getTagArg("bgb", 1));

    double posY = ((-mapy * latitude) / 180) + (mapy / 2);
    double posX = mapx * longitude / 360 + (mapx / 2);
    posX = static_cast<int>(posX) % static_cast<int>(mapx);

    getDisplayString().setTagArg("p", 0, static_cast<long>(posX));
    getDisplayString().setTagArg("p", 1, static_cast<long>(posY));
}

double Observer::checksnr(int satindex, double bandwidth)
{
    const double transmitterGain = 13.0; // GPS transmitter gain
    const double receiverGain = 0.0;     // Average GPS antenna
    const double transmitterPower = 14;  // GPS transmitter power
    const double lambda = 0.1905;        // wavelength of GPS signal in m (f = 1575,42 MHz)
    const double dG = 0.5;               // Average ratio of antenna radiation from ground (omnidirectional gps antenna)
    const double tR = 150;               // in K, Receiver noise temperature

    const double snr = calculation->calcSNR(transmitterGain, receiverGain,
            transmitterPower, lambda, satindex, bandwidth, latitude, longitude, -9999, dG, tR);
    return snr;
}

void Observer::finish()
{
    outfile.close();
    cancelAndDelete(timer);
}
