//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package cni_os3.base;

import cni_os3.base.Calculation;
import inet.mobility.ConstSpeedMobility;


//
// Bundles the control modules for the OS³ satellite simulator.
// ATTENTION: This version only works with INET version 20111118!!! Porting to INET 2.0 is scheduled for November 2012.
//
// @author Sarah Lehnhausen, Dennis Kaulbars
// @version 0.1
// Module defined
//
module CNI_OS3
{
    submodules:
        userConfig: UserConfig { // Module for user parameters
            @display("p=80,40");
        }
        webServiceControl: WebServiceControl {
            @display("p=80,180");
        }
        weatherControl: WeatherControl { // Module for fetching and handling of weather information
            @display("p=310,40");
        }
        calculation: Calculation { // Module for calculation
            @display("p=310,180");
        }
}
