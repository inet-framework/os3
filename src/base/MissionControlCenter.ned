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
import os3.mobility.LUTMotionMobility;
import inet.base.NotificationBoard;

//
// Bundles the analyzation and notification modules for the OS³ missions.
//
// @author Marian P. Felder, Florian Schweikowski, Sarah Lehnhausen, Dennis Kaulbars
// @version 0.1
// Module defined
//
module MissionControlCenter
{
    parameters:
        string cityName = default("");
        @display("t=$cityName");
        @node;
        
    gates:
        input radioIn; // Input gate
        output radioOut; // Output gate

    submodules:
        mobility: LUTMotionMobility { // Module for mobility (LUTMotionMobility)
            parameters:
                @display("p=54,256;i=block/cogwheel_s");
        }
        notificationBoard: NotificationBoard { // Module for Notfication on event occuring
            parameters:
                @display("p=182,248;i=block/control");
        }
    connections allowunconnected: // Connections between compound gates and submodules

}
