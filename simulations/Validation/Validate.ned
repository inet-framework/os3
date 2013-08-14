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

package cni_os3.simulations.Validation;

import cni_os3.base.Satellite;
import inet.world.radio.ChannelControl;
import cni_os3.base.CNI_OS3;
import cni_os3.base.Observer;

//
// Network Validate used to show Observer functionalities
//
// @author Sarah Lehnhausen
network Validate
{
    parameters:
        double playgroundSizeX; // Width of map in pixels
        double playgroundSizeY; // Height of map in pixels
        int numOfSats;	// Number of satellites

        @display("bgi=background_earth");

    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=80,200;r=10,,#707070;i=device/satellite_l");
        }
        channelcontrol: ChannelControl {
            parameters:
                //playgroundSizeX = playgroundSizeX;
                //playgroundSizeY = playgroundSizeY;
                numChannels = 2;
                @display("p=220,70;i=misc/sun");
        }
        cni_os3: CNI_OS3 {
            @display("p=80,70,i;i=old/bluebox");
        }
        observer: Observer {
            numgps = 0;
            @display("p=220,200");
        }
    connections allowunconnected:
}
