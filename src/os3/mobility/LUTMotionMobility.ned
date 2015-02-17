
package os3.mobility;

import inet.mobility.common.MovingMobilityBase;
import inet.mobility.contract.IMobility;

//
// Initializes the initial positions of Base Stations (LUT = Local User Terminal)
//
simple LUTMotionMobility extends MovingMobilityBase
{
    parameters:
        double latitude;
        double longitude;
        //double updateInterval @unit(s); // Time interval to update the hosts position
        @class(LUTMotionMobility);
}
