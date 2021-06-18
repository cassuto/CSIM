/**
 * @file SPICE compatible device support
 */

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: 2000 AlansFixes
**********/
/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition; you can redistribute it and/or           
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation; either      
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cmath>
#include <algorithm>
#include "csim/model/compact/SPICE-Devsup.h"

namespace csimModel
{

    /* Area Calculation Method (ACM) for MOS models (devsup.c) */
    int
    ACM_SourceDrainResistances(
        int ACM,
        double LD,
        double LDIF,
        double HDIF,
        double WMLT,
        double w,
        double XW,
        double RSH,
        int drainSquaresGiven,
        double RD,
        double RDC,
        double drainSquares,
        int sourceSquaresGiven,
        double RS,
        double RSC,
        double sourceSquares,
        double *drainResistance,
        double *sourceResistance)
    {
        switch (ACM)
        {
        case 1:
        case 11:
            *drainResistance = (LD + LDIF) / (w * WMLT + XW) * RD + RSH * drainSquares + RDC;
            *sourceResistance = (LD + LDIF) / (w * WMLT + XW) * RS + RSH * sourceSquares + RSC;

            break;

        case 2:
        case 12:
        case 3:
        case 13:
            if (drainSquaresGiven)
                *drainResistance = (LD + LDIF) / (w * WMLT + XW) * RD + RSH * drainSquares + RDC;
            else
                *drainResistance = ((LD + LDIF) * RD + (HDIF * WMLT) * RSH) / (w * WMLT + XW) + RDC;
            if (sourceSquaresGiven)
                *sourceResistance = (LD + LDIF) / (w * WMLT + XW) * RS + RSH * sourceSquares + RSC;
            else
                *sourceResistance = ((LD + LDIF) * RS + (HDIF * WMLT) * RSH) / (w * WMLT + XW) + RSC;

            break;

        default:
            break;
        }

        return 0;
    }

