/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */
#ifndef MODEL_COMPACT_SPICE_DEVSUP_H_
#define MODEL_COMPACT_SPICE_DEVSUP_H_

#undef PREDICTOR /* Currently PREDICTOR is not supported for SPICE model */
#define NEWCONV /* Always defined in SPICE */

/* old 'mode' parameters */
#define MODE 0x3U
#define MODETRAN 0x1U
#define MODEAC 0x2U

/* for noise analysis */
#define MODEACNOISE 0x8U

/* old 'modedc' parameters */
#define MODEDC 0x70U
#define MODEDCOP 0x10U
#define MODETRANOP 0x20U
#define MODEDCTRANCURVE 0x40U

/* old 'initf' parameters */
#define INITF 0x3f00U
#define MODEINITFLOAT 0x100U
#define MODEINITJCT 0x200U
#define MODEINITFIX 0x400U
#define MODEINITSMSIG 0x800U
#define MODEINITTRAN 0x1000U
#define MODEINITPRED 0x2000U

/* old 'nosolv' paramater */
#define MODEUIC 0x10000UL


#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define SMOOTHFACTOR 0.1
#define EPSOX 3.453133e-11
#define EPSSI 1.03594e-10
#define PI 3.141592654
#define Charge_q 1.60219e-19
#define Meter2Micron 1.0e6
#define CONSTCtoK 273.15
#define CONSTsqrt2 1.4142135623730950488016887242097
#define CONSTroot2 CONSTsqrt2
#define CONSTboltz 1.38064852e-23
#define CHARGE 1.6021766208e-19
#define CONSTnap 2.7182818284590452353602874713527
#define REFTEMP (27.0 + CONSTCtoK) /* 27 degrees C in K */
#define CONSTvt0 (CONSTboltz * REFTEMP / CHARGE)
#define CONSTKoverQ = (CONSTboltz / CHARGE)
#define CONSTe CONSTnap;

#include "csim/internal/IntegralHistory.h"

namespace csimModel
{

    int ACM_SourceDrainResistances(int, double, double, double, double, double,
                                   double, double, int, double, double, double,
                                   int, double, double, double, double *, double *);

    int ACM_saturationCurrents(int, int, int, double, double, double, double, double,
                               double, int, double, int, double, int, double, int,
                               double, double *, double *);

    int ACM_junctionCapacitances(int, int, int, double, double, double, double, int,
                                 double, int, double, int, double, int, double,
                                 double, double, double, double *, double *,
                                 double *, double *, double *, double *);

    double DEVlimvds(double vnew, double vold);
    double DEVfetlim(double, double, double);
    double DEVpnjlim(double vnew, double vold, double vt, double vcrit, int *icheck);

    void spiceTruncErr(const csim::IntegralHistory *, const csim::IntegralHistory *, const csim::IntegralHistory *,
                       double, double, double, double,
                       double, unsigned int, double *);
}

#endif // MODEL_COMPACT_#define DEVSUP_H_
