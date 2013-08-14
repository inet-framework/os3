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

#ifndef __CNI_OS3_WEATHERCONTROL_H_
#define __CNI_OS3_WEATHERCONTROL_H_

#include <omnetpp.h>
#include <WebServiceControl.h>

/**
 * @class WeatherControl
 * @brief Contributes weather information for the simulation model.
 * This class gets weather information from web service and integrates them in the simulation.
 * @author Sarah Lehnhausen, Dennis Kaulbars
 * @version 0.1
 * Class defined
 */
class WeatherControl : public cSimpleModule {
private:
    WebServiceControl *webServiceControl;
    double defaultPrecipPerHour;

protected:
    /**
     * @brief Initialize reference to WebServiceControl module and set weather icon.
     * This method initializes a connection to the WebServiceControl module and predefines weather icon.
     * @author Sarah Lehnhausen, Dennis Kaulbars
     * @version 0.1
     * Method defined
     */
    virtual void initialize();

    /**
     * @brief Dummy
     * This method is just a dummy method. No action is taking place here.
     * @param msg omnet-message
     * @author Sarah Lehnhausen, Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    virtual void handleMessage(cMessage *msg);

public:
    /**
     * @brief Sets weather symbol depending on weather situation.
     * This method sets the weather symbol of the module. The symbol is fetched from the WebServiceControl module, which returns
     * an symbol filename fitting the current weather situation.
     * In former versions this method used an weather index describing the statistically developed weather situation
     * (1: rainy, 2: cloudy, 3: suncloudy, 4: sunny)
     * @param latitude coordinate for the reference point
     * @param longitude coordinate for the reference point
     * @author Sarah Lehnhausen, Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method defined
     * @version 0.2
     * Added support for new enum type
     * @version 0.3
     * Version is using live weather data from WebServiceControl module, weather index is now not needed any more
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    void setWeatherGimmick(const double &latitude, const double &longitude);

    /**
     * @brief Gets the current (live!) precipation per hour
     * This method gets the current precipation (=rainfall) per hour value.
     * If a default value is NOT set (defaultPrecipPerHour = -1), it fetches live (!) weather data from the
     * Web service module. Does not update the weatherGimmick.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return Current precipation in mm/(m²*h)
     * @author Sarah Lehnhausen, Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method defined
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     * @version 0.3
     * Integrated default value handling
     */
    double getPrecipPerHour(const double &latitude, const double &longitude);

    /**
     * @brief sets the default (not live!) precipation per hour
     * This method sets the default precip (=rainfall) per hour value if live weather data should not be evaluated for a scenario.
     * Does not update the weatherGimmick.
     * @param precipValue Precipitation (rainfall per hour value)
     * @author Dennis Kaulbars
     * @version 0.1
     * Method defined
     */
    void setDefaultPrecipPerHour(double precipValue);
};

#endif