    /* Area Calculation Method (ACM) for MOS models */
    int
    ACM_saturationCurrents(
        int ACM,
        int CALCACM,
        int GEO,
        double HDIF,
        double WMLT,
        double w,
        double XW,
        double jctTempSatCurDensity,
        double jctSidewallTempSatCurDensity,
        int drainAreaGiven,
        double drainArea,
        int drainPerimeterGiven,
        double drainPerimeter,
        int sourceAreaGiven,
        double sourceArea,
        int sourcePerimeterGiven,
        double sourcePerimeter,
        double *DrainSatCurrent,
        double *SourceSatCurrent)
    {
        switch (ACM)
        {
        case 1:
        case 11:
            drainArea = (w * WMLT + XW) * WMLT;
            drainPerimeter = (w * WMLT + XW);
            *DrainSatCurrent = drainArea * jctTempSatCurDensity + drainPerimeter * jctSidewallTempSatCurDensity;
            if (*DrainSatCurrent <= 0.0)
                *DrainSatCurrent = 1.0e-14;

            sourceArea = (w * WMLT + XW) * WMLT;
            sourcePerimeter = (w * WMLT + XW);
            *SourceSatCurrent = sourceArea * jctTempSatCurDensity + sourcePerimeter * jctSidewallTempSatCurDensity;
            if (*SourceSatCurrent <= 0.0)
                *SourceSatCurrent = 1.0e-14;

            break;

        case 2:
        case 12:
            if ((ACM == 2) || ((ACM == 12) && (CALCACM == 1)))
            {
                if (!drainAreaGiven)
                    drainArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    drainArea = drainArea * WMLT * WMLT;
                if (!drainPerimeterGiven)
                    drainPerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
                else
                    drainPerimeter = drainPerimeter * WMLT;
            }
            *DrainSatCurrent = drainArea * jctTempSatCurDensity + drainPerimeter * jctSidewallTempSatCurDensity;
            if (*DrainSatCurrent <= 0.0)
                *DrainSatCurrent = 1.0e-14;

            if ((ACM == 2) || ((ACM == 12) && (CALCACM == 1)))
            {
                if (!sourceAreaGiven)
                    sourceArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    sourceArea = sourceArea * WMLT * WMLT;
                if (!sourcePerimeterGiven)
                    sourcePerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
                else
                    sourcePerimeter = sourcePerimeter * WMLT;
            }
            *SourceSatCurrent = sourceArea * jctTempSatCurDensity + sourcePerimeter * jctSidewallTempSatCurDensity;
            if (*SourceSatCurrent <= 0.0)
                *SourceSatCurrent = 1.0e-14;

            break;

        case 3:
        case 13:
            if (!drainAreaGiven)
                if ((GEO == 0) || (GEO == 2))
                    drainArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    drainArea = (HDIF * WMLT) * (w * WMLT + XW);
            else
                drainArea = drainArea * WMLT * WMLT;
            if (!drainPerimeterGiven)
                if ((GEO == 0) || (GEO == 2))
                    drainPerimeter = 4.0 * (HDIF * WMLT) + (w * WMLT + XW);
                else
                    drainPerimeter = 2.0 * (HDIF * WMLT);
            else
                drainPerimeter = drainPerimeter * WMLT;
            *DrainSatCurrent = drainArea * jctTempSatCurDensity + drainPerimeter * jctSidewallTempSatCurDensity;
            if (*DrainSatCurrent <= 0.0)
                *DrainSatCurrent = 1.0e-14;

            if (!sourceAreaGiven)
                if ((GEO == 0) || (GEO == 1))
                    sourceArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    sourceArea = (HDIF * WMLT) * (w * WMLT + XW);
            else
                sourceArea = sourceArea * WMLT * WMLT;
            if (!sourcePerimeterGiven)
                if ((GEO == 0) || (GEO == 1))
                    sourcePerimeter = 4.0 * (HDIF * WMLT) + (w * WMLT + XW);
                else
                    sourcePerimeter = 2.0 * (HDIF * WMLT);
            else
                sourcePerimeter = sourcePerimeter * WMLT;
            *SourceSatCurrent = sourceArea * jctTempSatCurDensity + sourcePerimeter * jctSidewallTempSatCurDensity;
            if (*SourceSatCurrent <= 0.0)
                *SourceSatCurrent = 1.0e-14;

            break;

        default:
            break;
        }

        return 0;
    }

