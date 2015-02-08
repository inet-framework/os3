//-----------------------------------------------------
// cNoradSDP4.h
//
// This class implements the NORAD Simple Deep Perturbation 4 orbit
// model. This model provides the ECI coordinates/velocity of satellites
// with periods >= 225 minutes.
//
// Copyright (c) 2003 Michael F. Henry
//-----------------------------------------------------
#ifndef __LIBNORAD_cNoradSDP4_H__
#define __LIBNORAD_cNoradSDP4_H__

#include "cNoradBase.h"

class cOrbit;

class cNoradSDP4 : public cNoradBase
{
public:
   cNoradSDP4(const cOrbit& orbit);
   virtual ~cNoradSDP4();

   virtual bool getPosition(double tsince, cEci& eci);

protected:
   bool DeepInit(double* eosq,    double* sinio,    double* cosio,  double* m_betao,
                 double* m_aodp,  double* m_theta2, double* m_sing, double* m_cosg,
                 double* m_betao2,double* xmdot,    double* omgdot, double* xnodott);

   bool DeepSecular(double* xmdf,  double* omgadf,double* xnode, double* emm,
                    double* xincc, double* xnn,   double* tsince);
   bool DeepCalcDotTerms  (double* pxndot, double* pxnddt, double* pxldot);
   void DeepCalcIntegrator(double* pxndot, double* pxnddt, double* pxldot,
                           const double& delt);
   bool DeepPeriodics(double* e,     double* xincc,  double* omgadf,
                      double* xnode, double* xmam);
   double m_sing;
   double m_cosg;

   // Deep Initialization
   double eqsq;   double siniq;  double cosiq;  double rteqsq; double ao;
   double cosq2;  double sinomo; double cosomo; double bsq;    double xlldot;
   double omgdt;  double xnodot;

   // Deep Secular, Periodic
   double xll;    double omgasm; double xnodes; double _em;
   double xinc;   double xn;     double t;

   // Variables shared by "Deep" routines
   double dp_e3;     double dp_ee2;    double dp_savtsn; double dp_se2;
   double dp_se3;    double dp_sgh2;   double dp_sgh3;   double dp_sgh4;
   double dp_sghs;   double dp_sh2;    double dp_sh3;    double dp_si2;
   double dp_si3;    double dp_sl2;    double dp_sl3;    double dp_sl4;
   double dp_xgh2;   double dp_xgh3;   double dp_xgh4;   double dp_xh2;
   double dp_xh3;    double dp_xi2;    double dp_xi3;    double dp_xl2;
   double dp_xl3;    double dp_xl4;    double dp_xqncl;  double dp_zmol;
   double dp_zmos;

   double dp_atime;  double dp_d2201;  double dp_d2211;  double dp_d3210;
   double dp_d3222;  double dp_d4410;  double dp_d4422;  double dp_d5220;
   double dp_d5232;  double dp_d5421;  double dp_d5433;  double dp_del1;
   double dp_del2;   double dp_del3;   double dp_fasx2;  double dp_fasx4;
   double dp_fasx6;  double dp_omegaq; double dp_sse;    double dp_ssg;
   double dp_ssh;    double dp_ssi;    double dp_ssl;    double dp_step2;
   double dp_stepn;  double dp_stepp;  double dp_thgr;   double dp_xfact;
   double dp_xlamo;  double dp_xli;    double dp_xni;

   bool dp_iresfl;
   bool dp_isynfl;

   // DeepInit vars that change with epoch
   double dpi_c;      double dpi_ctem;   double dpi_day;    double dpi_gam;
   double dpi_stem;   double dpi_xnodce; double dpi_zcosgl; double dpi_zcoshl;
   double dpi_zcosil; double dpi_zsingl; double dpi_zsinhl; double dpi_zsinil;
   double dpi_zx;     double dpi_zy;

};

#endif

