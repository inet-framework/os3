
#include "os3/base/WeatherControl.h"

#include "os3/base/WebServiceControl.h"

Define_Module(WeatherControl);

void WeatherControl::initialize()
{
    // Create reference to WebServiceControl module
    webServiceControl = dynamic_cast< WebServiceControl* >(getParentModule()->getSubmodule("webServiceControl"));
    if (webServiceControl == nullptr) {
        error("Error in WeatherControl::initialize(): Could not find WebServiceControl module.");
    }

    // Initialize default precipPerHourValue (-1 = not set)
    defaultPrecipPerHour = par("defaultPrecipPerHour");
}

void WeatherControl::handleMessage(cMessage* msg)
{
    error("Error in WeatherControl::handleMessage(): This module is not able to handle messages.");
}

void WeatherControl::setWeatherGimmick(const double& latitude, const double& longitude)
{
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
    getDisplayString().setTagArg("i", 0, symbolString.c_str());

    // Set weather symbol in CNI-OS3 icon to display in map
    getParentModule()->getDisplayString().setTagArg("i", 0, symbolString.c_str());

    return;
}

double WeatherControl::getPrecipPerHour(const double& latitude, const double& longitude)
{
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

void WeatherControl::setDefaultPrecipPerHour(const double precipValue)
{
    defaultPrecipPerHour = precipValue;
}