    int
    ACM_junctionCapacitances(
        int ACM,
        int CALCACM,
        int GEO,
        double HDIF,
        double WMLT,
        double w,
        double XW,
        int drainAreaGiven,
        double drainArea,
        int drainPerimeterGiven,
        double drainPerimeter,
        int sourceAreaGiven,
        double sourceArea,
        int sourcePerimeterGiven,
        double sourcePerimeter,
        double CJ,
        double CJSW,
        double CJGATE,
        double *areaDrainBulkCapacitance,
        double *periDrainBulkCapacitance,
        double *gateDrainBulkCapacitance,
        double *areaSourceBulkCapacitance,
        double *periSourceBulkCapacitance,
        double *gateSourceBulkCapacitance)
    {
        switch (ACM)
        {
        case 1:
            drainArea = (w * WMLT + XW) * WMLT;
            drainPerimeter = (w * WMLT + XW);
            *areaDrainBulkCapacitance = drainArea * CJ;
            *periDrainBulkCapacitance = drainPerimeter * CJSW;
            *gateDrainBulkCapacitance = 0.0;

            sourceArea = (w * WMLT + XW) * WMLT;
            sourcePerimeter = (w * WMLT + XW);
            *areaSourceBulkCapacitance = sourceArea * CJ;
            *periSourceBulkCapacitance = sourcePerimeter * CJSW;
            *gateSourceBulkCapacitance = 0.0;

            break;

        case 2:
            if (!drainAreaGiven)
                drainArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
            else
                drainArea = drainArea * WMLT * WMLT;
            if (!drainPerimeterGiven)
                drainPerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
            else
                drainPerimeter = drainPerimeter * WMLT;
            *areaDrainBulkCapacitance = drainArea * CJ;
            if (drainPerimeter > (w * WMLT + XW))
            {
                *periDrainBulkCapacitance = (drainPerimeter - (w * WMLT + XW)) * CJSW;
                *gateDrainBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periDrainBulkCapacitance = drainPerimeter * CJGATE;
                *gateDrainBulkCapacitance = 0.0;
            }

            if (!sourceAreaGiven)
                sourceArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
            else
                sourceArea = sourceArea * WMLT * WMLT;
            if (!sourcePerimeterGiven)
                sourcePerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
            else
                sourcePerimeter = sourcePerimeter * WMLT;
            *areaSourceBulkCapacitance = sourceArea * CJ;
            if (sourcePerimeter > (w * WMLT + XW))
            {
                *periSourceBulkCapacitance = (sourcePerimeter - (w * WMLT + XW)) * CJSW;
                *gateSourceBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periSourceBulkCapacitance = sourcePerimeter * CJGATE;
                *gateSourceBulkCapacitance = 0.0;
            }

            break;

        case 3:
            if (!drainAreaGiven)
                if ((GEO == 0) || (GEO == 2))
                    drainArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    drainArea = (HDIF * WMLT) * (w * WMLT + XW);
            else
                drainArea = drainArea * WMLT * WMLT;
            if (!drainPerimeterGiven)
                if ((GEO == 0) || (GEO == 2))
                    drainPerimeter = 4.0 * (HDIF * WMLT) + (w * WMLT + XW);
                else
                    drainPerimeter = 2.0 * (HDIF * WMLT);
            else
                drainPerimeter = drainPerimeter * WMLT;
            *areaDrainBulkCapacitance = drainArea * CJ;
            *periDrainBulkCapacitance = drainPerimeter * CJSW;
            *gateDrainBulkCapacitance = (w * WMLT + XW) * CJGATE;

            if (!sourceAreaGiven)
                if ((GEO == 0) || (GEO == 1))
                    sourceArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    sourceArea = (HDIF * WMLT) * (w * WMLT + XW);
            else
                sourceArea = sourceArea * WMLT * WMLT;
            if (!sourcePerimeterGiven)
                if ((GEO == 0) || (GEO == 1))
                    sourcePerimeter = 4.0 * (HDIF * WMLT) + (w * WMLT + XW);
                else
                    sourcePerimeter = 2.0 * (HDIF * WMLT);
            else
                sourcePerimeter = sourcePerimeter * WMLT;
            *areaSourceBulkCapacitance = sourceArea * CJ;
            *periSourceBulkCapacitance = sourcePerimeter * CJSW;
            *gateSourceBulkCapacitance = (w * WMLT + XW) * CJGATE;

            break;

        case 11:
            drainArea = (w * WMLT + XW) * WMLT;
            drainPerimeter = (w * WMLT + XW);
            *areaDrainBulkCapacitance = drainArea * CJ;
            *periDrainBulkCapacitance = drainPerimeter * CJSW;
            *gateDrainBulkCapacitance = 0.0;

            sourceArea = (w * WMLT + XW) * WMLT;
            sourcePerimeter = (w * WMLT + XW);
            *areaSourceBulkCapacitance = sourceArea * CJ;
            *periSourceBulkCapacitance = sourcePerimeter * CJSW;
            *gateSourceBulkCapacitance = 0.0;

            break;

        case 12:
            if (CALCACM == 1)
            {
                if (!drainAreaGiven)
                    drainArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    drainArea = drainArea * WMLT * WMLT;
                if (!drainPerimeterGiven)
                    drainPerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
                else
                    drainPerimeter = drainPerimeter * WMLT;
            }
            *areaDrainBulkCapacitance = drainArea * CJ;
            if (drainPerimeter > (w * WMLT + XW))
            {
                *periDrainBulkCapacitance = (drainPerimeter - (w * WMLT + XW)) * CJSW;
                *gateDrainBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periDrainBulkCapacitance = 0.0;
                *gateDrainBulkCapacitance = drainPerimeter * CJGATE;
            }

            if (CALCACM == 1)
            {
                if (!sourceAreaGiven)
                    sourceArea = 2.0 * (HDIF * WMLT) * (w * WMLT + XW);
                else
                    sourceArea = sourceArea * WMLT * WMLT;
                if (!sourcePerimeterGiven)
                    sourcePerimeter = 4.0 * (HDIF * WMLT) + 2.0 * (w * WMLT + XW);
                else
                    sourcePerimeter = sourcePerimeter * WMLT;
            }
            *areaSourceBulkCapacitance = sourceArea * CJ;
            if (sourcePerimeter > (w * WMLT + XW))
            {
                *periSourceBulkCapacitance = (sourcePerimeter - (w * WMLT + XW)) * CJSW;
                *gateSourceBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periSourceBulkCapacitance = 0.0;
                *gateSourceBulkCapacitance = sourcePerimeter * CJGATE;
            }

            break;

        case 13:
            drainArea = drainArea * WMLT * WMLT;
            drainPerimeter = drainPerimeter * WMLT;
            *areaDrainBulkCapacitance = drainArea * CJ;
            if (drainPerimeter > (w * WMLT + XW))
            {
                *periDrainBulkCapacitance = (drainPerimeter - (w * WMLT + XW)) * CJSW;
                *gateDrainBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periDrainBulkCapacitance = 0.0;
                *gateDrainBulkCapacitance = drainPerimeter * CJGATE;
            }

            sourceArea = sourceArea * WMLT * WMLT;
            sourcePerimeter = sourcePerimeter * WMLT;
            *areaSourceBulkCapacitance = sourceArea * CJ;
            if (sourcePerimeter > (w * WMLT + XW))
            {
                *periSourceBulkCapacitance = (sourcePerimeter - (w * WMLT + XW)) * CJSW;
                *gateSourceBulkCapacitance = (w * WMLT + XW) * CJGATE;
            }
            else
            {
                *periSourceBulkCapacitance = 0.0;
                *gateSourceBulkCapacitance = sourcePerimeter * CJGATE;
            }

            break;

        default:
            break;
        }

        return 0;
    }

