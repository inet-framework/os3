
package os3.mobility;

import inet.mobility.contract.IMobility;
import inet.mobility.common.MobilityBase;

//
// Manages the SatSGP4Fisheye Mobility
//
simple SatSGP4FisheyeMobility extends MobilityBase
{
    parameters:
        @class(SatSGP4FisheyeMobility);
        @display("i=block/cogwheel_s");
        double refCenterLatitude;        // Coordinate for the center point
        double refCenterLongitude;       // Coordinate for the center point
        double refCenterAltitude;        // Coordinate for the center point
        double updateInterval @unit(s);  // Time interval to update the hosts position
}
