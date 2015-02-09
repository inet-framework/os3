//
// cEci.cpp
//
// Copyright (c) 2002-2003 Michael F. Henry
//

#include "libnorad/cEci.h"

#include <cmath>

cEci::cEci(const cVector& pos,
           const cVector& vel,
           const cJulian& date,
           bool  IsAeUnits /* = true */)
{
   m_pos      = pos;
   m_vel      = vel;
   m_date     = date;
   m_VecUnits = (IsAeUnits ? UNITS_AE : UNITS_NONE);
}

//////////////////////////////////////////////////////////////////////
// cEci(cCoordGeo&, cJulian&)
// Calculate the ECI coordinates of the location "geo" at time "date".
// Assumes geo coordinates are km-based.
// Assumes the earth is an oblate spheroid as defined in WGS '72.
// Reference: The 1992 Astronomical Almanac, page K11
// Reference: www.celestrak.com (Dr. TS Kelso)
cEci::cEci(const cCoordGeo& geo, const cJulian& date)
{
   m_VecUnits = UNITS_KM;

   double mfactor = TWOPI * (OMEGA_E / SEC_PER_DAY);
   double lat = geo.m_Lat;
   double lon = geo.m_Lon;
   double alt = geo.m_Alt;

   // Calculate Local Mean Sidereal Time (theta)
   double theta = date.toLMST(lon);
   double c = 1.0 / std::sqrt(1.0 + F * (F - 2.0) * sqr(std::sin(lat)));
   double s = sqr(1.0 - F) * c;
   double achcp = (XKMPER_WGS72 * c + alt) * std::cos(lat);

   m_date = date;

   m_pos.m_x = achcp * std::cos(theta);                    // km
   m_pos.m_y = achcp * std::sin(theta);                    // km
   m_pos.m_z = (XKMPER_WGS72 * s + alt) * std::sin(lat);   // km
   m_pos.m_w = std::sqrt(sqr(m_pos.m_x) +
                         sqr(m_pos.m_y) +
                         sqr(m_pos.m_z));            // range, km

   m_vel.m_x = -mfactor * m_pos.m_y;                 // km / sec
   m_vel.m_y =  mfactor * m_pos.m_x;
   m_vel.m_z = 0.0;
   m_vel.m_w = std::sqrt(sqr(m_vel.m_x) +            // range rate km/sec^2
                         sqr(m_vel.m_y));
}

//////////////////////////////////////////////////////////////////////////////
// toGeo()
// Return the corresponding geodetic position (based on the current ECI
// coordinates/Julian date).
// Assumes the earth is an oblate spheroid as defined in WGS '72.
// Side effects: Converts the position and velocity vectors to km-based units.
// Reference: The 1992 Astronomical Almanac, page K12.
// Reference: www.celestrak.com (Dr. TS Kelso)
cCoordGeo cEci::toGeo()
{
   ae2km(); // Vectors must be in kilometer-based units

   double theta = AcTan(m_pos.m_y, m_pos.m_x);
   double lon   = std::fmod(theta - m_date.toGMST(), TWOPI);

   if (lon < 0.0)
      lon += TWOPI;  // "wrap" negative modulo

   double r   = std::sqrt(sqr(m_pos.m_x) + sqr(m_pos.m_y));
   double e2  = F * (2.0 - F);
   double lat = AcTan(m_pos.m_z, r);

   const double delta = 1.0e-07;
   double phi;
   double c;

   do {
      phi = lat;
      c   = 1.0 / std::sqrt(1.0 - e2 * sqr(std::sin(phi)));
      lat = AcTan(m_pos.m_z + XKMPER_WGS72 * c * e2 * std::sin(phi), r);
   } while (std::fabs(lat - phi) > delta);

   const double alt = r / std::cos(lat) - XKMPER_WGS72 * c;

   return cCoordGeo(lat, lon, alt); // radians, radians, kilometers
}

//////////////////////////////////////////////////////////////////////////////
// ae2km()
// Convert the position and velocity vector units from AE-based units
// to kilometer based units.
void cEci::ae2km()
{
   if (UnitsAreAe()) {
      MulPos(XKMPER_WGS72 / AE);                            // km
      MulVel((XKMPER_WGS72 / AE) * (MIN_PER_DAY / 86400));  // km/sec
      m_VecUnits = UNITS_KM;
   }
}
