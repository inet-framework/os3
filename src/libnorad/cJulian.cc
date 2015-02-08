//
// cJulian.cpp
//
// This class encapsulates Julian dates with the epoch of 12:00 noon (12:00 UT)
// on January 1, 4713 B.C. Some epoch dates:
//    01/01/1990 00:00 UTC - 2447892.5
//    01/01/1990 12:00 UTC - 2447893.0
//    01/01/2000 00:00 UTC - 2451544.5
//    01/01/2001 00:00 UTC - 2451910.5
//
// Note the Julian day begins at noon, which allows astronomers to have all
// the dates in a single observing session the same.
//
// References:
// "Astronomical Formulae for Calculators", Jean Meeus
// "Satellite Communications", Dennis Roddy, 2nd Edition, 1995.
//
// Copyright (c) 2003 Michael F. Henry
//
// mfh 12/24/2003
//

#include "cJulian.h"

#include <cassert>
#include <cstring>
#include <ctime>
#include <cmath>

cJulian::cJulian()
{
    initialize(2000, 1.0);
}

//-----------------------------------------------------
// Create a Julian date object from a time_t object. time_t objects store the
// number of seconds since midnight UTC January 1, 1970.
//-----------------------------------------------------
cJulian::cJulian(std::time_t time)
{
    struct tm* ptm = gmtime(&time);
    assert(ptm);

    const int year = ptm->tm_year + 1900;
    const double day = ptm->tm_yday + 1
            + (ptm->tm_hour + ((ptm->tm_min + (ptm->tm_sec / 60.0)) / 60.0))
                    / 24.0;

    initialize(year, day);
}

//-----------------------------------------------------
// Create a Julian date object from a year and day of year.
// Example parameters: year = 2001, day = 1.5 (Jan 1 12h)
//-----------------------------------------------------
cJulian::cJulian(int year, double day)
{
    initialize(year, day);
}

//-----------------------------------------------------
// Create a Julian date obj A(47);ect.
//-----------------------------------------------------
cJulian::cJulian(int year, // i.e., 2004
        int mon, // 1..12
        int day, // 1..31
        int hour, // 0..23
        int min, // 0..59
        double sec /* = 0.0 */) // 0..(59.999999...)
{
    // Calculate N, the day of the year (1..366)
    int N;
    const int F1 = static_cast<int>((275.0 * mon) / 9.0);
    const int F2 = static_cast<int>((mon + 9.0) / 12.0);

    if (IsLeapYear(year)) {
        // Leap year
        N = F1 - F2 + day - 30;
    } else {
        // Common year
        N = F1 - (2 * F2) + day - 30;
    }

    const double dblDay = N + (hour + (min + (sec / 60.0)) / 60.0) / 24.0;

    initialize(year, dblDay);
}

void cJulian::initialize(int year, double day)
{
    // 1582 A.D.: 10 days removed from calendar
    // 3000 A.D.: Arbitrary error checking limit
    assert((year > 1582) && (year < 3000));
    assert((day >= 0.0) && (day <= 366.5));

    // Now calculate Julian date

    year--;

    // Centuries are not leap years unless they divide by 400
    const int A = (year / 100);
    const int B = 2 - A + (A / 4);

    // 1720994.5 = Oct 30, year -1
    const double NewYears = static_cast<int>(365.25 * year)
                          + static_cast<int>(30.6001 * 14) + 1720994.5 + B;

    m_Date = NewYears + day;
}

//-----------------------------------------------------
// getComponent()
// Return requested components of date.
// Year : Includes the century.
// Month: 1..12
// Day  : 1..31 including fractional part
//-----------------------------------------------------
void cJulian::getComponent(int *pYear, int *pMon /* = NULL */,
        double *pDOM /* = NULL */) const
{
    assert(pYear != NULL);

    const double jdAdj = getDate() + 0.5;
    const int Z = static_cast<int>(jdAdj);               // integer part
    const double F = static_cast<double>(jdAdj - Z);     // fractional part
    const double alpha = static_cast<int>((Z - 1867216.25) / 36524.25);
    const double A = Z + 1 + alpha - static_cast<int>(alpha / 4.0);
    const double B = A + 1524.0;
    const int C = static_cast<int>((B - 122.1) / 365.25);
    const int D = static_cast<int>(C * 365.25);
    const int E = static_cast<int>((B - D) / 30.6001);

    const double DOM = B - D - static_cast<int>(E * 30.6001) + F;
    const int month = (E < 13.5) ? (E - 1) : (E - 13);
    const int year = (month > 2.5) ? (C - 4716) : (C - 4715);

    *pYear = year;

    if (pMon != NULL)
        *pMon = month;

    if (pDOM != NULL)
        *pDOM = DOM;
}

