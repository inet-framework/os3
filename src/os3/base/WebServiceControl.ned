
package os3.base;

//
// Gets information from a web service (e.g. for Weather Information, TLE Files) and returns them to requesting modules.
//
simple WebServiceControl
{
    parameters:
        @display("i=misc/globe");
        int altitudeCacheThreshold = default(100); // Maximum number of altitudes stored in the cache. Generally, it should always hold altitudeCacheThreshold >= number of base stations
        int tleCacheThreshold = default(10); // Maximum number of TLE data strings stored in cache. Generelly, it should always hold tleCacheThreshold >= number of TLE files used for simulation scenario
        int weatherCacheThreshold = default(10); // Maxmimum number of weather data strings stored in cache. Generally, it should always hold weatherCacheThreshold >= number of base stations
        string apiKeyWeather; // API key for connection with WorldWeatherOnline.com API interface. More infos can be found at www.worldweatheronline.com/free-weather-feed.aspx
        string usernameAltitude; // Username for connection with Geonames.org More infos can be found at www.geonames.org
}
