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


package os3.base;

import inet.base.NotificationBoard;

import inet.mobility.contract.IMobility;
import os3.mobility.Norad;
import os3.mobility.SatSGP4Mobility;

module Satellite
{
    parameters:
        @nodes();
        @node; //because of MobilityBase initialization
        string satelliteName = default(""); // Parameter for satellite name
        string mobilityType = default("SatSGP4Mobility"); // Define mobility module
        // Transmit Power of satellite (7dBW as example for CospasSarsat system)
        double transmitPower @unit(dBW) @display("i=device/satellite;bgb=324,226") = default(7dBW); // Symbol
        @display("t=$satelliteName");
    gates:
        input radioIn; // Input gate
        output radioOut; // Output gate

    submodules:
        mobility: <mobilityType> like IMobility { // Module for mobility (parent class -> BasicMobility), BasicMobility -> IMobility
            @display("p=96,94");
        }
        //mobility : SatSGP4Mobility {
        //     parameters:
        //     @display("p=194,103");
        //}
        notificationBoard: NotificationBoard { // Module for event notification
            parameters:
                @display("p=221,94");
        }
        NoradModule: Norad { // Module for transformation between omnet and libNorad
            parameters:
                @display("p=98,35");
        }
    connections allowunconnected:

}
