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
// Gets weather information from WebService and integrates them in the simulation.
//
// @author Sarah Lehnhausen, Dennis Kaulbars
// @version 0.1
// Module defined
// @version 0.2
// Integrated default value handling
//
simple WeatherControl
{
    parameters:
        @display("i=misc/sun"); // Symbol, depending on actual weather situation
        double defaultPrecipPerHour = default(-1); // Default precipitation (=rainfall) per hour value. If set to -1, then system is using live weather data.
}
