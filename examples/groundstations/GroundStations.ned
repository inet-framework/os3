
package os3.examples.groundstations;

import os3.base.MissionControlCenter;

network GroundStations
{
    parameters:
        int numOfMCCs;
        @display("bgi=background_earth;bgb=2160,1080");

    submodules:
        mcc[numOfMCCs]: MissionControlCenter {
            parameters:
                @display("p=240,150;i=misc/building;r=10,,black");
        }
    connections allowunconnected:
}
