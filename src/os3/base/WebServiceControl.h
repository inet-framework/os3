//-----------------------------------------------------
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
//-----------------------------------------------------

#ifndef __OS3_WebServiceControl_H__
#define __OS3_WebServiceControl_H__

#include <omnetpp.h>

#include <curl/curl.h>
#include <curl/easy.h>

struct WeatherData
{
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

struct TLEData
{
    std::string tleName;
    std::string tleLine1;
    std::string tleLine2;
};


//-----------------------------------------------------
// Class: WebServiceControl
//
// Pulls data for live weather, TLE and altitude
//-----------------------------------------------------
class WebServiceControl : public cSimpleModule
{
public:
    // returns live weather data for initialized region
    WeatherData getWeatherData(const double& latitude, const double& longitude);

    // returns altitude data
    double getAltitudeData(const double& latitude, const double& longitude);

    // returns actual TLE data for requested satellite (characterized by specific satellite number)
    // - the data is taken from celestrak.com. The file name must be identical to the one which is located at the website
    // - (e.g. the ISS can be found in file "stations.txt" so the variable fileName has to be set to "stations.txt").
    // - The method extracts the satellite number numSat (beginning from 0). So the satellite has to be present in celestrak's textfile.
    // fileName: file name of the TLE-File (corresponding to file located at www.clestrak.com/NORAD/elements/xxx.txt)
    // numSat: number of satellite for which TLE data is requested
    TLEData getTLEData(std::string fileName, unsigned int numSat);

    // Returns actual TLE data for requested satellite (characterized by specific satellite name)
    // - this method returns actual TLE data for a number of satellites which are contained in file with file name fileName.
    // - the data is taken from celestrak.com. The file name must be identical to the one which is located at the website
    // - (e.g. the ISS can be found in file "stations.txt" so the variable fileName has to be set to "stations.txt").
    // - the method extracts the satellite data for the satellite named with satName. So the satellite has to be present in celestrak's textfile.
    // fileName: file name of the TLE-File (corresponding to file located at www.clestrak.com/NORAD/elements/xxx.txt)
    // satName: name of the satellite for which TLE data is requested
    TLEData getTLEData(std::string fileName, std::string satName);

    // checks if an url exists
    bool urlExist(std::string url);

protected:
    virtual void initialize();

    virtual void handleMessage(cMessage* msg);

    // creates the request string for the weatherData request
    std::string getRequestStringWeatherData(const double& latitude, const double& longitude);

    // creates the request string for the altitude data request
    std::string getRequestStringAltitudeData(const double& latitude, const double& longitude);

    // creates the request string for the TLE data request
    std::string getRequestStringTLEData(std::string fileName);

    // creates the request for the weatherData
    std::string requestWeatherData(const double& latitude, const double& longitude);

    // creates the request for the altitude data
    double requestAltitudeData(const double& latitude, const double& longitude);

    /**
     * Creation of request for TLE data
     * This method creates the request for the TLE data. If an entry for requested TLE file is saved in tleCache, the saved data string is returned instead of
     * another request to the website is taken.
     * If the datastring is not in cache, data is loaded from the website and saved in cache in order to reduce latency for further requests.
     * @param fileName file name of the TLE file located at www.celestrak.com/NORAD/elements/xxx.txt
     * @return String containing the data of the TLE file with actual TLE data taken from www.celestrak.com
     */
    std::string requestTLEData(std::string fileName);

    /**
     * Evaluate weather data
     * This method evaluates the fetched weather data and parse them into a WeatherData struct.
     * @param dataString String containing the data of a .csv weather file from www.worldweatheronline.com
     * @return Weather data for requested region
     */
    WeatherData evaluateWeatherInformation(std::string dataString);

    /**
     * Evaluate TLE data
     * This method evaluates the fetched TLE data and parse the TLE data of the numSat satellite into a TLEData struct.
     * @param dataString String containing the data of a TLE file from www.celestrak.com/NORAD/elements/xxx.txt
     * @return TLEData for requested satellite
     */
    TLEData evaluateTLEData(std::string dataString, unsigned int numSat);

    /**
     * Evaluate TLE data
     * This method evaluates the fetched TLE data and parse the TLE data of the satellite with name satName into a TLEData struct.
     * @param dataString String containing the data of a TLE file from www.celestrak.com/NORAD/elements/xxx.txt
     * @return TLEData for requested satellite
     */
    TLEData evaluateTLEData(std::string dataString, std::string satName);

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
};

#endif
