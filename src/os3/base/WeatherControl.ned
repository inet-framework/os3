
package os3.base;

//
// Gets weather information from WebService and integrates them in the simulation.
//
simple WeatherControl
{
    parameters:
        @display("i=misc/sun"); // Symbol, depending on actual weather situation
        double defaultPrecipPerHour = default(-1); // Default precipitation (=rainfall) per hour value. If set to -1, then system is using live weather data.
}
