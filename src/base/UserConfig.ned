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
// Interaction with user parameters set in omnetpp.ini and the omnet simulation. Also creates satellite mobility vectors.
//
// @author Sarah Lehnhausen, Dennis Kaulbars
// @version 0.1
// Module defined
// @version 0.2
// Removed bool fisheye, merged LEO and GEO
//
simple UserConfig
{
    parameters:
        @class(UserConfig); // Module for class UserConfig
        @display("i=block/user"); // Symbol
        double min_snr @unit(dBHz);	// Minimal SNR to enable a transmission
        int numOfSats; // Number of satellites
        double frequency @unit(Hz); // Frequency of Satellite communication antennas in Hertz
}
