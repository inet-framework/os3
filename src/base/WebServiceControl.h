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

#ifndef __CNI_OS3_WEBSERVICECONTROL_H_
#define __CNI_OS3_WEBSERVICECONTROL_H_

#include <omnetpp.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <cstdio>

struct WeatherData {
    std::string date;
    double tempMaxC;
    double tempMaxF;
    double tempMinC;
    double tempMinF;
    double windSpeedMiles;
    double windSpeedKmph;
    double windDirDegree;
    double windDir16Point;
    double weatherCode;
    std::string weatherIconURL;
    std::string weatherDesc;
    double precipMM;
};

struct TLEData {
    std::string tleName;
    std::string tleLine1;
    std::string tleLine2;
};

/**
 * @class WebServiceControl
 * @brief Contributes informations from WebServices.
 *
 *  This class gets information from WebServices (e.g. weather information) and returns them to requesting modules.
 *
 * @author Dennis Kaulbars
 * @version 0.1
 * Class defined and Integration of live weather information
 * @version 0.2
 * Added LibCURL for downloading and handling files instead of classical wget-Method
 * @version 0.3
 * Included local cache for altitude data in order to reduce requests to Web Service
 * @version 0.4
 * Integrated TLE data service (either satellite name or satellite index) inclusive local cache in order to reduce requests to Web Service
 * @version 0.5
 * Integrated local cache for weather data in order to reduce requests to Web Service
 */
class WebServiceControl : public cSimpleModule {
private:
    //Variables
    std::string weatherApiKey;
    std::string altitudeUsername;
    unsigned int altitudeCacheThreshold;
    unsigned int tleCacheThreshold;
    unsigned int weatherCacheThreshold;
    std::map< std::pair< double, double >, double > altitudeCache;
    std::map< std::string, std::string > tleCache;
    std::map< std::pair< double, double >, std::string > weatherCache;

protected:
    /**
     * @brief Initialize class
     * This method initializes the class and creates a reference to the userConfig module.
     * @author Dennis Kaulbars
     * @version 0.1
     * Method defined
     */
    virtual void initialize();

    /**
     * @brief Dummy
     * This method is just a dummy method. No action is taking place here.
     * @param msg omnetpp-message
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief Creation of request string for weather data
     * This method creates the request string for the weatherData request.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return String in format of a request for worldweatheronline.com
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    std::string getRequestStringWeatherData(const double &latitude, const double &longitude);

    /**
     * @brief Creation of request string for altitude data
     * This method creates the request string for the altitude data request.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return String in format of a request for geonames.org using astergdem database
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    std::string getRequestStringAltitudeData(const double &latitude, const double &longitude);

    /**
     * @brief Creation of request string for TLE data
     * This method creates the request string for the TLE data request.
     * @param fileName file name of the TLE data file located at www.celestrak.com/NORAD/elements/xxx.txt
     * @return String in format of a request for celestrak.com
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    std::string getRequestStringTLEData(std::string fileName);

    /**
     * @brief Creation of request for weather data
     * This method creates the request for the weatherData.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return String containing the data of the .csv file with live weather data from www.worldweatheronline.com
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    std::string requestWeatherData(const double &latitude, const double &longitude);

    /**
     * @brief Creation of request for altitude data
     * This method creates the request for the altitude data.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return Double containing the altitude value of the website with live altitude data taken from www.geonames.org using astagdem database
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    double requestAltitudeData(const double &latitude, const double &longitude);

    /**
     * @brief Creation of request for TLE data
     * This method creates the request for the TLE data. If an entry for requested TLE file is saved in tleCache, the saved data string is returned instead of
     * another request to the website is taken.
     * If the datastring is not in cache, data is loaded from the website and saved in cache in order to reduce latency for further requests.
     * @param fileName file name of the TLE file located at www.celestrak.com/NORAD/elements/xxx.txt
     * @return String containing the data of the TLE file with actual TLE data taken from www.celestrak.com
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    std::string requestTLEData(std::string fileName);

    /**
     * @brief Evaluate weather data
     * This method evaluates the fetched weather data and parse them into a WeatherData struct.
     * @param dataString String containing the data of a .csv weather file from www.worldweatheronline.com
     * @return Weather data for requested region
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    WeatherData evaluateWeatherInformation(std::string dataString);

    /**
     * @brief Evaluate TLE data
     * This method evaluates the fetched TLE data and parse the TLE data of the numSat satellite into a TLEData struct.
     * @param dataString String containing the data of a TLE file from www.celestrak.com/NORAD/elements/xxx.txt
     * @return TLEData for requested satellite
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    TLEData evaluateTLEData(std::string dataString, unsigned int numSat);

    /**
     * @brief Evaluate TLE data
     * This method evaluates the fetched TLE data and parse the TLE data of the satellite with name satName into a TLEData struct.
     * @param dataString String containing the data of a TLE file from www.celestrak.com/NORAD/elements/xxx.txt
     * @return TLEData for requested satellite
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    TLEData evaluateTLEData(std::string dataString, std::string satName);

public:
    /**
     * @brief Returns live weather data for initialized region
     * This method returns live weather data for a reference point. Data is fetched from
     * www.worldweatheronline.com via free local weather API.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return Weather data
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     */
    WeatherData getWeatherData(const double &latitude, const double &longitude);

