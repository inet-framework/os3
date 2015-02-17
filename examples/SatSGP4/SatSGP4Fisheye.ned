
package os3.examples.SatSGP4;

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


