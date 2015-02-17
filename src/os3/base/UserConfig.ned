
package os3.base;

//
// Interaction with user parameters set in omnetpp.ini and the omnet simulation. Also creates satellite mobility vectors.
//
simple UserConfig
{
    parameters:
        @class(UserConfig);
        @display("i=block/user");
        double min_snr    @unit(dBHz);  // minimal SNR to enable a transmission
        int numOfSats;                  // number of satellites
        double frequency  @unit(Hz);    // frequency of satellite communication antennas in Hertz
}
