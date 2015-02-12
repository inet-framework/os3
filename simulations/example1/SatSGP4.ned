
package os3.simulations.example1;

import os3.base.Satellite;
import os3.base.MissionControlCenter;
import inet.world.radio.ChannelControl;
import os3.base.CNI_OS3;
//
// Network SatSGP4 enables satellite movement on worldmap
//
network SatSGP4
{
    parameters:
        int numOfSats; // Number of satellites
        int numOfMCCs; // Number of Mission Control Centers

        @display("bgi=background_earth;bgb=2160,1080");

    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=80,150;r=10,,#707070;i=device/satellite_l");
        }
        MCC[numOfMCCs]: MissionControlCenter {
            parameters:
                @display("p=240,150;i=misc/building;r=10,,black");
        }
        channelcontrol: ChannelControl {
            parameters:
                //playgroundSizeX = playgroundSizeX;
                //playgroundSizeY = playgroundSizeY; note: https://groups.google.com/forum/#!msg/omnetpp/iofWt9w-o7A/bRuaO3KrMfcJ
                numChannels = 2;
                @display("p=240,70;i=misc/sun");
        }
        cni_os3: CNI_OS3 {
            @display("p=80,70,i;i=old/bluebox");
        }
    connections allowunconnected:
}


