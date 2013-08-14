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
// Observer module
//
// @author Sarah Lehnhausen
simple Observer
{
    double ObserverLongitude; // Observer longitude
    double ObserverLatitude; // Observer latitude
    double ObserverAltitude; // Observer altitude
    double TimerInterval @unit(s); // Update interval for timer
    int numgps; // Number of GPS satellites used
    bool gps; // Check of GPS validation or ISS validation should run
    @display("i=block/user");

}
