//-----------------------------------------------------
// cTle.cpp
// This class encapsulates a single set of standard NORAD two line elements.
//
// Copyright 1996-2005 Michael F. Henry
//-----------------------------------------------------

#include "libnorad/cTLE.h"

#include <cstdio>
#include <cassert>

// Note: The column offsets are ZERO based.

// Name
const unsigned int TLE_LEN_LINE_DATA      = 69;
//const int TLE_LEN_LINE_NAME      = 22;

// Line 1
const int TLE1_COL_SATNUM        =  2; const int TLE1_LEN_SATNUM        =  5;
const int TLE1_COL_INTLDESC_A    =  9; const int TLE1_LEN_INTLDESC_A    =  2;
//const int TLE1_COL_INTLDESC_B    = 11;
const int TLE1_LEN_INTLDESC_B    =  3;
//const int TLE1_COL_INTLDESC_C    = 14;
const int TLE1_LEN_INTLDESC_C    =  3;
const int TLE1_COL_EPOCH_A       = 18; const int TLE1_LEN_EPOCH_A       =  2;
const int TLE1_COL_EPOCH_B       = 20; const int TLE1_LEN_EPOCH_B       = 12;
const int TLE1_COL_MEANMOTIONDT  = 33; const int TLE1_LEN_MEANMOTIONDT  = 10;
const int TLE1_COL_MEANMOTIONDT2 = 44; const int TLE1_LEN_MEANMOTIONDT2 =  8;
const int TLE1_COL_BSTAR         = 53; const int TLE1_LEN_BSTAR         =  8;
//const int TLE1_COL_EPHEMTYPE     = 62; const int TLE1_LEN_EPHEMTYPE     =  1;
const int TLE1_COL_ELNUM         = 64; const int TLE1_LEN_ELNUM         =  4;

// Line 2
//const int TLE2_COL_SATNUM        = 2;  const int TLE2_LEN_SATNUM        =  5;
const int TLE2_COL_INCLINATION   = 8;  const int TLE2_LEN_INCLINATION   =  8;
const int TLE2_COL_RAASCENDNODE  = 17; const int TLE2_LEN_RAASCENDNODE  =  8;
const int TLE2_COL_ECCENTRICITY  = 26; const int TLE2_LEN_ECCENTRICITY  =  7;
const int TLE2_COL_ARGPERIGEE    = 34; const int TLE2_LEN_ARGPERIGEE    =  8;
const int TLE2_COL_MEANANOMALY   = 43; const int TLE2_LEN_MEANANOMALY   =  8;
const int TLE2_COL_MEANMOTION    = 52; const int TLE2_LEN_MEANMOTION    = 11;
const int TLE2_COL_REVATEPOCH    = 63; const int TLE2_LEN_REVATEPOCH    =  5;

cTle::cTle(std::string& strName, std::string& strLine1, std::string& strLine2)
{
   m_strName  = strName;
   m_strLine1 = strLine1;
   m_strLine2 = strLine2;

   Initialize();
}

cTle::cTle(const cTle& tle)
{
   m_strName  = tle.m_strName;
   m_strLine1 = tle.m_strLine1;
   m_strLine2 = tle.m_strLine2;

   for (int fld = FLD_FIRST; fld < FLD_LAST; fld++)
   {
      m_Field[fld] = tle.m_Field[fld];
   }

   m_mapCache = tle.m_mapCache;
}

cTle::~cTle()
{}

//-----------------------------------------------------
// getField()
// Return requested field as a double (function return value) or as a text
// std::string (*pstr) in the units requested (eUnit). Set 'bStrUnits' to true
// to have units appended to text std::string.
//
// Note: numeric return values are cached; asking for the same field more
// than once incurs minimal overhead.
//-----------------------------------------------------
double cTle::getField(eField   fld,
                      eUnits   units,    /* = U_NATIVE */
                      std::string  *pstr      /* = NULL     */,
                      bool     bStrUnits /* = false    */) const
{
   assert((FLD_FIRST <= fld) && (fld < FLD_LAST));
   assert((U_FIRST <= units) && (units < U_LAST));

   if (pstr) {
      // Return requested field in std::string form.
      *pstr = m_Field[fld];

      if (bStrUnits)
         *pstr += getUnits(fld);

      return 0.0;
   } else {
      // Return requested field in floating-point form.
      // Return cache contents if it exists, else populate cache
      FldKey key = Key(units, fld);

      if (m_mapCache.find(key) == m_mapCache.end()) {
         // Value not in cache; add it
         double valNative = atof(m_Field[fld].c_str());
         double valConv   = ConvertUnits(valNative, fld, units);
         m_mapCache[key]  = valConv;

         return valConv;
      } else {
         // return cached value
         return m_mapCache[key];
      }
   }
}

