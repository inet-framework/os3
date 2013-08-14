// Copyright Andreas Lewandowski / Brian Niehoefer

package cni_os3.simulations;

import cni_os3.base.Satellite;
import cni_os3.base.MissionControlCenter;
import inet.world.ChannelControl;
import cni_os3.base.CNI_OS3;

network SatSGP4
{
    parameters:
        double playgroundSizeX;
        double playgroundSizeY;
        int numOfSats;
        int numOfMCCs;

		//Sarsat
        @display("bgi=background_earth");

    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=20,150;r=10,,#707070;i=device/satellite_l");
        }
        MCC[numOfMCCs]: MissionControlCenter {
            parameters:
                @display("p=180,150;i=misc/building");
        }
        channelcontrol: ChannelControl {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                numChannels = 2;
                @display("p=180,70;i=misc/sun");
        }
        cni_os3: CNI_OS3 {

            @display("p=20,70,i;i=old/bluebox");
        }
    connections allowunconnected:

}


