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

#include "Calculation.h"

Define_Module(Calculation);

void Calculation::initialize() {
    userConfig = dynamic_cast< UserConfig* >(this->getParentModule()->getSubmodule("userConfig"));
    if (userConfig == NULL)
        error("Error in Calculation::initialize(): Could not find module 'UserConfig'.");

    webserviceControl = dynamic_cast< WebServiceControl* >(this->getParentModule()->getSubmodule("webServiceControl"));
    if (webserviceControl == NULL)
        error("Error in Calculation::initialize(): Could not find module 'WebserviceControl'.");

    weatherControl = dynamic_cast< WeatherControl* >(this->getParentModule()->getSubmodule("weatherControl"));
    if (weatherControl == NULL)
        error("Error in Calculation::initialize(): Could not find module 'WeatherControl'.");

    // Fill map with coefficients for specific rain attenuation
    fillRainMap();
}

void Calculation::fillRainMap() {
    std::string fileName = par("rainTableFile"); // source: 'The aR^b Relation in the Calculation of Rain Attenuation', Olsen and Rogers and Hodge
    std::fstream fileStream;

    fileStream.open(fileName.c_str(), std::ios::in);

    if (!fileStream.good()) {
        error("Error in Calculation::fillRainMap: File not found!");
    }

    // Extract data from CSV file
    while (fileStream.good()) {
        char tmpLine[100];
        fileStream.getline(tmpLine, 100);
        std::string tmpString = tmpLine;

        // Ignore lines beginning with "#"
        if (tmpString.find("#") == std::string::npos) {
            // Search for new values and parse them into rainCoeffMap
            rainCoefficients newCoeff;
            double newFreq = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.aLPl = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.aLPh = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.aMP = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.aJt = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.aJd = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.bLPl = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.bLPh = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.bMP = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.bJt = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            newCoeff.bJd = atof((tmpString.substr(0, tmpString.find(";"))).c_str());
            tmpString = tmpString.erase(0, tmpString.find(";") + 1);

            if (rainCoeffMap.find(newFreq) != rainCoeffMap.end()) {
                EV << "Warning in FillRainMap: Duplicate entries in file for frequency \"" << newFreq
                        << "\". Older entry is overwritten.";
                rainCoeffMap.erase(newFreq);
            }

            // Insert values into rainMap
            rainCoeffMap.insert(rainCoeffMap.end(), std::pair< double, rainCoefficients >(newFreq, newCoeff));
        }
    }

    fileStream.close();
}

void Calculation::handleMessage(cMessage *msg) {
    error("Error in Calculation::handleMessage(): This module is not able to handle messages.");
}

double Calculation::rad2deg(const double &deg) {
    return deg * 180 / PI;
}

double Calculation::deg2rad(const double &rad) {
    return rad * PI / 180;
}

double Calculation::calcFSL(const int &satIndex,     const double &lambda,  const double &latitude,
                            const double &longitude, const double &altitude) {
    SatSGP4Mobility* sat = userConfig->getSatMobility().at(satIndex);

    double alt;
    if (altitude == -9999)
        alt = webserviceControl->getAltitudeData(latitude, longitude);
    else
        alt = altitude;

    double distance = sat->getDistance(latitude, longitude, alt);
    double FSL = 20 * log10((4 * PI * distance) / (lambda / 1000)); // lambda in m <-> distance in km
    return FSL;
}

double Calculation::calcDistance(const double &latitude1, const double &longitude1,
                                 const double &latitude2, const double &longitude2) {
    return EarthRadius
            * (acos(
                    sin(deg2rad(latitude1)) * sin(deg2rad(latitude2))
                  + cos(deg2rad(latitude1)) * cos(deg2rad(latitude2)) * cos(deg2rad(longitude2 - longitude1))));
}

