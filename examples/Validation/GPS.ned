
package os3.examples.Validation;

import os3.base.Satellite;
import inet.world.radio.ChannelControl;
import os3.base.CNI_OS3;
import os3.base.Observer;

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

