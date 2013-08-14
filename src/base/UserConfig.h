#ifndef __CNI_OS3_USERCONFIG_H_
#define __CNI_OS3_USERCONFIG_H_

#include <omnetpp.h>
#include <SatSGP4Mobility.h>
#include <SatSGP4FisheyeMobility.h>
#include <ChannelControl.h>

struct parameters {
    int numOfSats;
    int mapX;
    int mapY;
    double min_snr;
    double frequency;
};

/**
 * @class UserConfig
 * @brief Interaction with User Interface
 * This class manages the interaction with the user parameters and the omnet simulation. It also creates satellite mobility vectors.
 * @author Sarah Lehnhausen, Dennis Kaulbars
 * @version 0.1
 * Class defined
 */
class UserConfig : public cSimpleModule {
private:
    parameters userParameters;
    std::vector< SatSGP4Mobility* > satmoVector;

    ChannelControl *channelcontrol;

protected:
    /**
     * @brief Initialization of an UserConfig object
     * This method initializes an object of type UserConfig and saves the user variables given in omnetpp.ini
     * @author Sarah Lehnhausen, Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    virtual void initialize();

    /**
     * @brief dummy
     * This method is just a dummy method. No action is taking place here.
     * @param msg omnetpp-message
     * @author Sarah Lehnhausen, Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief Initializes the mobilities for the Satellites
     * This method fills the mobility-module-vector with satellite-mobilities (number of satellites set in omnetpp.ini). These vectors are needed for
     * the usage of SatSGP4Mobility and SatSGP4FisheyeMobility in other methods (get the mobility of a specific satellite).
     * @author Sarah Lehnhausen, Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Merged initializeSatMobility() with initializeSatFisheyeMobility()
     */
    void initializeSatMobility();

public:
    /**
     * @brief returns the user settings
     * This method is just a dummy method. No action is taking place here.
     * @return object of type parameters which contains user-specific parameters set in omnetpp.ini
     * @author Sarah Lehnhausen, Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now inline and constant
     */
    inline const parameters& getParameters() const {
        return userParameters;
    };

    /**
     * @brief Initializes the mobility for SatMobility
     * This method fills the mobility-module-vector (satmoGEO and satmoLEO Vectors) with satellite-mobilities (number of satellites set in omnetpp.ini).
     * @return a vector of type LineSegmentsMobilityBase (parent class of SatSGP4Mobilty and SatSGPFisheyeMobility). If an error occurs, the method returns an empty vector.
     * @author Sarah Lehnhausen, Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Method is now inline and constant
     */
    inline const std::vector< SatSGP4Mobility* >& getSatMobility() const {
        return satmoVector;
    };
};

#endif
