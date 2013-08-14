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

#include "WeatherControl.h"

Define_Module(WeatherControl);

void WeatherControl::initialize() {
    // Create reference to WebServiceControl module
    webServiceControl = dynamic_cast< WebServiceControl* >(this->getParentModule()->getSubmodule("webServiceControl"));
    if (webServiceControl == NULL) {
        error("Error in WeatherControl::initialize(): Could not find WebServiceControl module.");
    }

    // Initialize default precipPerHourValue (-1 = not set)
    defaultPrecipPerHour = par("defaultPrecipPerHour");
}

void WeatherControl::handleMessage(cMessage *msg) {
    error("Error in WeatherControl::handleMessage(): This module is not able to handle messages.");
}

void WeatherControl::setWeatherGimmick(const double &latitude, const double &longitude) {
    // Fetch symbol fitting current weather situation
    std::string tmpString = webServiceControl->getWeatherData(latitude, longitude).weatherIconURL;

    // Modify weatherIconURl: Erase URL from string and add weaterIcons folder before file name
    if (tmpString.find("wsymbol") == tmpString.npos) {
        // very likely, the weather poll failed
        EV << "Warning in WeatherControl::setWeatherGimmick(): Could not resolve weatherData from webServiceControl. "
              "Current weather information will not be available! This is probably due to a loss of internet connection "
              "or the service is currently unavailable.\n";
        return;
    }

    tmpString.erase(0, tmpString.find("wsymbol"));
    tmpString.erase(0, tmpString.find("/") + 1);
    tmpString.erase(tmpString.find(".png"), 4);

    std::string symbolString = "weatherIcons/";
    symbolString += tmpString;

    // Set weather symbol to module
    this->getDisplayString().setTagArg("i", 0, symbolString.c_str());

    // Set weather symbol in CNI-OS3 icon to display in map
    this->getParentModule()->getDisplayString().setTagArg("i", 0, symbolString.c_str());

    return;
}

double WeatherControl::getPrecipPerHour(const double &latitude, const double &longitude) {
    // Check if default value is set. If this is the case => return value. Otherwise, fetch live data from WebService module
    if (defaultPrecipPerHour != -1) {
        return defaultPrecipPerHour;
    }

    // Get weather data
    WeatherData currentData = webServiceControl->getWeatherData(latitude, longitude);

    // Set weather icon
    setWeatherGimmick(latitude, longitude);

    return currentData.precipMM / 24; //transform precip per day to precip per hour
}

void WeatherControl::setDefaultPrecipPerHour(double precipValue) {
    defaultPrecipPerHour = precipValue;
}

