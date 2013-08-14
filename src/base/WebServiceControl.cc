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

#include "WebServiceControl.h"

Define_Module(WebServiceControl);

void WebServiceControl::initialize() {
    // Read parameters
    altitudeCacheThreshold = par("altitudeCacheThreshold");
    tleCacheThreshold = par("tleCacheThreshold");
    weatherCacheThreshold = par("weatherCacheThreshold");
    weatherApiKey = par("apiKeyWeather").stringValue();
    altitudeUsername = par("usernameAltitude").stringValue();

    EV << "Web services are now available!" << endl;

    return;
}

void WebServiceControl::handleMessage(cMessage *msg) {
    error("Error in WebServiceControl::handleMessage(): This module is not able to handle messages");
}

std::string WebServiceControl::getRequestStringWeatherData(const double &latitude, const double &longitude) {

    std::ostringstream requestStream;

    // Create URL
    requestStream << "http://free.worldweatheronline.com/feed/weather.ashx?";

    // Check if URL exists
//    if (urlExist("http://free.worldweatheronline.com/") == false) {
//        error("Error in WebServiceControl::getRequestStringWeatherData(): URL does not exist!");
//        return "";
//    }

    // Latitude and longitude
    requestStream << "q=" << latitude << "," << longitude;

    // Format
    requestStream << "&format=csv&num_of_days=1";

    // API Key
    requestStream << "&key=" << weatherApiKey;

    return requestStream.str();
}

std::string WebServiceControl::getRequestStringAltitudeData(const double &latitude, const double &longitude) {
    std::ostringstream requestStream;

    // Check if URL exists
//    if (urlExist("http://api.geonames.org/") == false) {
//        error("Error in WebServiceControl::getRequestStringAltitudeData(): URL does not exist!");
//        return "";
//    }

    // Create URL
    requestStream << "http://api.geonames.org/astergdem?";

    // Latitude and longitude
    requestStream << "lat=" << latitude << "&lng=" << longitude;

    // Username
    requestStream << "&username=" << altitudeUsername;

    return requestStream.str();
}

std::string WebServiceControl::getRequestStringTLEData(std::string fileName) {
    std::ostringstream requestStream;

    // Create URL
    requestStream << "http://www.celestrak.com/NORAD/elements/" << fileName;

    // Check if URL exists
//    if (urlExist(requestStream.str()) == false) {
//        error("Error in WebServiceControl::getRequestStringTLEData(): URL does not exist!");
//        return "";
//    }

    return requestStream.str();
}

/**
 * @brief Conversion of cURL result to string
 * This method transforms a cURL result set into a std::string value. The intention of this method is to reduce the cost for additional saves
 * of result files on user's hard disk. The method is taken from http://www.c-plusplus.de/forum/262216
 * @author Dennis Kaulbars
 * @version 0.1
 * Method defined
 */
size_t write_data(char *ptr, size_t size, size_t nmemb, std::string *targetString) {
    if (targetString) {
        targetString->append(ptr, size * nmemb);
        return size * nmemb;
    }
    return 0;
}

bool WebServiceControl::urlExist(std::string url) {
    // Check if url is set
    if (url.size() == 0) {
        return false;
    }

    // Create request, Output is set to null-device, because it is not needed (only interested in status)
    FILE *null_device = std::fopen("/dev/null", "w");
    std::string resultString;
    CURL* easyHandle = curl_easy_init();
    curl_easy_setopt(easyHandle, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(easyHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(easyHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, null_device);

    // Exequte check
    if (curl_easy_perform(easyHandle) != 0) {
        curl_easy_cleanup(easyHandle);
        return false;
    }

    curl_easy_cleanup(easyHandle);
    return true;
}

std::string WebServiceControl::requestWeatherData(const double &latitude, const double &longitude) {
    // Check if TLE data with name fileName is in cache
    std::map< std::pair< double, double >,std::string >::iterator cacheIt = weatherCache.find(
            std::pair< double, double >(latitude, longitude));
    if (cacheIt != weatherCache.end()) {
        // Entry found! Return content of weatherCache => finish
        std::string resultString = cacheIt->second;
        return resultString;
    }

    // Initialize cURL variables
    CURL* easyHandle = curl_easy_init();

    // Create request string
    std::string requestString = getRequestStringWeatherData(latitude, longitude);

    // Take necessary options for cURL handler
    std::string resultString;
    curl_easy_setopt(easyHandle, CURLOPT_URL, requestString.c_str());
    curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, &resultString);
    curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, &write_data);

    // Fetch data from website, result is saved in resultString
    curl_easy_perform(easyHandle);
    curl_easy_cleanup(easyHandle);

    // Save resultString in cache for further requests if cache is not full
    if (weatherCache.size() < weatherCacheThreshold) {
        std::pair< double, double > temp = std::pair< double, double >(latitude, longitude);
        weatherCache.insert(std::pair< std::pair< double, double >,std::string >(temp, resultString));
    }

    return resultString;
}

