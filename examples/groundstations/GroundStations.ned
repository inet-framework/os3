
package os3.examples.groundstations;

import os3.examples.groundstations.GroundStation;


network GroundStations
{
    parameters:
        int numOfMCCs; // Number of Mission Control Centers
        @display("bgi=background_earth;bgb=2160,1080");

    submodules:
        mcc[numOfGndStations]: GroundStation {
            parameters:
                @display("p=112,104;i=misc/building;r=10,,black");
        }
    connections allowunconnected:
}