//-----------------------------------------------------
// toGMST()
// Calculate Greenwich Mean Sidereal Time for the Julian date. The return value
// is the angle, in radians, measuring eastward from the Vernal Equinox to the
// prime meridian. This angle is also referred to as "ThetaG" (Theta GMST).
//
// References:
//    The 1992 Astronomical Almanac, page B6.
//    Explanatory Supplement to the Astronomical Almanac, page 50.protected:
//    Orbital Coordinate Systems, Part III, Dr. T.S. Kelso, Satellite Times,
//       Nov/Dec 1995
//-----------------------------------------------------
double cJulian::toGMST() const
{
    const double UT = fmod(m_Date + 0.5, 1.0);
    const double TU = (FromJan1_12h_2000() - UT) / 36525.0;

    double GMST = 24110.54841
            + TU * (8640184.812866 + TU * (0.093104 - TU * 6.2e-06));

    GMST = std::fmod(GMST + SEC_PER_DAY * OMEGA_E * UT, SEC_PER_DAY);

    if (GMST < 0.0)
        GMST += SEC_PER_DAY; // "wrap" negative modulo value

    return (TWOPI * (GMST / SEC_PER_DAY));
}

//-----------------------------------------------------
// toLMST()
// Calculate Local Mean Sidereal Time for given longitude (for this date).
// The longitude is assumed to be in radians measured west from Greenwich.
// The return value is the angle, in radians, measuring eastward from the
// Vernal Equinox to the given longitude.
//-----------------------------------------------------
double cJulian::toLMST(double lon) const
{
    return std::fmod(toGMST() + lon, TWOPI);
}

//-----------------------------------------------------
// toTime()
// Convert to type time_t
// Avoid using this function as it discards the fractional seconds of the
// time component.
//-----------------------------------------------------
std::time_t cJulian::toTime() const
{
    int nYear;
    int nMonth;
    double dblDay;

    getComponent(&nYear, &nMonth, &dblDay);

    // dblDay is the fractional Julian Day (i.e., 29.5577).
    // Save the whole number day in nDOM and convert dblDay to
    // the fractional portion of day.
    int nDOM = static_cast<int>(dblDay);

    dblDay -= nDOM;

    const int SEC_PER_MIN = 60;
    const int SEC_PER_HR = 60 * SEC_PER_MIN;
    const int SEC_PER_DAY = 24 * SEC_PER_HR;

    const int secs = static_cast<int>((dblDay * SEC_PER_DAY) + 0.5);

    // Create a "struct tm" type.
    // NOTE:
    // The "struct tm" type has a 1-second resolution. Any fractional
    // component of the "seconds" time value is discarded.
    struct tm tGMT;
    std::memset(&tGMT, 0, sizeof(tGMT));

    tGMT.tm_year = nYear - 1900; // 2001 is 101
    tGMT.tm_mon = nMonth - 1; // January is 0
    tGMT.tm_mday = nDOM; // First day is 1
    tGMT.tm_hour = secs / SEC_PER_HR;
    tGMT.tm_min = (secs % SEC_PER_HR) / SEC_PER_MIN;
    tGMT.tm_sec = (secs % SEC_PER_HR) % SEC_PER_MIN;
    tGMT.tm_isdst = 0; // No conversion desired

    std::time_t tEpoch = std::mktime(&tGMT);

    if (tEpoch != -1) {
        // Valid time_t value returned from mktime().
        // mktime() expects a local time which means that tEpoch now needs
        // to be adjusted by the difference between this time zone and GMT.
#if defined(_WIN32)
        long _timezone = 0;
        tEpoch -= _timezone;
#else
        tEpoch -= timezone;
#endif
    }

    return tEpoch;
}