//-----------------------------------------------------
// Convert the given field into the requested units. It is assumed that
// the value being converted is in the TLE format's "native" form.
//-----------------------------------------------------
double cTle::ConvertUnits(double valNative, // value to convert
                          eField fld,       // what field the value is
                          eUnits units)     // what units to convert to
{
   switch (fld)
   {
      case FLD_I:
      case FLD_RAAN:
      case FLD_ARGPER:
      case FLD_M:
      {
         // The native TLE format is DEGREES
         if (units == U_RAD)
            return valNative * RADS_PER_DEG;
         break;
      }
      //FIXED: added default case!
      default:
          break;
   }

   return valNative; // return value in unconverted native format
}

std::string cTle::getUnits(eField fld) const
{
   static const std::string strDegrees    = " degrees";
   static const std::string strRevsPerDay = " revs / day";
   static const std::string strNull;

   switch (fld)
   {
      case FLD_I:
      case FLD_RAAN:
      case FLD_ARGPER:
      case FLD_M:
         return strDegrees;

      case FLD_MMOTION:
         return strRevsPerDay;

      default:
         return strNull;
   }
}

//-----------------------------------------------------
// ExpToDecimal()
// Converts TLE-style exponential notation of the form [ |-]00000[+|-]0 to
// decimal notation. Assumes implied decimal point to the left of the first
// number in the std::string, i.e.,
//       " 12345-3" =  0.00012345
//       "-23429-5" = -0.0000023429
//       " 40436+1" =  4.0436
//-----------------------------------------------------
std::string cTle::ExpToDecimal(const std::string& str)
{
   const int COL_EXP_SIGN = 6;
   const int LEN_EXP      = 2;

   const int LEN_BUFREAL  = 32;   // max length of buffer to hold floating point
                                 // representation of input std::string.
   int nMan;
   int nExp;

   // sscanf(%d) will read up to the exponent sign
   std::sscanf(str.c_str(), "%d", &nMan);

   double dblMan = nMan;
   bool  bNeg    = (nMan < 0);

   if (bNeg)
      dblMan *= -1;

   // Move decimal place to left of first digit
   while (dblMan >= 1.0)
      dblMan /= 10.0;

   if (bNeg)
      dblMan *= -1;

   // now read exponent
   std::sscanf(str.substr(COL_EXP_SIGN, LEN_EXP).c_str(), "%d", &nExp);

   double dblVal = dblMan * pow(10.0, nExp);
   char szVal[LEN_BUFREAL];

   //_snprintf(szVal, sizeof(szVal), "%.9f", dblVal);
   std::sprintf(szVal,"%.9f", dblVal);

   std::string strVal = szVal;

   return strVal;

} // ExpToDecimal()

