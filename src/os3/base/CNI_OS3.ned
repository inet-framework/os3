
package os3.base;

import os3.base.UserConfig;
import os3.base.WebServiceControl;
import os3.base.WeatherControl;
import os3.base.Calculation;

//
// Bundles the control modules for the OS³ satellite simulator.
// ATTENTION: This version only works with INET version 20111118!!! Porting to INET 2.0 is scheduled for November 2012.
//
module CNI_OS3
{
    submodules:
        userConfig: UserConfig {          // Module for user parameters
            @display("p=80,40");
        }
        webServiceControl: WebServiceControl {
            @display("p=80,180");
        }
        weatherControl: WeatherControl {  // Module for fetching and handling of weather information
            @display("p=310,40");
        }
        calculation: Calculation {        // Module for calculation
            @display("p=310,180");
        }
}
