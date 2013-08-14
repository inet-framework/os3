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


network GPS
{
    parameters:
        double playgroundSizeX; // Width of map in pixels
        double playgroundSizeY; // Height of map in pixels
        int numOfSats; // Number of satellites

        @display("bgi=binocular,c;bgb=980,980");

    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=70,180;r=10,,#707070;i=device/satellite_l");
        }

        channelcontrol: ChannelControl {
            parameters:
                //playgroundSizeX = playgroundSizeX;
                //playgroundSizeY = playgroundSizeY;
                numChannels = 2;
                @display("p=180,70;i=misc/sun");
        }
        cni_os3: CNI_OS3 {
            @display("p=50,70,i;i=old/bluebox");
        }
        observer: Observer {
            @display("p=490,490");
            numgps = numOfSats;
        }
    connections allowunconnected:


}

