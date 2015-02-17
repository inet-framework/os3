
package os3.base;
import os3.mobility.LUTMotionMobility;
import inet.base.NotificationBoard;

//
// Bundles the analyzation and notification modules for the OS³ missions.
//
module MissionControlCenter
{
    parameters:
        string cityName = default("");
        @display("t=$cityName");
        @node;

    gates:
        input radioIn; // Input gate
        output radioOut; // Output gate

    submodules:
        mobility: LUTMotionMobility { // Module for mobility (LUTMotionMobility)
            parameters:
                @display("p=54,256;i=block/cogwheel_s");
        }
        notificationBoard: NotificationBoard { // Module for Notfication on event occuring
            parameters:
                @display("p=182,248;i=block/control");
        }
    connections allowunconnected: // Connections between compound gates and submodules

}
