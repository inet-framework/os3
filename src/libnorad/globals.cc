
#include "globals.h"

#include <cmath>

double sqr(const double x)
{
   return (x * x);
}

double Fmod2p(const double arg)
{
   double modu = std::fmod(arg, TWOPI);

   if (modu < 0.0)
      modu += TWOPI;

   return modu;
}

//-----------------------------------------------------
// AcTan()
// ArcTangent of sin(x) / cos(x). The advantage of this function over arctan()
// is that it returns the correct quadrant of the angle.
//-----------------------------------------------------
double AcTan(const double sinx, const double cosx)
{
   double ret;

   if (cosx == 0.0) {
      if (sinx > 0.0)
         ret = PI / 2.0;
      else
         ret = 3.0 * PI / 2.0;
   } else {
      if (cosx > 0.0)
         ret = atan(sinx / cosx);
      else
         ret = PI + atan(sinx / cosx);
   }

   return ret;
}

double rad2deg(const double r)
{
   const double DEG_PER_RAD = 180.0 / PI;
   return r * DEG_PER_RAD;
}

double deg2rad(const double d)
{
   const double RAD_PER_DEG = PI / 180.0;
   return d * RAD_PER_DEG;
}