double WebServiceControl::requestAltitudeData(const double &latitude, const double &longitude) {
    // Initialize cURL variables
    CURL* easyHandle = curl_easy_init();

    // Create request string
    std::string requestString = getRequestStringAltitudeData(latitude, longitude);

    // Take necessary options for cURL handler
    std::string resultString;
    curl_easy_setopt(easyHandle, CURLOPT_URL, requestString.c_str());
    curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, &resultString);
    curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, &write_data);

    // Fetch data from website, result is saved in resultString
    curl_easy_perform(easyHandle);
    curl_easy_cleanup(easyHandle);

    return atof(resultString.c_str());
}

std::string WebServiceControl::requestTLEData(std::string fileName) {
    // Check if TLE data with name fileName is in cache
    std::map< std::string, std::string >::iterator cacheIt = tleCache.find(fileName);
    if (cacheIt != tleCache.end()) {
        //Entry found! Return content of TLEfile => finish
        std::string resultString = cacheIt->second;
        return resultString;
    }

    // Entry not in cache, load data and save them in cache for further requests

    // Initialize cURL variables
    CURL* easyHandle = curl_easy_init();

    // Create request string
    std::string requestString = getRequestStringTLEData(fileName);

    // Take necessary options for cURL handler
    std::string resultString;
    curl_easy_setopt(easyHandle, CURLOPT_URL, requestString.c_str());
    curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, &resultString);
    curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, &write_data);

    // Fetch data from website, result is saved in resultString
    curl_easy_perform(easyHandle);
    curl_easy_cleanup(easyHandle);

    // Save resultString in cache for further requests if cache is not full
    if (tleCache.size() < tleCacheThreshold) {
        tleCache.insert(std::pair< std::string, std::string >(fileName, resultString));
    }

    return resultString;
}

WeatherData WebServiceControl::evaluateWeatherInformation(std::string dataString) {
    WeatherData resultData;

    // Go to line 10 (contains interesting data) => skip first 9 lines
    for (int i = 0; i < 9; i++) {
        dataString = dataString.erase(0, dataString.find("\n") + 1);
    }

    // Evaluate weatherLine. syntax: date,tempMaxC,tempMaxF,tempMinC,tempMinF,windspeedMiles,windspeedKmph,winddirDegree,winddir16Point,weatherCode,weatherIconUrl,weatherDesc,precipMM
    resultData.date = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);

    std::string temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.tempMaxC = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.tempMaxF = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.tempMinC = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.tempMinF = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.windSpeedMiles = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.windSpeedKmph = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.windDirDegree = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.windDir16Point = atof(temp.c_str());

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.weatherCode = atof(temp.c_str());

    resultData.weatherIconURL = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);

    resultData.weatherDesc = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);

    temp = dataString.substr(0, dataString.find(","));
    dataString.erase(0, dataString.find(",") + 1);
    resultData.precipMM = atof(temp.c_str());

    return resultData;
}

