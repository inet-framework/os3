//
// cSite.h: interface for the cSite class.
//
// Copyright 2002-2003 Michael F. Henry
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "ccoord.h"
#include "cEci.h"

//////////////////////////////////////////////////////////////////////
// class cSite
// This class represents a location (ground site) on the earth.
class cSite  
{
public:
   cSite(double degLat, double degLon, double kmAlt);
   cSite(const cCoordGeo &geo);
   virtual ~cSite();

   void      setGeo(const cCoordGeo &geo);       // Set new coordinates
   cCoordGeo getGeo() const { return m_geo; }

   cEci       getPosition(const cJulian&) const; // calc ECI of geo location.
   cCoordTopo getLookAngle(const cEci&)   const; // calc topo coords to ECI object

   double getLat() const { return m_geo.m_Lat; }
   double getLon() const { return m_geo.m_Lon; }
   double getAlt() const { return m_geo.m_Alt; }

   std::string toString() const;

protected:
   cCoordGeo m_geo;  // lat, lon, alt of earth site

};
