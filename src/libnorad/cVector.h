// cVector.h: interface for the cVector class.
//
// Copyright 2003 (c) Michael F. Henry
//
//////////////////////////////////////////////////////////////////////
#pragma once

class cVector  
{
public:
   cVector(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0) :
      m_x(x), m_y(y), m_z(z), m_w(w) {}
   virtual ~cVector() {};

   void Sub(const cVector&);     // subtraction
   void Mul(double factor);      // multiply each component by 'factor'

   double Angle(const cVector&) const;    // angle between two vectors
   double Magnitude() const;              // vector magnitude
   double Dot(const cVector& vec) const;  // dot product

// protected:
   double m_x;
   double m_y;
   double m_z;
   double m_w;
};