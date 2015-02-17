
#ifndef __OS3_Calculation_H__
#define __OS3_Calculation_H__

#include <omnetpp.h>

/** Source: 'The aR^b Relation in the Calculation of Rain Attenuation', Olsen and Rogers and Hodge
 * a and b are coefficients for different average dropsize distributions to calculate rain specific attenuation
 * LPl: Laws and Parsons Distribution - low rain rate
 * LPh: Laws and Parsons Distribution - high rain rate
 * MP: Marshall-Palmer Distribution
 * Jt: "Thunderstorm" Distribution
 * Jd: "Drizzle" Distribution
 */
struct rainCoefficients
{
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

class UserConfig;
class WeatherControl;
class WebServiceControl;

//-----------------------------------------------------
// Class: Calculation
// Helper-class for calculating path loss, distances, etc.
//-----------------------------------------------------
class Calculation : public cSimpleModule
{
public:

    /**
     * @brief Convenience function
     * @param rad Angle in radiants
     * @return Angle in degrees
     */
    static inline double rad2deg(const double& rad);

    /**
     * @brief Convenience function
     * @param deg Angle in degrees
     * @return Angle in radiants
     */
    static inline double deg2rad(const double& deg);

    /**
     * Calculates the free space loss
     * @param satIndex Index of the satellite
     * @param lambda Wave length
     * @return Free space loss in dB
     */
    double calcFSL(const int& satIndex, const double& lambda, const double& latitude,
                   const double& longitude, const double& altitude = -9999);

    /**
     * Calculates the euclidean distance between two nodes (planar, i.e., without respect to altitude. E.g., sub-satellite point and base station)
     * @param latitude1 Latitude first node
     * @param longitude1 Longitude first node
     * @param latitude2 Latitude second node
     * @param longitude2 Longitude second node
     * @return Planar euclidean distance between the two given nodes
     */
    double calcDistance(const double& latitude1, const double& longitude1,
                        const double& latitude2, const double& longitude2);

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
     */
    double calcSNR(
            const double& transmitterGain,     // in dB
            const double& receiverGain,        // in dB
            const double& transmitterPower,    // in dbW
            const double& lambda,              // in m
            const int& satIndex,
            const double& bandwidth,           // in Hz
            const double& latitutde,
            const double& longitude,
            const double& altitude = -9999,
            const double& dG = 0.1,
            const double& tR = 150,
            const double& dR = 3);

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
     */
    int getScoredSatfromSNR(
            const double& latitude,
            const double& longitude,
            const double& transmitterGain,     // in dB
            const double& receiverGain,        // in dB
            const double& transmitterPower,    // in dbW
            const double& bandwidth,           // in Hz
            const double& altitude = -9999,
            const double& dG = 0.1,
            const double& tR = 150,
            const double& dR = 3);

protected:

    // initializes Calculation module and calls fillRainMap()
    virtual void initialize();

    virtual void handleMessage(cMessage* msg);

    // fills the rainCoeffMap with the Values from CSV file (default: data/TablespecRain.csv)
    void fillRainMap();

    // Maps the given frequency to an frequency existing in the table for RainCoefficients
    // frequency: frequency of used system
    // returns nearest frequency existing in rainCoeffMap
    double getMappedFrequency(const double& frequency);

private:

    static const double C;              // In m/s;
    static const double Boltzmann;      // In dBWs/K
    static const double EarthRadius;    // In km

    std::string rainTableFile;

    // Used for calculation of specific rain attenuation
    std::map< double, rainCoefficients > rainCoeffMap;

    UserConfig* userConfig;
    WeatherControl* weatherControl;
    WebServiceControl* webserviceControl;

    struct SAT {
        int satIndex;
        double snr;
    };
};

#endif