    /**
     * @brief Returns live weather data for initialized region
     * This method returns live altitude data for a reference point. Data is fetched from
     * www.geonames.org using astergdem database (30mx30m rastered, worldwide database) and saved in
     * a local cache in order to reduce unnecessary requests to the geonames database. If the cache
     * contains too many elements, the oldest accessed element will be deleted from the local storage.
     * @param latitude Coordinate for the reference point
     * @param longitude Coordinate for the reference point
     * @return Weather data. If an error occurs this method returns -9999 as altitude value.
     * @author Dennis Kaulbars, Daniel Merget
     * @version 0.1
     * Method implemented
     * @version 0.2
     * Coordinates for the reference point are now required (formerly taken from UserConfig)
     * @version 0.3
     * Added a cache, always storing the latest 'altitudeCacheThreshold' number of requests in a map (oldest first in map, latest last)
     */
    double getAltitudeData(const double &latitude, const double &longitude);

    /**
     * @brief Returns actual TLE data for requested satellite (characterized by specific satellite number)
     * This method returns actual TLE data for a number of satellites which are contained in file with file name fileName.
     * The data is taken from celestrak.com. The file name must be identical to the one which is located at the website
     * (e.g. the ISS can be found in file "stations.txt" so the variable fileName has to be set to "stations.txt").
     * The method extracts the satellite number numSat (beginning from 0). So the satellite has to be present in celestrak's textfile.
     * @param fileName file name of the TLE-File (corresponding to file located at www.clestrak.com/NORAD/elements/xxx.txt)
     * @param numSat number of satellite for which TLE data is requested
     * @return TLEData containing the TLE data for the requested satellites. On error, error will be displayed and TLEData struct will be empty
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    TLEData getTLEData(std::string fileName, unsigned int numSat);

    /**
     * @brief Returns actual TLE data for requested satellite (characterized by specific satellite name)
     * This method returns actual TLE data for a number of satellites which are contained in file with file name fileName.
     * The data is taken from celestrak.com. The file name must be identical to the one which is located at the website
     * (e.g. the ISS can be found in file "stations.txt" so the variable fileName has to be set to "stations.txt").
     * The method extracts the satellite data for the satellite named with satName. So the satellite has to be present in celestrak's textfile.
     * @param fileName file name of the TLE-File (corresponding to file located at www.clestrak.com/NORAD/elements/xxx.txt)
     * @param satName name of the satellite for which TLE data is requested
     * @return TLEData containing the TLE data for the requested satellites. On error, error will be displayed and TLEData struct will be empty
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    TLEData getTLEData(std::string fileName, std::string satName);

    /**
     * @brief Checks if an url exists
     * This method checks if an URL exists or not (=code 404). Checking procedure is done via libCURL.
     * @param url url which has to be checked (inclusive "http://")
     * @return true if url exists, false if not
     * @author Dennis Kaulbars
     * @version 0.1
     * Method implemented
     */
    bool urlExist(std::string url);
};

#endif
