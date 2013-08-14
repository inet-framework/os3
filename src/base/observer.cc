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

#include "observer.h"

Define_Module(Observer);

void Observer::initialize() {

    longitude = par("ObserverLongitude");
    latitude = par("ObserverLatitude");
    altitude = par("ObserverAltitude");
    interval = par("TimerInterval");

    Sat = dynamic_cast<SatSGP4Mobility*>(this->getParentModule()->getSubmodule(
            "satellite", 0)->getSubmodule("mobility", 0));
    if (Sat == NULL) {
        error(
                "Error in Observer::initialize(): Could not find Satellite module.");
    } else {
        std::cout << "Satellite module found!" << std::endl;
    }

    calculation =
            dynamic_cast<Calculation*>(this->getParentModule()->getSubmodule(
                    "cni_os3", 0)->getSubmodule("calculation", 0));
    if (calculation == NULL) {
        error(
                "Error in Observer::initialize(): Could not find Calculation module.");
    } else {
        std::cout << "Calculation module found!" << std::endl;
    }

    gps = par("gps");
    numgps = par("numgps");

    if (gps == true) {

        for (int i = 0; i < numgps; i++) {
            gpsSats[i] =
                    dynamic_cast<SatSGP4FisheyeMobility*>(this->getParentModule()->getSubmodule(
                            "satellite", i)->getSubmodule("mobility", 0));
            if (Sat == NULL) {
                error(
                        "Error in Observer::initialize(): Could not find Satellite module.");
            } else {
                std::cout << "Satellite module found!" << std::endl;
            }
        }

        timer = new cMessage("timer"); // Start Timer to toggle C/N0 calculation
        scheduleAt(simTime() + interval, timer);

        // Generate outfile
        char text[100];
        char * cstr = new char[100];
        sprintf(text, "CN0forGPSsatellites_%f,%f.txt", latitude, longitude);
        strcpy(cstr, text);

        // Open outfile
        outfile.open(cstr, ofstream::trunc);
        outfile << "Satellite\tC/N0\tElevation\tAzimuth" << endl;

    } else {
        timer = new cMessage("timer");

        scheduleAt(simTime() + interval, timer);

        // Generate outfile
        char text[100];
        char * cstr = new char[51];
        sprintf(text, "SatelliteMovementAt%f,%f.txt", latitude, longitude);
        strcpy(cstr, text);

        // Open outfile
        outfile.open(cstr, ofstream::trunc);
        outfile << "Date\tTime\tElevation\tAzimuth" << endl;

        // Initialize time and elevation
        timestamp = time(0);
        lastelv = 0;

        // Set Position on map
        setPosition(latitude, longitude);
    }

}

void Observer::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        if (gps == true) {
            double bandwidth = 2000000; // GPS bandwidth
            for (int i = 0; i < numgps; i++) {
                double tempsnr = checksnr(i, bandwidth);
                double tempcn0 = tempsnr + 10 * log10(bandwidth);

                outfile
                        << i
                        << "\t"
                        << tempcn0
                        << "\t"
                        << gpsSats[i]->getElevation(latitude, longitude,
                                altitude) << "\t"
                        << gpsSats[i]->getAzimuth(latitude, longitude, altitude)
                        << "\t" << endl;
            }

        } else {

            double elevation = Sat->getElevation(latitude, longitude, altitude);

            if ((elevation > 10) | (lastelv > 10)) { // Satellite in view or just out of view

                double azimuth = Sat->getAzimuth(latitude, longitude, altitude);

                tm *currentTime;
                time_t runtime = timestamp + simTime().dbl();
                currentTime = localtime(&runtime);

                if ((elevation > 10) & (lastelv < 10)) // First time satellite is in view
                    outfile << currentTime->tm_mday << "."
                            << currentTime->tm_mon + 1 << "."
                            << currentTime->tm_year + 1900 << "\t"
                            << currentTime->tm_hour << ':'
                            << currentTime->tm_min << ":" << currentTime->tm_sec
                            << "\t" << elevation << "\t" << azimuth << endl;

                if ((elevation < 10) & (lastelv > 10)) { // Last time satellite is in view
                    runtime = runtime - interval; // Get last time stamp back
                    currentTime = localtime(&runtime);
                    outfile << currentTime->tm_mday << "."
                            << currentTime->tm_mon + 1 << "."
                            << currentTime->tm_year + 1900 << "\t"
                            << currentTime->tm_hour << ':'
                            << currentTime->tm_min << ":" << currentTime->tm_sec
                            << "\t" << lastelv << "\t" << azimuth << endl;
                }

                lastelv = elevation;

            }
            scheduleAt(simTime() + interval, timer);

        }
    } else
        error("Observer should not receive Message other than self message");

}

void Observer::setPosition(double latitude, double longitude) {

    double mapx = atoi(
            this->getParentModule()->getDisplayString().getTagArg("bgb", 0));
    double mapy = atoi(
            this->getParentModule()->getDisplayString().getTagArg("bgb", 1));

    double posY = ((-mapy * latitude) / 180) + (mapy / 2);
    double posX = mapx * longitude / 360 + (mapx / 2);
    posX = ((int) posX % (int) mapx);

    this->getDisplayString().setTagArg("p", 0, (long) posX);
    this->getDisplayString().setTagArg("p", 1, (long) posY);
}

double Observer::checksnr(int satindex, double bandwidth) {

    double transmitterGain = 13.0; // GPS transmitter gain
    double receiverGain = 0.0; // Average GPS antenna
    double transmitterPower = 14; // GPS transmitter power
    double lambda = 0.1905; // wavelength of GPS signal in m (f = 1575,42 MHz)
    double dG = 0.5; // Average ratio of antenna radiation from ground (omnidirectional gps antenna)
    double tR = 150; // in K, Receiver noise temperature

    double snr = calculation->calcSNR(transmitterGain, receiverGain,
            transmitterPower, lambda, satindex, bandwidth, latitude, longitude, -9999, dG, tR);
    return snr;
}

void Observer::finish() {
    outfile.close();
    cancelAndDelete(timer);
}
