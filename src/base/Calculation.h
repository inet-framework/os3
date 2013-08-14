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

#ifndef __CNI_OS3_CALCULATION_H_
#define __CNI_OS3_CALCULATION_H_

#include <math.h>
#include <omnetpp.h>
#include <WeatherControl.h>
#include <UserConfig.h>

/** Source: 'The aR^b Relation in the Calculation of Rain Attenuation', Olsen and Rogers and Hodge
 * a and b are coefficients for different average dropsize distributions to calculate rain specific attenuation
 * LPl: Laws and Parsons Distribution - low rain rate
 * LPh: Laws and Parsons Distribution - high rain rate
 * MP: Marshall-Palmer Distribution
 * Jt: "Thunderstorm" Distribution
 * Jd: "Drizzle" Distribution
 */
struct rainCoefficients {
    double aLPl;
    double aLPh;
    double aMP;
    double aJt;
    double aJd;
    double bLPl;
    double bLPh;
    double bMP;
    double bJt;
    double bJd;
};

/**
 * @class Calculation
 * @brief Helper-class for calculating path loss, distances, etc.
 * @version 0.2
 * Removed some unnecessary member variables and general cleanup
 */
class Calculation : public cSimpleModule {
private:
    // Constants
    static const double C = 299792458; // In m/s;
    static const double Boltzmann = -228.6; // In dBWs/K
    static const double EarthRadius = 6378.137764899274; // In km

    std::string rainTableFile;
    std::map< double, rainCoefficients > rainCoeffMap; // Used for calculation of specific rain attenuation

    UserConfig *userConfig;
    WeatherControl *weatherControl;
    WebServiceControl *webserviceControl;

    struct SAT {
        int satIndex;
        double snr;
    };

protected:

    /**
     * Initializes Calculation module and calls fillRainMap()
     * @author Dennis Kaulbars, Sarah Lehnhausen
     * @version 0.1
     */
    virtual void initialize();

    /**
     * @brief dummy
     * This method is just a dummy method. No action is taking place here.
     * @param msg Omnetpp-message
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * Fills the rainCoeffMap with the Values from CSV file (default: misc/TablespecRain.csv)
     * @author Dennis Kaulbars, Sarah Lehnhausen
     * @version 0.1
     */
    void fillRainMap();

    /**
     * Maps the given frequency to an frequency existing in the table for RainCoefficients
     * @param Frequency Frequency of used system
     * @return Nearest frequency existing in rainCoeffMap
     * @author Dennis Kaulbars, Sarah Lehnhausen
     * @version 0.1
     */
    double getMappedFrequency(const double &frequency);

public:

    /**
     * @brief Convenience function
     * @param rad Angle in radiants
     * @return Angle in degrees
     * @author Daniel Merget
     * @version 0.1
     * created
     */
    static inline double rad2deg(const double &rad);

    /**
     * @brief Convenience function
     * @param deg Angle in degrees
     * @return Angle in radiants
     * @author Daniel Merget
     * @version 0.1
     * created
     */
    static inline double deg2rad(const double &deg);

    /**
     * Calculates the free space loss
     * @param satIndex Index of the satellite
     * @param lambda Wave length
     * @return Free space loss in dB
     * @author Dennis Kaulbars, Sarah Lehnhausen, Daniel Merget
     * @version 0.1
     * created
     * @version 0.2
     * added param mobilityType
     * @version 0.3
     * mobilityType is not used anymore
     * @version 0.4
     * Now requires a reference point (formerly taken from UserConfig)
     */
    double calcFSL(const int &satIndex, const double &lambda, const double &latitude,
                   const double &longitude, const double &altitude = -9999);

    /**
     * Calculates the euclidean distance between two nodes (planar, i.e., without respect to altitude. E.g., sub-satellite point and base station)
     * @param latitude1 Latitude first node
     * @param longitude1 Longitude first node
     * @param latitude2 Latitude second node
     * @param longitude2 Longitude second node
     * @return Planar euclidean distance between the two given nodes
     * @author Dennis Kaulbars, Sarah Lehnhausen, Daniel Merget
     * @version 0.1
     */
    double calcDistance(const double &latitude1, const double &longitude1,
                        const double &latitude2, const double &longitude2);

    /**
     * Calculates the SNR for a transmission (in dBHz)
     * @param transmitterGain Gain of the transmitting antenna in dB
     * @param receiverGain Gain of the receiving antenna in dB
     * @param transmitterPower Transmit Power in dBW
     * @param lambda Wave length in m
     * @param satIndex Index of the satellite that is used
     * @param bandwidth Bandwidth of the used system in Hz
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @param altitude Altitude of the reference point, default -9999 uses altitude data from webservice
     * @param dG Average ratio of antenna radiation from ground, default 0.1
     * @param tR Receiver noise temperature, default 150 K
     * @param dR Highest point of rain area in km, default 3 is average for mild climate
     * @return SNR in dB
     * @author Dennis Kaulbars, Sarah Lehnhausen, Daniel Merget
     * @version 0.1
     * implemented
     * @version 0.2
     * Now requires a reference point (formerly taken from UserConfig)
     * @version 0.3
     * dG and dR are variabel now
     * @version 0.4
     * includes variabel tR
     */
    double calcSNR(
            const double &transmitterGain,     // in dB
            const double &receiverGain,        // in dB
            const double &transmitterPower,    // in dbW
            const double &lambda,              // in m
            const int &satIndex,
            const double &bandwidth,           // in Hz
            const double &latitutde,
            const double &longitude,
            const double &altitude = -9999,
            const double &dG = 0.1,
            const double &tR = 150,
            const double &dR = 3);

   /**
     * Determines the best-in-reach satellite depending on calculated SNR in dBHz
     * @param latitude Latitude of base station
     * @param longitude Longitude of base station
     * @param transmitterGain Gain of the transmitting antenna in dB
     * @param receiverGain Gain of the receiving antenna in dB
     * @param transmitterPower Transmit Power in dBW
     * @param bandwidth Bandwidth of the used system in Hz
     * @param altitude Altitude of the reference point, default -9999 uses altitude data from webservice
     * @param dG Average ratio of antenna radiation from ground, default 0.1
     * @param tR Receiver noise temperature, default 150 K
     * @param dR Highest point of rain area in km, default 3 is average for mild climate
     * @return index of the best-in-reach satellite
     * @author Dennis Kaulbars, Sarah Lehnhausen, Daniel Merget
     * @version 0.1
     * implemented
     * @version 0.2
     * Now requires more input parameters (fitting calcSNR parameters)
     */
    int getScoredSatfromSNR(
            const double &latitude,
            const double &longitude,
            const double &transmitterGain,     // in dB
            const double &receiverGain,        // in dB
            const double &transmitterPower,    // in dbW
            const double &bandwidth,           // in Hz
            const double &altitude = -9999,
            const double &dG = 0.1,
            const double &tR = 150,
            const double &dR = 3);
};


#endif