double Calculation::calcSNR(const double &transmitterGain, const double &receiverGain, const double &transmitterPower,
                            const double &lambda,          const int &satIndex,        const double &bandwidth,
                            const double &latitude,        const double &longitude,    const double &altitude,
                            const double &dG,              const double &tR,           const double &dR) {

    double tB = 2.725;  // Cosmic microwave background in K +- 0.002K
    double tM = 270;    // Approximated atmospheric noise temperature in K
    double t0 = 290;    // Average temperature of earth surface in K

    double frequency = C / lambda; // In Hz
    double alt;
    if (altitude == -9999)
        alt = webserviceControl->getAltitudeData(latitude, longitude);
    else
        alt = altitude;

    double elevation = userConfig->getSatMobility().at(satIndex)->getElevation(latitude, longitude, alt);

    // Map frequency on listed frequencies given in rainCoeffMap
    double newFreq = getMappedFrequency(frequency / 1e9); // Frequency in map in GHz

    // Get values from rainCoeffMap
    std::map< double, rainCoefficients >::iterator rainCoeffMapIt = rainCoeffMap.find(newFreq);

    if (rainCoeffMapIt == rainCoeffMap.end()) {
        error("Error in Calculation::calcSNR(): Frequency not found!");
        return -1;
    }

    double a = rainCoeffMapIt->second.aMP;    // Coefficient a from table, using Marshall-Palmer distribution
    double b = rainCoeffMapIt->second.bMP;    // Coefficient b from table, using Marshall-Palmer distribution
    double rp = weatherControl->getPrecipPerHour(latitude, longitude);  // Current weather Data

    double le = dR / sin(deg2rad(elevation));          // Length of signal path through rain
    double gammaR = a * pow(rp, b);           // Specific attenuation depending on frequency and rain density
    double aRain = gammaR * le;               // Attenuation depending on weather
    double aRain_lin = pow(10, (aRain / 10)); // Transform aRain from dB to linear unit
    double tANoise = tB / aRain_lin + tM * (1 - 1 / aRain_lin) + t0 * dG; // Noise temperature; source: 'Satellite Communications Systems', Maral et. Bousquet
    double tSysNoise = tANoise + tR;        // System noise temperature in K
    double tSdB = 10 * log10(tSysNoise);    // System noise temperature in dBK

    double snr =   transmitterPower
                 + transmitterGain
                 + receiverGain
                 - tSdB
                 - aRain
                 - calcFSL(satIndex, lambda, latitude, longitude, altitude)
                 - Boltzmann
                 - 10 * log10(bandwidth);

    return snr;
}

int Calculation::getScoredSatfromSNR(const double &latitude, const double &longitude, const double &transmitterGain,
                                     const double &receiverGain, const double &transmitterPower, const double &bandwidth,
                                     const double &altitude, const double &dG, const double &tR, const double &dR) {
    // Initialize parameters
    std::list< SAT > scoredSatList;
    std::vector< SatSGP4Mobility* > satmoVector = userConfig->getSatMobility();
    int sat_count = userConfig->getParameters().numOfSats;
    double lambda = C / userConfig->getParameters().frequency;

    for (int index = 0; index < sat_count; index++) {

        double snr = calcSNR(transmitterGain, receiverGain, transmitterPower, lambda, index,
                                        bandwidth, latitude, longitude, altitude, dG, tR, dR);


        if (snr >= userConfig->getParameters().min_snr) {
            struct SAT s;
            s.satIndex = index;
            s.snr = snr;
            if (scoredSatList.empty() == true)
                scoredSatList.push_back(s);
            else {
                std::list< struct SAT >::iterator it = scoredSatList.begin();

                // Insert satellite into list (sorted by SNR)
                while ((it != scoredSatList.end()) && (it->snr > snr)) {
                    it++;
                }
                scoredSatList.insert(it, s);
            }
        }
    }

    if (scoredSatList.size() > 0) {
        std::list< struct SAT >::const_iterator it = scoredSatList.begin();

        if (it->snr < 0) {
            bubble("Suboptimal satellite chosen.");
        }

        return it->satIndex;
    }
    return -1;
}

double Calculation::getMappedFrequency(const double &frequency) {
    double mindist = -1;
    double freq;

    // Check if frequency already exists in map
    if (rainCoeffMap.find(frequency) != rainCoeffMap.end()) {
        return frequency;
    }
    else {
        std::map< double, rainCoefficients >::iterator rainCoeffMapIt;
        for (rainCoeffMapIt = rainCoeffMap.begin(); rainCoeffMapIt != rainCoeffMap.end(); rainCoeffMapIt++) {
            double dist = abs(frequency - rainCoeffMapIt->first); // Calculate euclidean distance
            if (dist < mindist || mindist == -1) {
                mindist = dist;
                freq = rainCoeffMapIt->first;
            }
        }

        return freq;
    }
}
