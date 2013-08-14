// Copyright Andreas Lewandowski / Brian Niehoefer

package cni_os3.simulations;

import cni_os3.base.Satellite;
import inet.world.ChannelControl;
import cni_os3.base.CNI_OS3;


network SatSGP4Fisheye
{
    parameters:
        double playgroundSizeX;
        double playgroundSizeY;
        int numOfSats;
		//int num_MCC; 

        @display("bgi=binocular;bgb=980,980");
   
    submodules:
        satellite[numOfSats]: Satellite {
            parameters:
                @display("p=70,180;r=10,,#707070;i=device/satellite_l");
        }
//Fixme: wird das hier benötigt oder nicht? bzw, in wie fern?
       // MCC[num_MCC]: MissionControlCenter {
        //    parameters:
         //       @display("p=80,34;i=misc/building");
        //} 
        channelcontrol: ChannelControl {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                numChannels = 2;
                @display("p=180,70;i=misc/sun");
        }
        cni_os3:CNI_OS3{
              @display("p=20,70,i;i=old/bluebox");
        }
    connections allowunconnected:


}