TLEData WebServiceControl::evaluateTLEData(std::string dataString, unsigned int numSat) {
    TLEData resultData;

    // Count number of lines in text file => number of lines / 3 = number of satellites in textfile
    unsigned int numSatTXT = 0;
    std::string tmpDataString = dataString;

    // Find out number of satellites
    while (tmpDataString.empty() == false) {
        tmpDataString.erase(0, tmpDataString.find("\n") + 1);
        numSatTXT++;
    }
    numSatTXT = numSatTXT / 3;

    // Check if number of satellites contained in dataString fits to numSat
    if (numSat >= numSatTXT) {
        error(
                "Error in WebServiceControl::evaluateTLEData(): numSat too big (greater or equal than number of satellites contained in textfile)");
        return resultData;
    }

    // Go to line numSat*3
    for (unsigned int i = 0; i < numSat * 3; i++) {
        dataString.erase(0, dataString.find("\n") + 1);
    }

    // Fetch data of satellite numSat
    resultData.tleName = dataString.substr(0, dataString.find("\n"));
    dataString.erase(0, dataString.find("\n") + 1);

    resultData.tleLine1 = dataString.substr(0, dataString.find("\n"));
    dataString.erase(0, dataString.find("\n") + 1);

    resultData.tleLine2 = dataString.substr(0, dataString.find("\n"));
    dataString.erase(0, dataString.find("\n") + 1);

    // Pretty up name in TLE data
    resultData.tleName.erase(resultData.tleName.find("  "), resultData.tleName.size() - resultData.tleName.find(" "));

    return resultData;
}

TLEData WebServiceControl::evaluateTLEData(std::string dataString, std::string satName) {
    TLEData resultData;

    // Find satellite with name satName
    unsigned int pos = dataString.find(satName);

    if (pos == std::string::npos) {
        error("Error in WebServiceControl::evaluateTLEData(): satellite can not be found!");
        return resultData;
    }
    else {
        // Extract TLE data and parse them into the result struct
        dataString = dataString.erase(0, pos);

        resultData.tleName = dataString.substr(0, dataString.find("\n"));
        dataString.erase(0, dataString.find("\n") + 1);

        resultData.tleLine1 = dataString.substr(0, dataString.find("\n"));
        dataString.erase(0, dataString.find("\n") + 1);

        resultData.tleLine2 = dataString.substr(0, dataString.find("\n"));
    }

    // Pretty up name in TLE data
    resultData.tleName.erase(resultData.tleName.find("  "), resultData.tleName.size() - resultData.tleName.find(" "));

    return resultData;
}

WeatherData WebServiceControl::getWeatherData(const double &latitude, const double &longitude) {
    // Fetch current weather data
    std::string weatherData = requestWeatherData(latitude, longitude);
    WeatherData currentWeatherData = evaluateWeatherInformation(weatherData);

    return currentWeatherData;
}

double WebServiceControl::getAltitudeData(const double &latitude, const double &longitude) {
    double currentAltitude = -9999;

    std::pair< double, double > key = std::make_pair(latitude, longitude);

    // Check if there is a matching cached value
    try {
        currentAltitude = altitudeCache.at(key); // Throws std::out_of_range

        // Rearrange the map, such that the latest requested value comes last
        altitudeCache.erase(key);
        altitudeCache.insert(altitudeCache.end(), std::make_pair(key, currentAltitude));
    }
    catch (std::out_of_range &oor) {
        // Value is not cached => fetch current altitude data
        currentAltitude = requestAltitudeData(latitude, longitude);

        // Check if cached altitude is an existing value or an error occured (-9999)
        if (currentAltitude == -9999) {
            error("Error in WebServiceControl::getAltitudeData(): Fetched value for altitude data is invalid!");
            return -9999;
        }

        // Add the value to the cache
        altitudeCache.insert(altitudeCache.end(), std::make_pair(key, currentAltitude));

        // Ensure that the size of the map does not get too large (this could happen, e.g., for mobile base stations)
        // In such a case, delete the oldest element (the first in the map)
        if (altitudeCache.size() > altitudeCacheThreshold)
            altitudeCache.erase(altitudeCache.begin());
    } // End catch

    return currentAltitude;
}

TLEData WebServiceControl::getTLEData(std::string fileName, unsigned int numSat) {
    std::cout << "satellite index: " << numSat << std::endl;
    // Fetch TLE data file
    std::string dataString = requestTLEData(fileName);

    // Evaluate data and return result
    return evaluateTLEData(dataString, numSat);
}

TLEData WebServiceControl::getTLEData(std::string fileName, std::string satName) {
    // Fetch TLE data file
    std::string dataString = requestTLEData(fileName);

    // Evaluate data and return result
    return evaluateTLEData(dataString, satName);
}