    /* 
    * Limit the per-iteration change of VDS 
    */
    double
    DEVlimvds(double vnew, double vold)
    {

        if (vold >= 3.5)
        {
            if (vnew > vold)
            {
                vnew = std::min(vnew, (3 * vold) + 2);
            }
            else
            {
                if (vnew < 3.5)
                {
                    vnew = std::max(vnew, 2.0);
                }
            }
        }
        else
        {
            if (vnew > vold)
            {
                vnew = std::min(vnew, 4.0);
            }
            else
            {
                vnew = std::max(vnew, -.5);
            }
        }
        return (vnew);
    }

    /*  
    * Limit the per-iteration change of PN junction voltages 
    *
    * This code has been fixed by Alan Gillespie adding limiting
    * for negative voltages.
    */
    double
    DEVpnjlim(double vnew, double vold, double vt, double vcrit, int *icheck)
    {
        double arg;

        if ((vnew > vcrit) && (fabs(vnew - vold) > (vt + vt)))
        {
            if (vold > 0)
            {
                arg = (vnew - vold) / vt;
                if (arg > 0)
                {
                    vnew = vold + vt * (2 + log(arg - 2));
                }
                else
                {
                    vnew = vold - vt * (2 + log(2 - arg));
                }
            }
            else
            {
                vnew = vt * log(vnew / vt);
            }
            *icheck = 1;
        }
        else
        {
            if (vnew < 0)
            {
                if (vold > 0)
                {
                    arg = -1 * vold - 1;
                }
                else
                {
                    arg = 2 * vold - 1;
                }
                if (vnew < arg)
                {
                    vnew = arg;
                    *icheck = 1;
                }
                else
                {
                    *icheck = 0;
                }
            }
            else
            {
                *icheck = 0;
            }
        }
        return (vnew);
    }

