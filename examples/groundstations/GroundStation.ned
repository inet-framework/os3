
package os3.examples.groundstations;

import os3.mobility.LUTMotionMobility;

module GroundStation
{
    parameters:
        string cityName = default("");
        @display("t=$cityName");
        @node;

    submodules:
        mobility: LUTMotionMobility {
            parameters:
                @display("p=38,36;i=block/cogwheel_s");
        }
    connections allowunconnected:

}
