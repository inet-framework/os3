
package os3.mobility;

import inet.mobility.common.MobilityBase;

//
// Manages the SatSGP4 Mobility
//
simple SatSGP4Mobility extends MobilityBase
{
    parameters:
        @class(SatSGP4Mobility);
        @display("i=block/cogwheel_s");
        double updateInterval @unit(s); // Time interval to update the hosts position
}
