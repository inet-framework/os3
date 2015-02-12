//
// coord.cpp
//
// Copyright (c) 2003 Michael F. Henry
//
#include "ccoord.h"

cCoordGeo::cCoordGeo()
{
   m_Lat = 0.0;
   m_Lon = 0.0;
   m_Alt = 0.0;
}

cCoordTopo::cCoordTopo()
{
   m_Az = 0.0;
   m_El = 0.0;
   m_Range = 0.0;
   m_RangeRate = 0.0;
}
