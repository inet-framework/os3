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

package cni_os3.mobility;

import inet.mobility.common.MobilityBase;

//
// Manages the SatSGP4 Mobility
//
// @author Marian P. Felder, Florian Schweikowski
// @version 0.1
// Module defined
//
simple SatSGP4Mobility extends MobilityBase
{
    parameters:
        double updateInterval @unit(s); // Time interval to update the hosts position
        @display("i=block/cogwheel_s");
        @class(SatSGP4Mobility);
}
