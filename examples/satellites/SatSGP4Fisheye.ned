
package os3.examples.satellites;

import os3.base.Satellite;

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
                @display("p=73,74;r=10,,#707070;i=device/satellite_l");
        }

    connections allowunconnected:
}
