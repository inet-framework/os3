//-----------------------------------------------------
// cEci.h
//
// Copyright (c) 2003 Michael F. Henry
//-----------------------------------------------------
#ifndef __LIBNORAD_cEci_H__
#define __LIBNORAD_cEci_H__

#include "ccoord.h"
#include "cVector.h"
#include "cJulian.h"

//-----------------------------------------------------
// Class: cEci
// Description: Encapsulates an Earth-Centered Inertial
//              position, velocity, and time.
//-----------------------------------------------------
class cEci
{
public:
   cEci() { m_VecUnits = UNITS_NONE; }
   cEci(const cCoordGeo& geo, const cJulian& cJulian);
   cEci(const cVector& pos, const cVector& vel,
        const cJulian& date, bool IsAeUnits = true);
   virtual ~cEci() {};

   cCoordGeo toGeo();

   cVector getPos()  const             { return m_pos;  }
   cVector getVel()  const             { return m_vel;  }
   cJulian getDate() const             { return m_date; }

   void setUnitsAe()                   { m_VecUnits = UNITS_AE; }
   void setUnitsKm()                   { m_VecUnits = UNITS_KM; }
   bool UnitsAreAe() const             { return m_VecUnits == UNITS_AE; }
   bool UnitsAreKm() const             { return m_VecUnits == UNITS_KM; }
   void ae2km();  // Convert position, velocity vector units from AE to km

protected:
   void MulPos(double factor)          { m_pos.Mul(factor); }
   void MulVel(double factor)          { m_vel.Mul(factor); }

   enum VecUnits
   {
      UNITS_NONE, // not initialized
      UNITS_AE,
      UNITS_KM,
   };

   cVector  m_pos;
   cVector  m_vel;
   cJulian  m_date;
   VecUnits m_VecUnits;
};

#endif
