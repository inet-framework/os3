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

#ifndef __CNI_OS3_NORAD_H_
#define __CNI_OS3_NORAD_H_

#include <omnetpp.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <libnorad.h>
#include <WebServiceControl.h>

/**
 * @class Norad
 * @brief provides the functionality for satellite positioning
 * this class provides the functionality needed to get the positions for satellites according
 * to current tables from web information by providing known data
 * @author Marian P. Felder, Florian Schweikowski
 * @version 0.1
 * first draft
 */
class Norad : public cSimpleModule {
public:
    /**
     * @brief Sets the internal calendar by translating the current gregorian time
     * @author Marian P. Felder, Florian Schweikowski
     * @param currentTime Time at which the simulation takes place
     * @version 0.1
     * Method created
     */
    void setJulian(tm* currentTime);

    /**
     * @brief Updates the end time of current linear movement for calculation of current position.
     * @author Marian P. Felder, Florian Schweikowski
     * @param targetTime End time of current linear movement
     * @version 0.1
     * Method created
     */
    void updateTime(const simtime_t &targetTime);

    /**
     * @brief Initializes the Module for use in Mobility
     * This method gets the current simulation time, cares for the file download (happens only once)
     * of the TLE files from the web and reads the values for the satellites according to the
     * omnet.ini-file. The information is provided by the respective mobility class.
     * @author Marian P. Felder, Florian Schweikowski
     * @param targetTime End time of current linear movement
     * @version 0.1
     * Method created
     */
    virtual void initializeMobility(const simtime_t &targetTime);

    /**
     * @brief Returns the longitude
     * @author Marian P. Felder, Florian Schweikowski
     * @return Longitude
     * @version 0.1
     * Method created
     */
    double getLongitude();

    /**
     * @brief Returns the latitude
     * @author Marian P. Felder, Florian Schweikowski
     * @return Latitude
     * @version 0.1
     * Method created
     */
    double getLatitude();

    /**
     * @brief Returns the elevation to a reference point
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @return Elevation
     * @version 0.1
     * Method created
     */
    double getElevation(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999);

    /**
     * @brief Returns the azimuth
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @returns Azimuth
     * @version 0.1
     * Method created
     */
    double getAzimuth(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999);

    /**
     * @brief Returns the altitude
     * @author Marian P. Felder, Florian Schweikowski, Daniel Merget
     * @return Altitiude
     * @version 0.1
     * Method created
     */
    double getAltitude();

    /**
     * @brief Clean up method
     * If this is called for the first satellite in the network, the downloaded file is removed on call.
     * @author Marian P. Felder
     * @version 0.1
     * Method created
     */
    void finish();

    /**
     * @brief Returns the distance to the satellite from a reference point
     * @author Sarah Lehnhausen, Daniel Merget
     * @return Distance in km
     * @version 0.1
     * Method created
     */
    double getDistance(const double &refLatitude, const double &refLongitude, const double &refAltitude = -9999);

protected:
    /**
     * @brief handleMessage method - no use
     * @author Marian P. Felder, Florian Schweikowski
     * @param msg Incoming cMessage
     * @version 0.1
     * Method created
     */
    virtual void handleMessage(cMessage *msg);

private:

    cEci eci;
    cJulian currentJulian;
    double gap;

    cTle *tle;
    cOrbit *orbit;
    cCoordGeo geoCoord;
    std::string line0, line1, line2, line3;
};

#endif
