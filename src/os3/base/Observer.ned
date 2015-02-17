
package os3.base;

//
// Observer module
//
simple Observer
{
parameters:
    @display("i=block/user");
    double ObserverLongitude;       // Observer longitude
    double ObserverLatitude;        // Observer latitude
    double ObserverAltitude;        // Observer altitude
    double TimerInterval @unit(s);  // Update interval for timer
    int numgps;                     // Number of GPS satellites used
    bool gps;                       // Check of GPS validation or ISS validation should run
}
