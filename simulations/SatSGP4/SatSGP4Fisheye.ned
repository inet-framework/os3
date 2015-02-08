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

// Copyright Andreas Lewandowski / Brian Niehoefer

package os3.simulations.SatSGP4;

import os3.base.Satellite;
import inet.world.radio.ChannelControl;
import os3.base.CNI_OS3;

//
// Network SatSGP4Fisheye enables satellite movement in fisheye view
//
network SatSGP4Fisheye
{
    parameters:
        int numOfSats; // Number of satellites

        @display("bgi=binocular,c;bgb=980,980");

    submodules:
        satellite[numOfSats]: Satellite { 
            parameters:
                @display("p=70,180;r=10,,#707070;i=device/satellite_l");
        }

        channelcontrol: ChannelControl {
            parameters:
                numChannels = 2;
                @display("p=180,70;i=misc/sun");
        }
        cni_os3: CNI_OS3 {
            @display("p=50,70,i;i=old/bluebox");
        }
    connections allowunconnected:
}