    /* 
    * Limit the per-iteration change of FET voltages 
    *
    * This code has been fixed by Alan Gillespie: a new
    * definition for vtstlo. 
    */
    double
    DEVfetlim(double vnew, double vold, double vto)
    {
        double vtsthi;
        double vtstlo;
        double vtox;
        double delv;
        double vtemp;

        vtsthi = fabs(2 * (vold - vto)) + 2;
        vtstlo = fabs(vold - vto) + 1;
        vtox = vto + 3.5;
        delv = vnew - vold;

        if (vold >= vto)
        {
            if (vold >= vtox)
            {
                if (delv <= 0)
                {
                    /* going off */
                    if (vnew >= vtox)
                    {
                        if (-delv > vtstlo)
                        {
                            vnew = vold - vtstlo;
                        }
                    }
                    else
                    {
                        vnew = std::max(vnew, vto + 2);
                    }
                }
                else
                {
                    /* staying on */
                    if (delv >= vtsthi)
                    {
                        vnew = vold + vtsthi;
                    }
                }
            }
            else
            {
                /* middle region */
                if (delv <= 0)
                {
                    /* decreasing */
                    vnew = std::max(vnew, vto - .5);
                }
                else
                {
                    /* increasing */
                    vnew = std::min(vnew, vto + 4);
                }
            }
        }
        else
        {
            /* off */
            if (delv <= 0)
            {
                if (-delv > vtsthi)
                {
                    vnew = vold - vtsthi;
                }
            }
            else
            {
                vtemp = vto + .5;
                if (vnew <= vtemp)
                {
                    if (delv > vtstlo)
                    {
                        vnew = vold + vtstlo;
                    }
                }
                else
                {
                    vnew = vtemp;
                }
            }
        }
        return (vnew);
    }

    void spiceTruncErr(
        const csim::IntegralHistory *x,
        const csim::IntegralHistory *y,
        const csim::IntegralHistory *hstep,
        double factor,
        double abstol,
        double reltol,
        double chgtol,
        double trtol,
        unsigned int order,
        double *timeStep)
    {
        double volttol;
        double chargetol;
        double tol;
        double del;
        double diff[8];
        double deltmp[8];
        int i;
        int j;

        volttol = abstol + reltol * std::max(fabs(x->get(0)), fabs(x->get(1)));

        chargetol = std::max(fabs(y->get(0)), fabs(y->get(1)));
        chargetol = reltol * std::max(chargetol, chgtol) / hstep->get(0);
        tol = std::max(volttol, chargetol);
        /* now divided differences */
        for (i = (int)order + 1; i >= 0; i--)
        {
            diff[i] = y->get(i);
        }
        for (i = 0; i <= (int)order; i++)
        {
            deltmp[i] = hstep->get(i);
        }
        j = (int)order;
        for (;;)
        {
            for (i = 0; i <= j; i++)
            {
                diff[i] = (diff[i] - diff[i + 1]) / deltmp[i];
            }
            if (--j < 0)
                break;
            for (i = 0; i <= j; i++)
            {
                deltmp[i] = deltmp[i + 1] + hstep->get(i);
            }
        }

        del = trtol * tol / std::max(abstol, factor * fabs(diff[0]));
        if (order == 2)
        {
            del = sqrt(del);
        }
        else if (order > 2)
        {
            del = exp(log(del) / order);
        }
        *timeStep = std::min(*timeStep, del);
    }
}
