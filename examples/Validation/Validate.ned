
package os3.examples.Validation;

import os3.base.Satellite;
import inet.world.radio.ChannelControl;
import os3.base.CNI_OS3;
import os3.base.Observer;

//
// Network Validate used to show Observer functionalities
//
network Validate
{
    parameters:
        int numOfSats;

        @display("bgi=background_earth");

    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=80,200;r=10,,#707070;i=device/satellite_l");
        }
        channelcontrol: ChannelControl {
            parameters:
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
