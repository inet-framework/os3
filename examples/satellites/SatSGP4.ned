
package os3.examples.satellites;

import os3.base.Satellite;
import os3.base.MissionControlCenter;

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
                @display("p=96,92;r=10,,#707070;i=device/satellite_l");
        }
        MCC[numOfMCCs]: MissionControlCenter {
            parameters:
                @display("p=232,92;i=misc/building;r=10,,black");
        }
    connections allowunconnected:
}

