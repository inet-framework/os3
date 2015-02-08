//-----------------------------------------------------
// cJulian.h
//
// Copyright (c) 2003 Michael F. Henry
//-----------------------------------------------------
#ifndef __LIBNORAD_cJulian_H__
#define __LIBNORAD_cJulian_H__

#include <ctime>

#include "globals.h"

//-----------------------------------------------------
// See note in cJulian.cpp for information on this class and the epoch dates
//-----------------------------------------------------
const double EPOCH_JAN1_00H_1900 = 2415019.5; // Jan 1.0 1900 = Jan 1 1900 00h UTC
const double EPOCH_JAN1_12H_1900 = 2415020.0; // Jan 1.5 1900 = Jan 1 1900 12h UTC
const double EPOCH_JAN1_12H_2000 = 2451545.0; // Jan 1.5 2000 = Jan 1 2000 12h UTC

class cJulian
{
public:
    cJulian();
    explicit cJulian(std::time_t t);           // Create from time_t
    explicit cJulian(int year, double day);    // Create from year, day of year
    explicit cJulian(int year,                 // i.e., 2004
            int mon, // 1..12
            int day, // 1..31
            int hour, // 0..23
            int min, // 0..59
            double sec = 0.0); // 0..(59.999999...)
    ~cJulian() {}

    double toGMST() const;                     // Greenwich Mean Sidereal Time
    double toLMST(double lon) const;           // Local Mean Sideral Time
    std::time_t toTime() const;                // To time_t type - avoid using

    double FromJan1_00h_1900() const           { return m_Date - EPOCH_JAN1_00H_1900; }
    double FromJan1_12h_1900() const           { return m_Date - EPOCH_JAN1_12H_1900; }
    double FromJan1_12h_2000() const           { return m_Date - EPOCH_JAN1_12H_2000; }

    void getComponent(int* pYear, int* pMon = NULL, double* pDOM = NULL) const;

    double getDate() const                     { return m_Date;                   }

    void addDay(double day)                    { m_Date += day;                   }
    void addHour(double hr)                    { m_Date += (hr / HR_PER_DAY);     }
    void addMin(double min)                    { m_Date += (min / MIN_PER_DAY);   }
    void addSec(double sec)                    { m_Date += (sec / SEC_PER_DAY);   }

    double spanDay(const cJulian& b) const     { return m_Date - b.m_Date;        }
    double spanHour(const cJulian& b) const    { return spanDay(b) * HR_PER_DAY;  }
    double spanMin(const cJulian& b) const     { return spanDay(b) * MIN_PER_DAY; }
    double spanSec(const cJulian& b) const     { return spanDay(b) * SEC_PER_DAY; }

    static bool IsLeapYear(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }
protected:
    void initialize(int year, double day);

    double m_Date; // Julian date
};

#endif