//-----------------------------------------------------
// Initialize()
// Initialize the std::string array.
//-----------------------------------------------------
void cTle::Initialize()
{
   // Have we already been initialized?
   if (m_Field[FLD_NORADNUM].size())
      return;

   assert(!m_strName.empty());
   assert(!m_strLine1.empty());
   assert(!m_strLine2.empty());

   m_Field[FLD_NORADNUM] = m_strLine1.substr(TLE1_COL_SATNUM, TLE1_LEN_SATNUM);
   m_Field[FLD_INTLDESC] = m_strLine1.substr(TLE1_COL_INTLDESC_A,
                                             TLE1_LEN_INTLDESC_A +
                                             TLE1_LEN_INTLDESC_B +
                                             TLE1_LEN_INTLDESC_C);
   m_Field[FLD_EPOCHYEAR] =
         m_strLine1.substr(TLE1_COL_EPOCH_A, TLE1_LEN_EPOCH_A);

   m_Field[FLD_EPOCHDAY] =
         m_strLine1.substr(TLE1_COL_EPOCH_B, TLE1_LEN_EPOCH_B);

   if (m_strLine1[TLE1_COL_MEANMOTIONDT] == '-') {
      // value is negative
      m_Field[FLD_MMOTIONDT] = "-0";
   } else {
      m_Field[FLD_MMOTIONDT] = "0";
   }
   m_Field[FLD_MMOTIONDT] += m_strLine1.substr(TLE1_COL_MEANMOTIONDT + 1,
                                               TLE1_LEN_MEANMOTIONDT);

   // decimal point assumed; exponential notation
   m_Field[FLD_MMOTIONDT2] = ExpToDecimal(
                                 m_strLine1.substr(TLE1_COL_MEANMOTIONDT2,
                                                   TLE1_LEN_MEANMOTIONDT2));
   // decimal point assumed; exponential notation
   m_Field[FLD_BSTAR] = ExpToDecimal(m_strLine1.substr(TLE1_COL_BSTAR,
                                                       TLE1_LEN_BSTAR));
   //TLE1_COL_EPHEMTYPE
   //TLE1_LEN_EPHEMTYPE
   m_Field[FLD_SET] = m_strLine1.substr(TLE1_COL_ELNUM, TLE1_LEN_ELNUM);

   TrimLeft(m_Field[FLD_SET]);

   //TLE2_COL_SATNUM
   //TLE2_LEN_SATNUM

   m_Field[FLD_I] = m_strLine2.substr(TLE2_COL_INCLINATION,
                                      TLE2_LEN_INCLINATION);
   TrimLeft(m_Field[FLD_I]);

   m_Field[FLD_RAAN] = m_strLine2.substr(TLE2_COL_RAASCENDNODE,
                                         TLE2_LEN_RAASCENDNODE);
   TrimLeft(m_Field[FLD_RAAN]);

   // decimal point is assumed
   m_Field[FLD_E]   = "0.";
   m_Field[FLD_E]   += m_strLine2.substr(TLE2_COL_ECCENTRICITY,
                                       TLE2_LEN_ECCENTRICITY);

   m_Field[FLD_ARGPER] = m_strLine2.substr(TLE2_COL_ARGPERIGEE,
                                           TLE2_LEN_ARGPERIGEE);
   TrimLeft(m_Field[FLD_ARGPER]);

   m_Field[FLD_M]   = m_strLine2.substr(TLE2_COL_MEANANOMALY,
                                      TLE2_LEN_MEANANOMALY);
   TrimLeft(m_Field[FLD_M]);

   m_Field[FLD_MMOTION]   = m_strLine2.substr(TLE2_COL_MEANMOTION,
                                            TLE2_LEN_MEANMOTION);
   TrimLeft(m_Field[FLD_MMOTION]);

   m_Field[FLD_ORBITNUM] = m_strLine2.substr(TLE2_COL_REVATEPOCH,
                                             TLE2_LEN_REVATEPOCH);
   TrimLeft(m_Field[FLD_ORBITNUM]);

}

//-----------------------------------------------------
// IsTleFormat()
// Returns true if "str" is a valid data line of a two-line element set,
//   else false.
//
// To be valid a line must:
//      Have as the first character the line number
//      Have as the second character a blank
//      Be TLE_LEN_LINE_DATA characters long
//      Have a valid checksum (note: no longer required as of 12/96)
//-----------------------------------------------------
bool cTle::IsValidLine(std::string& str, eTleLine line)
{
   TrimLeft(str);
   TrimRight(str);

   size_t nLen = str.size();

   if (nLen != TLE_LEN_LINE_DATA)
      return false;

   // First char in std::string must be line number
   if ((str[0] - '0') != line)
      return false;

   // Second char in std::string must be blank
   if (str[1] != ' ')
      return false;

   /*
      NOTE: 12/96
      The requirement that the last char in the line data must be a valid
      checksum is too restrictive.

      // Last char in std::string must be checksum
      int nSum = CheckSum(str);

      if (nSum != (str[TLE_LEN_LINE_DATA - 1] - '0'))
         return false;
   */

   return true;

}

//-----------------------------------------------------
// CheckSum()
// Calculate the check sum for a given line of TLE data, the last character
// of which is the current checksum. (Although there is no check here,
// the current checksum should match the one we calculate.)
// The checksum algorithm:
//      Each number in the data line is summed, modulo 10.
//    Non-numeric characters are zero, except minus signs, which are 1.
//-----------------------------------------------------
int cTle::CheckSum(const std::string& str)
{
   // The length is "- 1" because we don't include the current (existing)
   // checksum character in the checksum calculation.
   size_t len = str.size() - 1;
   int xsum = 0;

   for (size_t i = 0; i < len; i++)
   {
      char ch = str[i];
      if (isdigit(ch))
         xsum += (ch - '0');
      else if (ch == '-')
         xsum++;
   }

   return (xsum % 10);

}

void cTle::TrimLeft(std::string& s)
{
   while (s[0] == ' ')
      s.erase(0, 1);
}

void cTle::TrimRight(std::string& s)
{
   while (s[s.size() - 1] == ' ')
      s.erase(s.size() - 1);
}

