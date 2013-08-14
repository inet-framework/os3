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

package cni_os3.base;

//
// Gets information from a web service (e.g. for Weather Information, TLE Files) and returns them to requesting modules.
//
// @author Dennis Kaulbars, Daniel Merget
// @version 0.1
// Module defined and Integration of live weather information
// @version 0.2
// Integrated live altitude data
// @version 0.3
// Altitude data is now cached in a map in order to reduce web service requests
// @version 0.4
// Integrated TLE data service (either satellite name or satellite index) inclusive local cache in order to reduce requests to Web Service
// @version 0.5
// Integrated local cache for weather data in order to reduce requests to Web Service
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
