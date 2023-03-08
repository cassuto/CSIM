/**** BSIM3v3.2.4, Released by Xuemei Xi 12/21/2001 ****/

/**********
 * Copyright 2001 Regents of the University of California. All rights reserved.
 * File: b3ld.c of BSIM3v3.2.4
 * Author: 1991 JianHui Huang and Min-Chie Jeng.
 * Modified by Mansun Chan (1995).
 * Author: 1997-1999 Weidong Liu.
 * Author: 2001 Xuemei Xi
 * Modified by Xuemei Xi, 10/05, 12/21, 2001.
 * Modified by Paolo Nenzi 2002 and Dietmar Warning 2003
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

#include <cmath>
#include <algorithm>
#include "csim/internal/IntegralHistory.h"
#include "bsim3v32.h"

#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define EPSOX 3.453133e-11
#define EPSSI 1.03594e-10
#define Charge_q 1.60219e-19
#define DELTA_1 0.02
#define DELTA_2 0.02
#define DELTA_3 0.02
#define DELTA_4 0.02

namespace csimModel
{

    int BSIM3V32::stampDCTran(Environment *env)
    {
        double SourceSatCurrent, DrainSatCurrent;
        double ag0, qgd, qgs, qgb, von, cbhat, VgstNVt, ExpVgst;
        double cdrain, cdhat, cdreq, ceqbd, ceqbs, ceqqb, ceqqd, ceqqg, ceq, geq;
        double czbd, czbdsw, czbdswg, czbs, czbssw, czbsswg, evbd, evbs, arg, sarg;
        double delvbd, delvbs, delvds, delvgd, delvgs;
        double Vfbeff, dVfbeff_dVg, dVfbeff_dVd = 0.0, dVfbeff_dVb, V3, V4;
        double gcbdb, gcbgb, gcbsb, gcddb, gcdgb, gcdsb, gcgdb, gcggb, gcgsb, gcsdb;
#ifndef NEWCONV
        double tol;
#endif
        double gcsgb, gcssb, MJ, MJSW, MJSWG;
        double vbd, vbs, vds, vgb, vgd, vgs, vgdo;
#ifndef PREDICTOR
        double xfact;
#endif
        double qgate = 0.0, qbulk = 0.0, qdrn = 0.0, qsrc;
        double qinoi, cqgate, cqbulk, cqdrn;
        double Vds, Vgs, Vbs, Gmbs, FwdSum, RevSum;
        double Vgs_eff, Vfb, dVfb_dVb = 0.0, dVfb_dVd = 0.0;
        double Phis, dPhis_dVb, sqrtPhis, dsqrtPhis_dVb, Vth, dVth_dVb, dVth_dVd;
        double Vgst, dVgst_dVg, dVgst_dVb, dVgs_eff_dVg, Nvtm;
        double Vtm;
        double n, dn_dVb, dn_dVd, voffcv, noff, dnoff_dVd, dnoff_dVb;
        double ExpArg, V0, CoxWLcen, QovCox, LINK;
        double DeltaPhi, dDeltaPhi_dVg, dDeltaPhi_dVd, dDeltaPhi_dVb;
        double Cox, Tox, Tcen, dTcen_dVg, dTcen_dVd, dTcen_dVb;
        double Ccen, Coxeff, dCoxeff_dVg, dCoxeff_dVd, dCoxeff_dVb;
        double Denomi, dDenomi_dVg, dDenomi_dVd, dDenomi_dVb;
        double ueff, dueff_dVg, dueff_dVd, dueff_dVb;
        double Esat, Vdsat;
        double EsatL, dEsatL_dVg, dEsatL_dVd, dEsatL_dVb;
        double dVdsat_dVg, dVdsat_dVb, dVdsat_dVd, Vasat, dAlphaz_dVg, dAlphaz_dVb;
        double dVasat_dVg, dVasat_dVb, dVasat_dVd, Va, dVa_dVd, dVa_dVg, dVa_dVb;
        double Vbseff, dVbseff_dVb, VbseffCV, dVbseffCV_dVb;
        double Arg1, One_Third_CoxWL, Two_Third_CoxWL, Alphaz, CoxWL;
        double T0, dT0_dVg, dT0_dVd, dT0_dVb;
        double T1, dT1_dVg, dT1_dVd, dT1_dVb;
        double T2, dT2_dVg, dT2_dVd, dT2_dVb;
        double T3, dT3_dVg, dT3_dVd, dT3_dVb;
        double T4;
        double T5;
        double T6;
        double T7;
        double T8;
        double T9;
        double T10;
        double T11, T12;
        double tmp, Abulk, dAbulk_dVb, Abulk0, dAbulk0_dVb;
        double VACLM, dVACLM_dVg, dVACLM_dVd, dVACLM_dVb;
        double VADIBL, dVADIBL_dVg, dVADIBL_dVd, dVADIBL_dVb;
        double Xdep, dXdep_dVb, lt1, dlt1_dVb, ltw, dltw_dVb, Delt_vth, dDelt_vth_dVb;
        double Theta0, dTheta0_dVb;
        double TempRatio, tmp1, tmp2, tmp3, tmp4;
        double DIBL_Sft, dDIBL_Sft_dVd, Lambda, dLambda_dVg;
        double Idtot, Ibtot;
#ifndef NOBYPASS
        double tempv;
#endif
        double a1, ScalingFactor;

        double Vgsteff, dVgsteff_dVg, dVgsteff_dVd, dVgsteff_dVb;
        double Vdseff, dVdseff_dVg, dVdseff_dVd, dVdseff_dVb;
        double VdseffCV, dVdseffCV_dVg, dVdseffCV_dVd, dVdseffCV_dVb;
        double diffVds, dAbulk_dVg;
        double beta, dbeta_dVg, dbeta_dVd, dbeta_dVb;
        double gche, dgche_dVg, dgche_dVd, dgche_dVb;
        double fgche1, dfgche1_dVg, dfgche1_dVd, dfgche1_dVb;
        double fgche2, dfgche2_dVg, dfgche2_dVd, dfgche2_dVb;
        double Idl, dIdl_dVg, dIdl_dVd, dIdl_dVb;
        double Idsa, dIdsa_dVg, dIdsa_dVd, dIdsa_dVb;
        double Ids, Gm, Gds, Gmb;
        double Isub, Gbd, Gbg, Gbb;
        double VASCBE, dVASCBE_dVg, dVASCBE_dVd, dVASCBE_dVb;
        double CoxWovL;
        double Rds, dRds_dVg, dRds_dVb, WVCox, WVCoxRds;
        double Vgst2Vtm, VdsatCV, dVdsatCV_dVg, dVdsatCV_dVb;
        double Leff, Weff, dWeff_dVg, dWeff_dVb;
        double AbulkCV, dAbulkCV_dVb;
        double qgdo, qgso, cgdo, cgso;

        double qcheq = 0.0, qdef, gqdef = 0.0, cqdef, cqcheq, gtau_diff, gtau_drift;
        double gcqdb = 0.0, gcqsb = 0.0, gcqgb = 0.0, gcqbb = 0.0;
        double dxpart, sxpart, ggtg, ggtd, ggts, ggtb;
        double ddxpart_dVd, ddxpart_dVg, ddxpart_dVb, ddxpart_dVs;
        double dsxpart_dVd, dsxpart_dVg, dsxpart_dVb, dsxpart_dVs;

        double gbspsp, gbbdp, gbbsp, gbspg, gbspb, gbspdp;
        double gbdpdp, gbdpg, gbdpb, gbdpsp;
        double Cgg, Cgd, Cgb, Cdg, Cdd, Cds;
        double Csg, Csd, Css, Csb, Cbg, Cbd, Cbb;
        double Cgg1, Cgb1, Cgd1, Cbg1, Cbb1, Cbd1, Qac0, Qsub0;
        double dQac0_dVg, dQac0_dVd = 0.0, dQac0_dVb, dQsub0_dVg;
        double dQsub0_dVd, dQsub0_dVb;

        double m;

        printf("CKTstate1:\n");
        for (int kk = 0; kk < BSIM3v32numStates; kk++)
        {
            printf("%d %lf\n", kk, getIntegratorX(kk)->get(1));
        }
        printf("\n");

        struct bsim3v32SizeDependParam *pParam;
        int ByPass, Check, ChargeComputationNeeded, error;
        int CKTnoncon = 0;

        ScalingFactor = 1.0e-9;
        ChargeComputationNeeded =
            ((env->getCKTmode() & (MODEDCTRANCURVE | MODEAC | MODETRAN | MODEINITSMSIG)) ||
             ((env->getCKTmode() & MODETRANOP) && (env->getCKTmode() & MODEUIC)))
                ? 1
                : 0;

        Check = 1;
        ByPass = 0;
        pParam = this->pParam;
        if ((env->getCKTmode() & MODEINITSMSIG))
        {
            vbs = getIntegratorX(BSIM3v32vbs)->get(0);
            vgs = getIntegratorX(BSIM3v32vgs)->get(0);
            vds = getIntegratorX(BSIM3v32vds)->get(0);
            qdef = getIntegratorX(BSIM3v32qdef)->get(0);
        }
        else if ((env->getCKTmode() & MODEINITTRAN))
        {
            vbs = getIntegratorX(BSIM3v32vbs)->get(1);
            vgs = getIntegratorX(BSIM3v32vgs)->get(1);
            vds = getIntegratorX(BSIM3v32vds)->get(1);
            qdef = getIntegratorX(BSIM3v32qdef)->get(1);
        }
        else if ((env->getCKTmode() & MODEINITJCT) && !this->BSIM3v32off)
        {
            vds = model->BSIM3v32type * this->BSIM3v32icVDS;
            vgs = model->BSIM3v32type * this->BSIM3v32icVGS;
            vbs = model->BSIM3v32type * this->BSIM3v32icVBS;
            qdef = 0.0;

            if ((vds == 0.0) && (vgs == 0.0) && (vbs == 0.0) &&
                ((env->getCKTmode() & (MODETRAN | MODEAC | MODEDCOP |
                                       MODEDCTRANCURVE)) ||
                 (!(env->getCKTmode() & MODEUIC))))
            {
                vbs = 0.0;
                vgs = model->BSIM3v32type * this->BSIM3v32vth0 + 0.1;
                vds = 0.1;
            }
        }
        else if ((env->getCKTmode() & (MODEINITJCT | MODEINITFIX)) &&
                 (this->BSIM3v32off))
        {
            qdef = vbs = vgs = vds = 0.0;
        }
        else
        {
#ifndef PREDICTOR
            if ((env->getCKTmode() & MODEINITPRED))
            {
                xfact = getIntegratorTimestep()->get(0) / getIntegratorTimestep()->get(1);
                getIntegratorX(BSIM3v32vbs)->set(0, getIntegratorX(BSIM3v32vbs)->get(1));
                vbs = (1.0 + xfact) * (getIntegratorX(BSIM3v32vbs)->get(1)) - (xfact * (getIntegratorX(BSIM3v32vbs)->get(2)));
                getIntegratorX(BSIM3v32vgs)->set(0, getIntegratorX(BSIM3v32vgs)->get(1));
                vgs = (1.0 + xfact) * (getIntegratorX(BSIM3v32vgs)->get(1)) - (xfact * (getIntegratorX(BSIM3v32vgs)->get(2)));
                getIntegratorX(BSIM3v32vds)->set(0, getIntegratorX(BSIM3v32vds)->get(1));
                vds = (1.0 + xfact) * (getIntegratorX(BSIM3v32vds)->get(1)) - (xfact * (getIntegratorX(BSIM3v32vds)->get(2)));
                getIntegratorX(BSIM3v32vbd)->set(0, getIntegratorX(BSIM3v32vbs)->get(0) - getIntegratorX(BSIM3v32vds)->get(0));
                getIntegratorX(BSIM3v32qdef)->set(0, getIntegratorX(BSIM3v32qdef)->get(1));
                qdef = (1.0 + xfact) * (getIntegratorX(BSIM3v32qdef)->get(1)) - (xfact * (getIntegratorX(BSIM3v32qdef)->get(2)));
            }
            else
            {
#endif /* PREDICTOR */
                vbs = model->BSIM3v32type * (getU(getNode(this->BSIM3v32bNode)).real() - getU(getNode(this->BSIM3v32sNodePrime)).real());
                vgs = model->BSIM3v32type * (getU(getNode(this->BSIM3v32gNode)).real() - getU(getNode(this->BSIM3v32sNodePrime)).real());
                vds = model->BSIM3v32type * (getU(getNode(this->BSIM3v32dNodePrime)).real() - getU(getNode(this->BSIM3v32sNodePrime)).real());
                printf("vbs = %lf\n", vbs);
                printf("vgs = %lf\n", vgs);
                printf("vds = %lf\n", vds);
                qdef = model->BSIM3v32type * (getU(getNode(this->BSIM3v32qNode)).real());
#ifndef PREDICTOR
            }
#endif /* PREDICTOR */

            vbd = vbs - vds;
            vgd = vgs - vds;
            vgdo = getIntegratorX(BSIM3v32vgs)->get(0) - getIntegratorX(BSIM3v32vds)->get(0);
            delvbs = vbs - getIntegratorX(BSIM3v32vbs)->get(0);
            delvbd = vbd - getIntegratorX(BSIM3v32vbd)->get(0);
            delvgs = vgs - getIntegratorX(BSIM3v32vgs)->get(0);
            delvds = vds - getIntegratorX(BSIM3v32vds)->get(0);
            delvgd = vgd - vgdo;

            if (this->BSIM3v32mode >= 0)
            {
                Idtot = this->BSIM3v32cd + this->BSIM3v32csub - this->BSIM3v32cbd;
                cdhat = Idtot - this->BSIM3v32gbd * delvbd + (this->BSIM3v32gmbs + this->BSIM3v32gbbs) * delvbs + (this->BSIM3v32gm + this->BSIM3v32gbgs) * delvgs + (this->BSIM3v32gds + this->BSIM3v32gbds) * delvds;
                Ibtot = this->BSIM3v32cbs + this->BSIM3v32cbd - this->BSIM3v32csub;
                cbhat = Ibtot + this->BSIM3v32gbd * delvbd + (this->BSIM3v32gbs - this->BSIM3v32gbbs) * delvbs - this->BSIM3v32gbgs * delvgs - this->BSIM3v32gbds * delvds;
            }
            else
            {
                Idtot = this->BSIM3v32cd - this->BSIM3v32cbd;
                cdhat = Idtot - (this->BSIM3v32gbd - this->BSIM3v32gmbs) * delvbd + this->BSIM3v32gm * delvgd - this->BSIM3v32gds * delvds;
                Ibtot = this->BSIM3v32cbs + this->BSIM3v32cbd - this->BSIM3v32csub;
                cbhat = Ibtot + this->BSIM3v32gbs * delvbs + (this->BSIM3v32gbd - this->BSIM3v32gbbs) * delvbd - this->BSIM3v32gbgs * delvgd + this->BSIM3v32gbds * delvds;
            }

#ifndef NOBYPASS
            /* following should be one big if connected by && all over
            * the place, but some C compilers can't handle that, so
            * we split it up here to let them digest it in stages
            */

            if ((!(env->getCKTmode() & MODEINITPRED)) && (env->getBypassEnabled()))
                if ((fabs(delvbs) < (env->getRelTol() * std::max(fabs(vbs),
                                                                 fabs(getIntegratorX(BSIM3v32vbs)->get(0))) +
                                     env->getVoltTol())))
                    if ((fabs(delvbd) < (env->getRelTol() * std::max(fabs(vbd),
                                                                     fabs(getIntegratorX(BSIM3v32vbd)->get(0))) +
                                         env->getVoltTol())))
                        if ((fabs(delvgs) < (env->getRelTol() * std::max(fabs(vgs),
                                                                         fabs(getIntegratorX(BSIM3v32vgs)->get(0))) +
                                             env->getVoltTol())))
                            if ((fabs(delvds) < (env->getRelTol() * std::max(fabs(vds),
                                                                             fabs(getIntegratorX(BSIM3v32vds)->get(0))) +
                                                 env->getVoltTol())))
                                if ((fabs(cdhat - Idtot) < env->getRelTol() * std::max(fabs(cdhat), fabs(Idtot)) + env->getAbsTol()))
                                {
                                    tempv = std::max(fabs(cbhat), fabs(Ibtot)) + env->getAbsTol();
                                    if ((fabs(cbhat - Ibtot)) < env->getRelTol() * tempv)
                                    { /* bypass code */
                                        vbs = getIntegratorX(BSIM3v32vbs)->get(0);
                                        vbd = getIntegratorX(BSIM3v32vbd)->get(0);
                                        vgs = getIntegratorX(BSIM3v32vgs)->get(0);
                                        vds = getIntegratorX(BSIM3v32vds)->get(0);
                                        qdef = getIntegratorX(BSIM3v32qdef)->get(0);

                                        vgd = vgs - vds;
                                        vgb = vgs - vbs;

                                        cdrain = this->BSIM3v32cd;
                                        if ((env->getCKTmode() & (MODETRAN | MODEAC)) ||
                                            ((env->getCKTmode() & MODETRANOP) &&
                                             (env->getCKTmode() & MODEUIC)))
                                        {
                                            ByPass = 1;
                                            qgate = this->BSIM3v32qgate;
                                            qbulk = this->BSIM3v32qbulk;
                                            qdrn = this->BSIM3v32qdrn;
                                            goto line755;
                                        }
                                        else
                                        {
                                            goto line850;
                                        }
                                    }
                                }

#endif /*NOBYPASS*/
            von = this->BSIM3v32von;
            if (getIntegratorX(BSIM3v32vds)->get(0) >= 0.0)
            {
                vgs = DEVfetlim(vgs, getIntegratorX(BSIM3v32vgs)->get(0), von);
                vds = vgs - vgd;
                vds = DEVlimvds(vds, getIntegratorX(BSIM3v32vds)->get(0));
                vgd = vgs - vds;
            }
            else
            {
                vgd = DEVfetlim(vgd, vgdo, von);
                vds = vgs - vgd;
                vds = -DEVlimvds(-vds, -(getIntegratorX(BSIM3v32vds)->get(0)));
                vgs = vgd + vds;
            }

            if (vds >= 0.0)
            {
                vbs = DEVpnjlim(vbs, getIntegratorX(BSIM3v32vbs)->get(0),
                                CONSTvt0, model->BSIM3v32vcrit, &Check);
                vbd = vbs - vds;
            }
            else
            {
                vbd = DEVpnjlim(vbd, getIntegratorX(BSIM3v32vbd)->get(0),
                                CONSTvt0, model->BSIM3v32vcrit, &Check);
                vbs = vbd + vds;
            }
        }

        /* determine DC current and derivatives */
        vbd = vbs - vds;
        vgd = vgs - vds;
        vgb = vgs - vbs;

        /* Source/drain junction diode DC model begins */
        Nvtm = model->BSIM3v32vtm * model->BSIM3v32jctEmissionCoeff;
        /* acm model */
        if (model->BSIM3v32acmMod == 0)
        {
            if ((this->BSIM3v32sourceArea <= 0.0) && (this->BSIM3v32sourcePerimeter <= 0.0))
            {
                SourceSatCurrent = 1.0e-14;
            }
            else
            {
                SourceSatCurrent = this->BSIM3v32sourceArea * model->BSIM3v32jctTempSatCurDensity + this->BSIM3v32sourcePerimeter * model->BSIM3v32jctSidewallTempSatCurDensity;
            }
            if ((this->BSIM3v32drainArea <= 0.0) && (this->BSIM3v32drainPerimeter <= 0.0))
            {
                DrainSatCurrent = 1.0e-14;
            }
            else
            {
                DrainSatCurrent = this->BSIM3v32drainArea * model->BSIM3v32jctTempSatCurDensity + this->BSIM3v32drainPerimeter * model->BSIM3v32jctSidewallTempSatCurDensity;
            }
        }
        else
        {
            error = ACM_saturationCurrents(
                model->BSIM3v32acmMod,
                model->BSIM3v32calcacm,
                this->BSIM3v32geo,
                model->BSIM3v32hdif,
                model->BSIM3v32wmlt,
                this->BSIM3v32w,
                model->BSIM3v32xw,
                model->BSIM3v32jctTempSatCurDensity,
                model->BSIM3v32jctSidewallTempSatCurDensity,
                this->BSIM3v32drainAreaGiven,
                this->BSIM3v32drainArea,
                this->BSIM3v32drainPerimeterGiven,
                this->BSIM3v32drainPerimeter,
                this->BSIM3v32sourceAreaGiven,
                this->BSIM3v32sourceArea,
                this->BSIM3v32sourcePerimeterGiven,
                this->BSIM3v32sourcePerimeter,
                &DrainSatCurrent,
                &SourceSatCurrent);
            if (error)
                return (error);
        }
        if (SourceSatCurrent <= 0.0)
        {
            this->BSIM3v32gbs = env->getGmin();
            this->BSIM3v32cbs = this->BSIM3v32gbs * vbs;
        }
        else
        {
            if (model->BSIM3v32ijth == 0.0)
            {
                evbs = exp(vbs / Nvtm);
                this->BSIM3v32gbs = SourceSatCurrent * evbs / Nvtm + env->getGmin();
                this->BSIM3v32cbs = SourceSatCurrent * (evbs - 1.0) + env->getGmin() * vbs;
            }
            else
            {
                if (vbs < this->BSIM3v32vjsm)
                {
                    evbs = exp(vbs / Nvtm);
                    this->BSIM3v32gbs = SourceSatCurrent * evbs / Nvtm + env->getGmin();
                    this->BSIM3v32cbs = SourceSatCurrent * (evbs - 1.0) + env->getGmin() * vbs;
                }
                else
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        T0 = this->BSIM3v32IsEvjsm / Nvtm;
                        this->BSIM3v32gbs = T0 + (env->getGmin());
                        this->BSIM3v32cbs = this->BSIM3v32IsEvjsm - SourceSatCurrent + T0 * (vbs - this->BSIM3v32vjsm) + (env->getGmin()) * vbs;
                        break;
                    case BSIM3v32V32:
                    default:
                        T0 = (SourceSatCurrent + model->BSIM3v32ijth) / Nvtm;
                        this->BSIM3v32gbs = T0 + (env->getGmin());
                        this->BSIM3v32cbs = model->BSIM3v32ijth + (env->getGmin()) * vbs + T0 * (vbs - this->BSIM3v32vjsm);
                    }
                }
            }
        }

        if (DrainSatCurrent <= 0.0)
        {
            this->BSIM3v32gbd = env->getGmin();
            this->BSIM3v32cbd = this->BSIM3v32gbd * vbd;
        }
        else
        {
            if (model->BSIM3v32ijth == 0.0)
            {
                evbd = exp(vbd / Nvtm);
                this->BSIM3v32gbd = DrainSatCurrent * evbd / Nvtm + env->getGmin();
                this->BSIM3v32cbd = DrainSatCurrent * (evbd - 1.0) + env->getGmin() * vbd;
            }
            else
            {
                if (vbd < this->BSIM3v32vjdm)
                {
                    evbd = exp(vbd / Nvtm);
                    this->BSIM3v32gbd = DrainSatCurrent * evbd / Nvtm + env->getGmin();
                    this->BSIM3v32cbd = DrainSatCurrent * (evbd - 1.0) + env->getGmin() * vbd;
                }
                else
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        T0 = this->BSIM3v32IsEvjdm / Nvtm;
                        this->BSIM3v32gbd = T0 + (env->getGmin());
                        this->BSIM3v32cbd = this->BSIM3v32IsEvjdm - DrainSatCurrent + T0 * (vbd - this->BSIM3v32vjdm) + (env->getGmin()) * vbd;
                        break;
                    case BSIM3v32V32:
                    default:
                        T0 = (DrainSatCurrent + model->BSIM3v32ijth) / Nvtm;
                        this->BSIM3v32gbd = T0 + (env->getGmin());
                        this->BSIM3v32cbd = model->BSIM3v32ijth + (env->getGmin()) * vbd + T0 * (vbd - this->BSIM3v32vjdm);
                    }
                }
            }
        }
        /* End of diode DC model */

        if (vds >= 0.0)
        { /* normal mode */
            this->BSIM3v32mode = 1;
            Vds = vds;
            Vgs = vgs;
            Vbs = vbs;
        }
        else
        { /* inverse mode */
            this->BSIM3v32mode = -1;
            Vds = -vds;
            Vgs = vgd;
            Vbs = vbd;
        }

        T0 = Vbs - pParam->BSIM3v32vbsc - 0.001;
        T1 = sqrt(T0 * T0 - 0.004 * pParam->BSIM3v32vbsc);
        Vbseff = pParam->BSIM3v32vbsc + 0.5 * (T0 + T1);
        dVbseff_dVb = 0.5 * (1.0 + T0 / T1);
        if (Vbseff < Vbs)
        {
            Vbseff = Vbs;
        }

        if (Vbseff > 0.0)
        {
            T0 = pParam->BSIM3v32phi / (pParam->BSIM3v32phi + Vbseff);
            Phis = pParam->BSIM3v32phi * T0;
            dPhis_dVb = -T0 * T0;
            sqrtPhis = pParam->BSIM3v32phis3 / (pParam->BSIM3v32phi + 0.5 * Vbseff);
            dsqrtPhis_dVb = -0.5 * sqrtPhis * sqrtPhis / pParam->BSIM3v32phis3;
        }
        else
        {
            Phis = pParam->BSIM3v32phi - Vbseff;
            dPhis_dVb = -1.0;
            sqrtPhis = sqrt(Phis);
            dsqrtPhis_dVb = -0.5 / sqrtPhis;
        }
        Xdep = pParam->BSIM3v32Xdep0 * sqrtPhis / pParam->BSIM3v32sqrtPhi;
        dXdep_dVb = (pParam->BSIM3v32Xdep0 / pParam->BSIM3v32sqrtPhi) * dsqrtPhis_dVb;

        Leff = pParam->BSIM3v32leff;
        Vtm = model->BSIM3v32vtm;
        /* Vth Calculation */
        T3 = sqrt(Xdep);
        V0 = pParam->BSIM3v32vbi - pParam->BSIM3v32phi;

        T0 = pParam->BSIM3v32dvt2 * Vbseff;
        if (T0 >= -0.5)
        {
            T1 = 1.0 + T0;
            T2 = pParam->BSIM3v32dvt2;
        }
        else /* Added to avoid any discontinuity problems caused by dvt2 */
        {
            T4 = 1.0 / (3.0 + 8.0 * T0);
            T1 = (1.0 + 3.0 * T0) * T4;
            T2 = pParam->BSIM3v32dvt2 * T4 * T4;
        }
        lt1 = model->BSIM3v32factor1 * T3 * T1;
        dlt1_dVb = model->BSIM3v32factor1 * (0.5 / T3 * T1 * dXdep_dVb + T3 * T2);

        T0 = pParam->BSIM3v32dvt2w * Vbseff;
        if (T0 >= -0.5)
        {
            T1 = 1.0 + T0;
            T2 = pParam->BSIM3v32dvt2w;
        }
        else /* Added to avoid any discontinuity problems caused by dvt2w */
        {
            T4 = 1.0 / (3.0 + 8.0 * T0);
            T1 = (1.0 + 3.0 * T0) * T4;
            T2 = pParam->BSIM3v32dvt2w * T4 * T4;
        }
        ltw = model->BSIM3v32factor1 * T3 * T1;
        dltw_dVb = model->BSIM3v32factor1 * (0.5 / T3 * T1 * dXdep_dVb + T3 * T2);

        T0 = -0.5 * pParam->BSIM3v32dvt1 * Leff / lt1;
        if (T0 > -EXP_THRESHOLD)
        {
            T1 = exp(T0);
            Theta0 = T1 * (1.0 + 2.0 * T1);
            dT1_dVb = -T0 / lt1 * T1 * dlt1_dVb;
            dTheta0_dVb = (1.0 + 4.0 * T1) * dT1_dVb;
        }
        else
        {
            T1 = MIN_EXP;
            Theta0 = T1 * (1.0 + 2.0 * T1);
            dTheta0_dVb = 0.0;
        }

        this->BSIM3v32thetavth = pParam->BSIM3v32dvt0 * Theta0;
        Delt_vth = this->BSIM3v32thetavth * V0;
        dDelt_vth_dVb = pParam->BSIM3v32dvt0 * dTheta0_dVb * V0;

        T0 = -0.5 * pParam->BSIM3v32dvt1w * pParam->BSIM3v32weff * Leff / ltw;
        if (T0 > -EXP_THRESHOLD)
        {
            T1 = exp(T0);
            T2 = T1 * (1.0 + 2.0 * T1);
            dT1_dVb = -T0 / ltw * T1 * dltw_dVb;
            dT2_dVb = (1.0 + 4.0 * T1) * dT1_dVb;
        }
        else
        {
            T1 = MIN_EXP;
            T2 = T1 * (1.0 + 2.0 * T1);
            dT2_dVb = 0.0;
        }

        T0 = pParam->BSIM3v32dvt0w * T2;
        T2 = T0 * V0;
        dT2_dVb = pParam->BSIM3v32dvt0w * dT2_dVb * V0;

        TempRatio = env->getTemp() / model->BSIM3v32tnom - 1.0;
        T0 = sqrt(1.0 + pParam->BSIM3v32nlx / Leff);
        T1 = pParam->BSIM3v32k1ox * (T0 - 1.0) * pParam->BSIM3v32sqrtPhi + (pParam->BSIM3v32kt1 + pParam->BSIM3v32kt1l / Leff + pParam->BSIM3v32kt2 * Vbseff) * TempRatio;
        tmp2 = model->BSIM3v32tox * pParam->BSIM3v32phi / (pParam->BSIM3v32weff + pParam->BSIM3v32w0);

        T3 = pParam->BSIM3v32eta0 + pParam->BSIM3v32etab * Vbseff;
        if (T3 < 1.0e-4) /* avoid  discontinuity problems caused by etab */
        {
            T9 = 1.0 / (3.0 - 2.0e4 * T3);
            T3 = (2.0e-4 - T3) * T9;
            T4 = T9 * T9;
        }
        else
        {
            T4 = 1.0;
        }
        dDIBL_Sft_dVd = T3 * pParam->BSIM3v32theta0vb0;
        DIBL_Sft = dDIBL_Sft_dVd * Vds;

        Vth = model->BSIM3v32type * this->BSIM3v32vth0 - pParam->BSIM3v32k1 * pParam->BSIM3v32sqrtPhi + pParam->BSIM3v32k1ox * sqrtPhis - pParam->BSIM3v32k2ox * Vbseff - Delt_vth - T2 + (pParam->BSIM3v32k3 + pParam->BSIM3v32k3b * Vbseff) * tmp2 + T1 - DIBL_Sft;

        this->BSIM3v32von = Vth;

        dVth_dVb = pParam->BSIM3v32k1ox * dsqrtPhis_dVb - pParam->BSIM3v32k2ox - dDelt_vth_dVb - dT2_dVb + pParam->BSIM3v32k3b * tmp2 - pParam->BSIM3v32etab * Vds * pParam->BSIM3v32theta0vb0 * T4 + pParam->BSIM3v32kt2 * TempRatio;
        dVth_dVd = -dDIBL_Sft_dVd;

        /* Calculate n */
        tmp2 = pParam->BSIM3v32nfactor * EPSSI / Xdep;
        tmp3 = pParam->BSIM3v32cdsc + pParam->BSIM3v32cdscb * Vbseff + pParam->BSIM3v32cdscd * Vds;
        tmp4 = (tmp2 + tmp3 * Theta0 + pParam->BSIM3v32cit) / model->BSIM3v32cox;
        if (tmp4 >= -0.5)
        {
            n = 1.0 + tmp4;
            dn_dVb = (-tmp2 / Xdep * dXdep_dVb + tmp3 * dTheta0_dVb + pParam->BSIM3v32cdscb * Theta0) / model->BSIM3v32cox;
            dn_dVd = pParam->BSIM3v32cdscd * Theta0 / model->BSIM3v32cox;
        }
        else
        /* avoid  discontinuity problems caused by tmp4 */
        {
            T0 = 1.0 / (3.0 + 8.0 * tmp4);
            n = (1.0 + 3.0 * tmp4) * T0;
            T0 *= T0;
            dn_dVb = (-tmp2 / Xdep * dXdep_dVb + tmp3 * dTheta0_dVb + pParam->BSIM3v32cdscb * Theta0) / model->BSIM3v32cox * T0;
            dn_dVd = pParam->BSIM3v32cdscd * Theta0 / model->BSIM3v32cox * T0;
        }

        /* Poly Gate Si Depletion Effect */
        T0 = this->BSIM3v32vfb + pParam->BSIM3v32phi;
        if ((pParam->BSIM3v32ngate > 1.e18) && (pParam->BSIM3v32ngate < 1.e25) && (Vgs > T0))
        /* added to avoid the problem caused by ngate */
        {
            T1 = 1.0e6 * Charge_q * EPSSI * pParam->BSIM3v32ngate / (model->BSIM3v32cox * model->BSIM3v32cox);
            T4 = sqrt(1.0 + 2.0 * (Vgs - T0) / T1);
            T2 = T1 * (T4 - 1.0);
            T3 = 0.5 * T2 * T2 / T1; /* T3 = Vpoly */
            T7 = 1.12 - T3 - 0.05;
            T6 = sqrt(T7 * T7 + 0.224);
            T5 = 1.12 - 0.5 * (T7 + T6);
            Vgs_eff = Vgs - T5;
            dVgs_eff_dVg = 1.0 - (0.5 - 0.5 / T4) * (1.0 + T7 / T6);
        }
        else
        {
            Vgs_eff = Vgs;
            dVgs_eff_dVg = 1.0;
        }
        Vgst = Vgs_eff - Vth;

        /* Effective Vgst (Vgsteff) Calculation */

        T10 = 2.0 * n * Vtm;
        VgstNVt = Vgst / T10;
        ExpArg = (2.0 * pParam->BSIM3v32voff - Vgst) / T10;

        /* MCJ: Very small Vgst */
        if (VgstNVt > EXP_THRESHOLD)
        {
            Vgsteff = Vgst;
            dVgsteff_dVg = dVgs_eff_dVg;
            dVgsteff_dVd = -dVth_dVd;
            dVgsteff_dVb = -dVth_dVb;
        }
        else if (ExpArg > EXP_THRESHOLD)
        {
            T0 = (Vgst - pParam->BSIM3v32voff) / (n * Vtm);
            ExpVgst = exp(T0);
            Vgsteff = Vtm * pParam->BSIM3v32cdep0 / model->BSIM3v32cox * ExpVgst;
            dVgsteff_dVg = Vgsteff / (n * Vtm);
            dVgsteff_dVd = -dVgsteff_dVg * (dVth_dVd + T0 * Vtm * dn_dVd);
            dVgsteff_dVb = -dVgsteff_dVg * (dVth_dVb + T0 * Vtm * dn_dVb);
            dVgsteff_dVg *= dVgs_eff_dVg;
        }
        else
        {
            ExpVgst = exp(VgstNVt);
            T1 = T10 * log(1.0 + ExpVgst);
            dT1_dVg = ExpVgst / (1.0 + ExpVgst);
            dT1_dVb = -dT1_dVg * (dVth_dVb + Vgst / n * dn_dVb) + T1 / n * dn_dVb;
            dT1_dVd = -dT1_dVg * (dVth_dVd + Vgst / n * dn_dVd) + T1 / n * dn_dVd;

            dT2_dVg = -model->BSIM3v32cox / (Vtm * pParam->BSIM3v32cdep0) * exp(ExpArg);
            T2 = 1.0 - T10 * dT2_dVg;
            dT2_dVd = -dT2_dVg * (dVth_dVd - 2.0 * Vtm * ExpArg * dn_dVd) + (T2 - 1.0) / n * dn_dVd;
            dT2_dVb = -dT2_dVg * (dVth_dVb - 2.0 * Vtm * ExpArg * dn_dVb) + (T2 - 1.0) / n * dn_dVb;

            Vgsteff = T1 / T2;
            T3 = T2 * T2;
            dVgsteff_dVg = (T2 * dT1_dVg - T1 * dT2_dVg) / T3 * dVgs_eff_dVg;
            dVgsteff_dVd = (T2 * dT1_dVd - T1 * dT2_dVd) / T3;
            dVgsteff_dVb = (T2 * dT1_dVb - T1 * dT2_dVb) / T3;
        }
        /* Added revision dependent code */
        if (model->BSIM3v32intVersion > BSIM3v32V323)
        {
            this->BSIM3v32Vgsteff = Vgsteff;
        }

        /* Calculate Effective Channel Geometry */
        T9 = sqrtPhis - pParam->BSIM3v32sqrtPhi;
        Weff = pParam->BSIM3v32weff - 2.0 * (pParam->BSIM3v32dwg * Vgsteff + pParam->BSIM3v32dwb * T9);
        dWeff_dVg = -2.0 * pParam->BSIM3v32dwg;
        dWeff_dVb = -2.0 * pParam->BSIM3v32dwb * dsqrtPhis_dVb;

        if (Weff < 2.0e-8) /* to avoid the discontinuity problem due to Weff*/
        {
            T0 = 1.0 / (6.0e-8 - 2.0 * Weff);
            Weff = 2.0e-8 * (4.0e-8 - Weff) * T0;
            T0 *= T0 * 4.0e-16;
            dWeff_dVg *= T0;
            dWeff_dVb *= T0;
        }

        T0 = pParam->BSIM3v32prwg * Vgsteff + pParam->BSIM3v32prwb * T9;
        if (T0 >= -0.9)
        {
            Rds = pParam->BSIM3v32rds0 * (1.0 + T0);
            dRds_dVg = pParam->BSIM3v32rds0 * pParam->BSIM3v32prwg;
            dRds_dVb = pParam->BSIM3v32rds0 * pParam->BSIM3v32prwb * dsqrtPhis_dVb;
        }
        else
        /* to avoid the discontinuity problem due to prwg and prwb*/
        {
            T1 = 1.0 / (17.0 + 20.0 * T0);
            Rds = pParam->BSIM3v32rds0 * (0.8 + T0) * T1;
            T1 *= T1;
            dRds_dVg = pParam->BSIM3v32rds0 * pParam->BSIM3v32prwg * T1;
            dRds_dVb = pParam->BSIM3v32rds0 * pParam->BSIM3v32prwb * dsqrtPhis_dVb * T1;
        }
        /* Added revision dependent code */
        if (model->BSIM3v32intVersion > BSIM3v32V323)
        {
            this->BSIM3v32rds = Rds; /* Noise Bugfix */
        }

        /* Calculate Abulk */
        T1 = 0.5 * pParam->BSIM3v32k1ox / sqrtPhis;
        dT1_dVb = -T1 / sqrtPhis * dsqrtPhis_dVb;

        T9 = sqrt(pParam->BSIM3v32xj * Xdep);
        tmp1 = Leff + 2.0 * T9;
        T5 = Leff / tmp1;
        tmp2 = pParam->BSIM3v32a0 * T5;
        tmp3 = pParam->BSIM3v32weff + pParam->BSIM3v32b1;
        tmp4 = pParam->BSIM3v32b0 / tmp3;
        T2 = tmp2 + tmp4;
        dT2_dVb = -T9 / tmp1 / Xdep * dXdep_dVb;
        T6 = T5 * T5;
        T7 = T5 * T6;

        Abulk0 = 1.0 + T1 * T2;
        dAbulk0_dVb = T1 * tmp2 * dT2_dVb + T2 * dT1_dVb;

        T8 = pParam->BSIM3v32ags * pParam->BSIM3v32a0 * T7;
        dAbulk_dVg = -T1 * T8;
        Abulk = Abulk0 + dAbulk_dVg * Vgsteff;
        dAbulk_dVb = dAbulk0_dVb - T8 * Vgsteff * (dT1_dVb + 3.0 * T1 * dT2_dVb);

        if (Abulk0 < 0.1) /* added to avoid the problems caused by Abulk0 */
        {
            T9 = 1.0 / (3.0 - 20.0 * Abulk0);
            Abulk0 = (0.2 - Abulk0) * T9;
            dAbulk0_dVb *= T9 * T9;
        }

        if (Abulk < 0.1)
        /* added to avoid the problems caused by Abulk */
        {
            T9 = 1.0 / (3.0 - 20.0 * Abulk);
            Abulk = (0.2 - Abulk) * T9;
            /* Added revision dependent code */
            if (model->BSIM3v32intVersion > BSIM3v32V32)
            {
                T10 = T9 * T9;
                dAbulk_dVb *= T10;
                dAbulk_dVg *= T10;
            }
            else
            {
                dAbulk_dVb *= T9 * T9;
            }
        }
        /* Added revision dependent code */
        if (model->BSIM3v32intVersion > BSIM3v32V323)
        {
            this->BSIM3v32Abulk = Abulk;
        }

        T2 = pParam->BSIM3v32keta * Vbseff;
        if (T2 >= -0.9)
        {
            T0 = 1.0 / (1.0 + T2);
            dT0_dVb = -pParam->BSIM3v32keta * T0 * T0;
        }
        else
        /* added to avoid the problems caused by Keta */
        {
            T1 = 1.0 / (0.8 + T2);
            T0 = (17.0 + 20.0 * T2) * T1;
            dT0_dVb = -pParam->BSIM3v32keta * T1 * T1;
        }
        dAbulk_dVg *= T0;
        dAbulk_dVb = dAbulk_dVb * T0 + Abulk * dT0_dVb;
        dAbulk0_dVb = dAbulk0_dVb * T0 + Abulk0 * dT0_dVb;
        Abulk *= T0;
        Abulk0 *= T0;

        /* Mobility calculation */
        if (model->BSIM3v32mobMod == 1)
        {
            T0 = Vgsteff + Vth + Vth;
            T2 = pParam->BSIM3v32ua + pParam->BSIM3v32uc * Vbseff;
            T3 = T0 / model->BSIM3v32tox;
            T5 = T3 * (T2 + pParam->BSIM3v32ub * T3);
            dDenomi_dVg = (T2 + 2.0 * pParam->BSIM3v32ub * T3) / model->BSIM3v32tox;
            dDenomi_dVd = dDenomi_dVg * 2.0 * dVth_dVd;
            dDenomi_dVb = dDenomi_dVg * 2.0 * dVth_dVb + pParam->BSIM3v32uc * T3;
        }
        else if (model->BSIM3v32mobMod == 2)
        {
            T5 = Vgsteff / model->BSIM3v32tox * (pParam->BSIM3v32ua + pParam->BSIM3v32uc * Vbseff + pParam->BSIM3v32ub * Vgsteff / model->BSIM3v32tox);
            dDenomi_dVg = (pParam->BSIM3v32ua + pParam->BSIM3v32uc * Vbseff + 2.0 * pParam->BSIM3v32ub * Vgsteff / model->BSIM3v32tox) / model->BSIM3v32tox;
            dDenomi_dVd = 0.0;
            dDenomi_dVb = Vgsteff * pParam->BSIM3v32uc / model->BSIM3v32tox;
        }
        else
        {
            T0 = Vgsteff + Vth + Vth;
            T2 = 1.0 + pParam->BSIM3v32uc * Vbseff;
            T3 = T0 / model->BSIM3v32tox;
            T4 = T3 * (pParam->BSIM3v32ua + pParam->BSIM3v32ub * T3);
            T5 = T4 * T2;
            dDenomi_dVg = (pParam->BSIM3v32ua + 2.0 * pParam->BSIM3v32ub * T3) * T2 / model->BSIM3v32tox;
            dDenomi_dVd = dDenomi_dVg * 2.0 * dVth_dVd;
            dDenomi_dVb = dDenomi_dVg * 2.0 * dVth_dVb + pParam->BSIM3v32uc * T4;
        }

        if (T5 >= -0.8)
        {
            Denomi = 1.0 + T5;
        }
        else /* Added to avoid the discontinuity problem caused by ua and ub*/
        {
            T9 = 1.0 / (7.0 + 10.0 * T5);
            Denomi = (0.6 + T5) * T9;
            T9 *= T9;
            dDenomi_dVg *= T9;
            dDenomi_dVd *= T9;
            dDenomi_dVb *= T9;
        }

        this->BSIM3v32ueff = ueff = this->BSIM3v32u0temp / Denomi;
        T9 = -ueff / Denomi;
        dueff_dVg = T9 * dDenomi_dVg;
        dueff_dVd = T9 * dDenomi_dVd;
        dueff_dVb = T9 * dDenomi_dVb;

        /* Saturation Drain Voltage  Vdsat */
        WVCox = Weff * pParam->BSIM3v32vsattemp * model->BSIM3v32cox;
        WVCoxRds = WVCox * Rds;

        Esat = 2.0 * pParam->BSIM3v32vsattemp / ueff;
        EsatL = Esat * Leff;
        T0 = -EsatL / ueff;
        dEsatL_dVg = T0 * dueff_dVg;
        dEsatL_dVd = T0 * dueff_dVd;
        dEsatL_dVb = T0 * dueff_dVb;

        /* Sqrt() */
        a1 = pParam->BSIM3v32a1;
        if (a1 == 0.0)
        {
            Lambda = pParam->BSIM3v32a2;
            dLambda_dVg = 0.0;
        }
        else if (a1 > 0.0)
        /* Added to avoid the discontinuity problem
   caused by a1 and a2 (Lambda) */
        {
            T0 = 1.0 - pParam->BSIM3v32a2;
            T1 = T0 - pParam->BSIM3v32a1 * Vgsteff - 0.0001;
            T2 = sqrt(T1 * T1 + 0.0004 * T0);
            Lambda = pParam->BSIM3v32a2 + T0 - 0.5 * (T1 + T2);
            dLambda_dVg = 0.5 * pParam->BSIM3v32a1 * (1.0 + T1 / T2);
        }
        else
        {
            T1 = pParam->BSIM3v32a2 + pParam->BSIM3v32a1 * Vgsteff - 0.0001;
            T2 = sqrt(T1 * T1 + 0.0004 * pParam->BSIM3v32a2);
            Lambda = 0.5 * (T1 + T2);
            dLambda_dVg = 0.5 * pParam->BSIM3v32a1 * (1.0 + T1 / T2);
        }

        Vgst2Vtm = Vgsteff + 2.0 * Vtm;
        /* Added revision dependent code */
        if (model->BSIM3v32intVersion > BSIM3v32V323)
        {
            this->BSIM3v32AbovVgst2Vtm = Abulk / Vgst2Vtm;
        }

        if (Rds > 0)
        {
            tmp2 = dRds_dVg / Rds + dWeff_dVg / Weff;
            tmp3 = dRds_dVb / Rds + dWeff_dVb / Weff;
        }
        else
        {
            tmp2 = dWeff_dVg / Weff;
            tmp3 = dWeff_dVb / Weff;
        }
        if ((Rds == 0.0) && (Lambda == 1.0))
        {
            T0 = 1.0 / (Abulk * EsatL + Vgst2Vtm);
            tmp1 = 0.0;
            T1 = T0 * T0;
            T2 = Vgst2Vtm * T0;
            T3 = EsatL * Vgst2Vtm;
            Vdsat = T3 * T0;

            dT0_dVg = -(Abulk * dEsatL_dVg + EsatL * dAbulk_dVg + 1.0) * T1;
            dT0_dVd = -(Abulk * dEsatL_dVd) * T1;
            dT0_dVb = -(Abulk * dEsatL_dVb + dAbulk_dVb * EsatL) * T1;

            dVdsat_dVg = T3 * dT0_dVg + T2 * dEsatL_dVg + EsatL * T0;
            dVdsat_dVd = T3 * dT0_dVd + T2 * dEsatL_dVd;
            dVdsat_dVb = T3 * dT0_dVb + T2 * dEsatL_dVb;
        }
        else
        {
            tmp1 = dLambda_dVg / (Lambda * Lambda);
            T9 = Abulk * WVCoxRds;
            T8 = Abulk * T9;
            T7 = Vgst2Vtm * T9;
            T6 = Vgst2Vtm * WVCoxRds;
            T0 = 2.0 * Abulk * (T9 - 1.0 + 1.0 / Lambda);
            dT0_dVg = 2.0 * (T8 * tmp2 - Abulk * tmp1 + (2.0 * T9 + 1.0 / Lambda - 1.0) * dAbulk_dVg);

            dT0_dVb = 2.0 * (T8 * (2.0 / Abulk * dAbulk_dVb + tmp3) + (1.0 / Lambda - 1.0) * dAbulk_dVb);
            dT0_dVd = 0.0;
            T1 = Vgst2Vtm * (2.0 / Lambda - 1.0) + Abulk * EsatL + 3.0 * T7;

            dT1_dVg = (2.0 / Lambda - 1.0) - 2.0 * Vgst2Vtm * tmp1 + Abulk * dEsatL_dVg + EsatL * dAbulk_dVg + 3.0 * (T9 + T7 * tmp2 + T6 * dAbulk_dVg);
            dT1_dVb = Abulk * dEsatL_dVb + EsatL * dAbulk_dVb + 3.0 * (T6 * dAbulk_dVb + T7 * tmp3);
            dT1_dVd = Abulk * dEsatL_dVd;

            T2 = Vgst2Vtm * (EsatL + 2.0 * T6);
            dT2_dVg = EsatL + Vgst2Vtm * dEsatL_dVg + T6 * (4.0 + 2.0 * Vgst2Vtm * tmp2);
            dT2_dVb = Vgst2Vtm * (dEsatL_dVb + 2.0 * T6 * tmp3);
            dT2_dVd = Vgst2Vtm * dEsatL_dVd;

            T3 = sqrt(T1 * T1 - 2.0 * T0 * T2);
            Vdsat = (T1 - T3) / T0;

            dT3_dVg = (T1 * dT1_dVg - 2.0 * (T0 * dT2_dVg + T2 * dT0_dVg)) / T3;
            dT3_dVd = (T1 * dT1_dVd - 2.0 * (T0 * dT2_dVd + T2 * dT0_dVd)) / T3;
            dT3_dVb = (T1 * dT1_dVb - 2.0 * (T0 * dT2_dVb + T2 * dT0_dVb)) / T3;

            dVdsat_dVg = (dT1_dVg - (T1 * dT1_dVg - dT0_dVg * T2 - T0 * dT2_dVg) / T3 - Vdsat * dT0_dVg) / T0;
            dVdsat_dVb = (dT1_dVb - (T1 * dT1_dVb - dT0_dVb * T2 - T0 * dT2_dVb) / T3 - Vdsat * dT0_dVb) / T0;
            dVdsat_dVd = (dT1_dVd - (T1 * dT1_dVd - T0 * dT2_dVd) / T3) / T0;
        }
        this->BSIM3v32vdsat = Vdsat;

        /* Effective Vds (Vdseff) Calculation */
        T1 = Vdsat - Vds - pParam->BSIM3v32delta;
        dT1_dVg = dVdsat_dVg;
        dT1_dVd = dVdsat_dVd - 1.0;
        dT1_dVb = dVdsat_dVb;

        T2 = sqrt(T1 * T1 + 4.0 * pParam->BSIM3v32delta * Vdsat);
        T0 = T1 / T2;
        T3 = 2.0 * pParam->BSIM3v32delta / T2;
        dT2_dVg = T0 * dT1_dVg + T3 * dVdsat_dVg;
        dT2_dVd = T0 * dT1_dVd + T3 * dVdsat_dVd;
        dT2_dVb = T0 * dT1_dVb + T3 * dVdsat_dVb;

        Vdseff = Vdsat - 0.5 * (T1 + T2);
        dVdseff_dVg = dVdsat_dVg - 0.5 * (dT1_dVg + dT2_dVg);
        dVdseff_dVd = dVdsat_dVd - 0.5 * (dT1_dVd + dT2_dVd);
        dVdseff_dVb = dVdsat_dVb - 0.5 * (dT1_dVb + dT2_dVb);
        /* Added revision dependent code */
        switch (model->BSIM3v32intVersion)
        {
        case BSIM3v32V324:
        case BSIM3v32V323:
        case BSIM3v32V322:
            /* Added to eliminate non-zero Vdseff at Vds=0.0 */
            if (Vds == 0.0)
            {
                Vdseff = 0.0;
                dVdseff_dVg = 0.0;
                dVdseff_dVb = 0.0;
            }
            break;
        case BSIM3v32V32:
        default:
            /* Do nothing */
            break;
        }

        /* Calculate VAsat */
        tmp4 = 1.0 - 0.5 * Abulk * Vdsat / Vgst2Vtm;
        T9 = WVCoxRds * Vgsteff;
        T8 = T9 / Vgst2Vtm;
        T0 = EsatL + Vdsat + 2.0 * T9 * tmp4;

        T7 = 2.0 * WVCoxRds * tmp4;
        dT0_dVg = dEsatL_dVg + dVdsat_dVg + T7 * (1.0 + tmp2 * Vgsteff) - T8 * (Abulk * dVdsat_dVg - Abulk * Vdsat / Vgst2Vtm + Vdsat * dAbulk_dVg);

        dT0_dVb = dEsatL_dVb + dVdsat_dVb + T7 * tmp3 * Vgsteff - T8 * (dAbulk_dVb * Vdsat + Abulk * dVdsat_dVb);
        dT0_dVd = dEsatL_dVd + dVdsat_dVd - T8 * Abulk * dVdsat_dVd;

        T9 = WVCoxRds * Abulk;
        T1 = 2.0 / Lambda - 1.0 + T9;
        dT1_dVg = -2.0 * tmp1 + WVCoxRds * (Abulk * tmp2 + dAbulk_dVg);
        dT1_dVb = dAbulk_dVb * WVCoxRds + T9 * tmp3;

        Vasat = T0 / T1;
        dVasat_dVg = (dT0_dVg - Vasat * dT1_dVg) / T1;
        dVasat_dVb = (dT0_dVb - Vasat * dT1_dVb) / T1;
        dVasat_dVd = dT0_dVd / T1;

        if (Vdseff > Vds)
            Vdseff = Vds;
        diffVds = Vds - Vdseff;
        /* Added revision dependent code */
        if (model->BSIM3v32intVersion > BSIM3v32V323)
        {
            this->BSIM3v32Vdseff = Vdseff;
        }

        /* Calculate VACLM */
        if ((pParam->BSIM3v32pclm > 0.0) && (diffVds > 1.0e-10))
        {
            T0 = 1.0 / (pParam->BSIM3v32pclm * Abulk * pParam->BSIM3v32litl);
            dT0_dVb = -T0 / Abulk * dAbulk_dVb;
            dT0_dVg = -T0 / Abulk * dAbulk_dVg;

            T2 = Vgsteff / EsatL;
            T1 = Leff * (Abulk + T2);
            dT1_dVg = Leff * ((1.0 - T2 * dEsatL_dVg) / EsatL + dAbulk_dVg);
            dT1_dVb = Leff * (dAbulk_dVb - T2 * dEsatL_dVb / EsatL);
            dT1_dVd = -T2 * dEsatL_dVd / Esat;

            T9 = T0 * T1;
            VACLM = T9 * diffVds;
            dVACLM_dVg = T0 * dT1_dVg * diffVds - T9 * dVdseff_dVg + T1 * diffVds * dT0_dVg;
            dVACLM_dVb = (dT0_dVb * T1 + T0 * dT1_dVb) * diffVds - T9 * dVdseff_dVb;
            dVACLM_dVd = T0 * dT1_dVd * diffVds + T9 * (1.0 - dVdseff_dVd);
        }
        else
        {
            VACLM = MAX_EXP;
            dVACLM_dVd = dVACLM_dVg = dVACLM_dVb = 0.0;
        }

        /* Calculate VADIBL */
        if (pParam->BSIM3v32thetaRout > 0.0)
        {
            T8 = Abulk * Vdsat;
            T0 = Vgst2Vtm * T8;
            dT0_dVg = Vgst2Vtm * Abulk * dVdsat_dVg + T8 + Vgst2Vtm * Vdsat * dAbulk_dVg;
            dT0_dVb = Vgst2Vtm * (dAbulk_dVb * Vdsat + Abulk * dVdsat_dVb);
            dT0_dVd = Vgst2Vtm * Abulk * dVdsat_dVd;

            T1 = Vgst2Vtm + T8;
            dT1_dVg = 1.0 + Abulk * dVdsat_dVg + Vdsat * dAbulk_dVg;
            dT1_dVb = Abulk * dVdsat_dVb + dAbulk_dVb * Vdsat;
            dT1_dVd = Abulk * dVdsat_dVd;

            T9 = T1 * T1;
            T2 = pParam->BSIM3v32thetaRout;
            VADIBL = (Vgst2Vtm - T0 / T1) / T2;
            dVADIBL_dVg = (1.0 - dT0_dVg / T1 + T0 * dT1_dVg / T9) / T2;
            dVADIBL_dVb = (-dT0_dVb / T1 + T0 * dT1_dVb / T9) / T2;
            dVADIBL_dVd = (-dT0_dVd / T1 + T0 * dT1_dVd / T9) / T2;

            T7 = pParam->BSIM3v32pdiblb * Vbseff;
            if (T7 >= -0.9)
            {
                T3 = 1.0 / (1.0 + T7);
                VADIBL *= T3;
                dVADIBL_dVg *= T3;
                dVADIBL_dVb = (dVADIBL_dVb - VADIBL * pParam->BSIM3v32pdiblb) * T3;
                dVADIBL_dVd *= T3;
            }
            else
            /* Added to avoid the discontinuity problem caused by pdiblcb */
            {
                T4 = 1.0 / (0.8 + T7);
                T3 = (17.0 + 20.0 * T7) * T4;
                dVADIBL_dVg *= T3;
                dVADIBL_dVb = dVADIBL_dVb * T3 - VADIBL * pParam->BSIM3v32pdiblb * T4 * T4;
                dVADIBL_dVd *= T3;
                VADIBL *= T3;
            }
        }
        else
        {
            VADIBL = MAX_EXP;
            dVADIBL_dVd = dVADIBL_dVg = dVADIBL_dVb = 0.0;
        }

        /* Calculate VA */

        T8 = pParam->BSIM3v32pvag / EsatL;
        T9 = T8 * Vgsteff;
        if (T9 > -0.9)
        {
            T0 = 1.0 + T9;
            dT0_dVg = T8 * (1.0 - Vgsteff * dEsatL_dVg / EsatL);
            dT0_dVb = -T9 * dEsatL_dVb / EsatL;
            dT0_dVd = -T9 * dEsatL_dVd / EsatL;
        }
        else /* Added to avoid the discontinuity problems caused by pvag */
        {
            T1 = 1.0 / (17.0 + 20.0 * T9);
            T0 = (0.8 + T9) * T1;
            T1 *= T1;
            dT0_dVg = T8 * (1.0 - Vgsteff * dEsatL_dVg / EsatL) * T1;

            T9 *= T1 / EsatL;
            dT0_dVb = -T9 * dEsatL_dVb;
            dT0_dVd = -T9 * dEsatL_dVd;
        }

        tmp1 = VACLM * VACLM;
        tmp2 = VADIBL * VADIBL;
        tmp3 = VACLM + VADIBL;

        T1 = VACLM * VADIBL / tmp3;
        tmp3 *= tmp3;
        dT1_dVg = (tmp1 * dVADIBL_dVg + tmp2 * dVACLM_dVg) / tmp3;
        dT1_dVd = (tmp1 * dVADIBL_dVd + tmp2 * dVACLM_dVd) / tmp3;
        dT1_dVb = (tmp1 * dVADIBL_dVb + tmp2 * dVACLM_dVb) / tmp3;

        Va = Vasat + T0 * T1;
        dVa_dVg = dVasat_dVg + T1 * dT0_dVg + T0 * dT1_dVg;
        dVa_dVd = dVasat_dVd + T1 * dT0_dVd + T0 * dT1_dVd;
        dVa_dVb = dVasat_dVb + T1 * dT0_dVb + T0 * dT1_dVb;

        /* Calculate VASCBE */
        if (pParam->BSIM3v32pscbe2 > 0.0)
        {
            if (diffVds > pParam->BSIM3v32pscbe1 * pParam->BSIM3v32litl / EXP_THRESHOLD)
            {
                T0 = pParam->BSIM3v32pscbe1 * pParam->BSIM3v32litl / diffVds;
                VASCBE = Leff * exp(T0) / pParam->BSIM3v32pscbe2;
                T1 = T0 * VASCBE / diffVds;
                dVASCBE_dVg = T1 * dVdseff_dVg;
                dVASCBE_dVd = -T1 * (1.0 - dVdseff_dVd);
                dVASCBE_dVb = T1 * dVdseff_dVb;
            }
            else
            {
                VASCBE = MAX_EXP * Leff / pParam->BSIM3v32pscbe2;
                dVASCBE_dVg = dVASCBE_dVd = dVASCBE_dVb = 0.0;
            }
        }
        else
        {
            VASCBE = MAX_EXP;
            dVASCBE_dVg = dVASCBE_dVd = dVASCBE_dVb = 0.0;
        }

        /* Calculate Ids */
        CoxWovL = model->BSIM3v32cox * Weff / Leff;
        beta = ueff * CoxWovL;
        dbeta_dVg = CoxWovL * dueff_dVg + beta * dWeff_dVg / Weff;
        dbeta_dVd = CoxWovL * dueff_dVd;
        dbeta_dVb = CoxWovL * dueff_dVb + beta * dWeff_dVb / Weff;

        T0 = 1.0 - 0.5 * Abulk * Vdseff / Vgst2Vtm;
        dT0_dVg = -0.5 * (Abulk * dVdseff_dVg - Abulk * Vdseff / Vgst2Vtm + Vdseff * dAbulk_dVg) / Vgst2Vtm;
        dT0_dVd = -0.5 * Abulk * dVdseff_dVd / Vgst2Vtm;
        dT0_dVb = -0.5 * (Abulk * dVdseff_dVb + dAbulk_dVb * Vdseff) / Vgst2Vtm;

        fgche1 = Vgsteff * T0;
        dfgche1_dVg = Vgsteff * dT0_dVg + T0;
        dfgche1_dVd = Vgsteff * dT0_dVd;
        dfgche1_dVb = Vgsteff * dT0_dVb;

        T9 = Vdseff / EsatL;
        fgche2 = 1.0 + T9;
        dfgche2_dVg = (dVdseff_dVg - T9 * dEsatL_dVg) / EsatL;
        dfgche2_dVd = (dVdseff_dVd - T9 * dEsatL_dVd) / EsatL;
        dfgche2_dVb = (dVdseff_dVb - T9 * dEsatL_dVb) / EsatL;

        gche = beta * fgche1 / fgche2;
        dgche_dVg = (beta * dfgche1_dVg + fgche1 * dbeta_dVg - gche * dfgche2_dVg) / fgche2;
        dgche_dVd = (beta * dfgche1_dVd + fgche1 * dbeta_dVd - gche * dfgche2_dVd) / fgche2;
        dgche_dVb = (beta * dfgche1_dVb + fgche1 * dbeta_dVb - gche * dfgche2_dVb) / fgche2;

        T0 = 1.0 + gche * Rds;
        T9 = Vdseff / T0;
        Idl = gche * T9;

        dIdl_dVg = (gche * dVdseff_dVg + T9 * dgche_dVg) / T0 - Idl * gche / T0 * dRds_dVg;

        dIdl_dVd = (gche * dVdseff_dVd + T9 * dgche_dVd) / T0;
        dIdl_dVb = (gche * dVdseff_dVb + T9 * dgche_dVb - Idl * dRds_dVb * gche) / T0;

        T9 = diffVds / Va;
        T0 = 1.0 + T9;
        Idsa = Idl * T0;
        dIdsa_dVg = T0 * dIdl_dVg - Idl * (dVdseff_dVg + T9 * dVa_dVg) / Va;
        dIdsa_dVd = T0 * dIdl_dVd + Idl * (1.0 - dVdseff_dVd - T9 * dVa_dVd) / Va;
        dIdsa_dVb = T0 * dIdl_dVb - Idl * (dVdseff_dVb + T9 * dVa_dVb) / Va;

        T9 = diffVds / VASCBE;
        T0 = 1.0 + T9;
        Ids = Idsa * T0;

        Gm = T0 * dIdsa_dVg - Idsa * (dVdseff_dVg + T9 * dVASCBE_dVg) / VASCBE;
        Gds = T0 * dIdsa_dVd + Idsa * (1.0 - dVdseff_dVd - T9 * dVASCBE_dVd) / VASCBE;
        Gmb = T0 * dIdsa_dVb - Idsa * (dVdseff_dVb + T9 * dVASCBE_dVb) / VASCBE;

        Gds += Gm * dVgsteff_dVd;
        Gmb += Gm * dVgsteff_dVb;
        Gm *= dVgsteff_dVg;
        Gmb *= dVbseff_dVb;

        /* Substrate current begins */
        tmp = pParam->BSIM3v32alpha0 + pParam->BSIM3v32alpha1 * Leff;
        if ((tmp <= 0.0) || (pParam->BSIM3v32beta0 <= 0.0))
        {
            Isub = Gbd = Gbb = Gbg = 0.0;
        }
        else
        {
            T2 = tmp / Leff;
            if (diffVds > pParam->BSIM3v32beta0 / EXP_THRESHOLD)
            {
                T0 = -pParam->BSIM3v32beta0 / diffVds;
                T1 = T2 * diffVds * exp(T0);
                T3 = T1 / diffVds * (T0 - 1.0);
                dT1_dVg = T3 * dVdseff_dVg;
                dT1_dVd = T3 * (dVdseff_dVd - 1.0);
                dT1_dVb = T3 * dVdseff_dVb;
            }
            else
            {
                T3 = T2 * MIN_EXP;
                T1 = T3 * diffVds;
                dT1_dVg = -T3 * dVdseff_dVg;
                dT1_dVd = T3 * (1.0 - dVdseff_dVd);
                dT1_dVb = -T3 * dVdseff_dVb;
            }
            Isub = T1 * Idsa;
            Gbg = T1 * dIdsa_dVg + Idsa * dT1_dVg;
            Gbd = T1 * dIdsa_dVd + Idsa * dT1_dVd;
            Gbb = T1 * dIdsa_dVb + Idsa * dT1_dVb;

            Gbd += Gbg * dVgsteff_dVd;
            Gbb += Gbg * dVgsteff_dVb;
            Gbg *= dVgsteff_dVg;
            Gbb *= dVbseff_dVb; /* bug fixing */
        }

        cdrain = Ids;
        this->BSIM3v32gds = Gds;
        this->BSIM3v32gm = Gm;
        this->BSIM3v32gmbs = Gmb;

        this->BSIM3v32gbbs = Gbb;
        this->BSIM3v32gbgs = Gbg;
        this->BSIM3v32gbds = Gbd;

        this->BSIM3v32csub = Isub;

        /* BSIM3v32 thermal noise Qinv calculated from all capMod
           * 0, 1, 2 & 3 stored in this->BSIM3v32qinv 1/1998 */

        if ((model->BSIM3v32xpart < 0) || (!ChargeComputationNeeded))
        {
            qgate = qdrn = qsrc = qbulk = 0.0;
            this->BSIM3v32cggb = this->BSIM3v32cgsb = this->BSIM3v32cgdb = 0.0;
            this->BSIM3v32cdgb = this->BSIM3v32cdsb = this->BSIM3v32cddb = 0.0;
            this->BSIM3v32cbgb = this->BSIM3v32cbsb = this->BSIM3v32cbdb = 0.0;
            this->BSIM3v32cqdb = this->BSIM3v32cqsb = this->BSIM3v32cqgb = this->BSIM3v32cqbb = 0.0;
            this->BSIM3v32gtau = 0.0;
            goto finished;
        }
        else if (model->BSIM3v32capMod == 0)
        {
            if (Vbseff < 0.0)
            {
                Vbseff = Vbs;
                dVbseff_dVb = 1.0;
            }
            else
            {
                Vbseff = pParam->BSIM3v32phi - Phis;
                dVbseff_dVb = -dPhis_dVb;
            }

            Vfb = pParam->BSIM3v32vfbcv;
            Vth = Vfb + pParam->BSIM3v32phi + pParam->BSIM3v32k1ox * sqrtPhis;
            Vgst = Vgs_eff - Vth;
            dVth_dVb = pParam->BSIM3v32k1ox * dsqrtPhis_dVb;
            dVgst_dVb = -dVth_dVb;
            dVgst_dVg = dVgs_eff_dVg;

            CoxWL = model->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV;
            Arg1 = Vgs_eff - Vbseff - Vfb;

            if (Arg1 <= 0.0)
            {
                qgate = CoxWL * Arg1;
                qbulk = -qgate;
                qdrn = 0.0;

                this->BSIM3v32cggb = CoxWL * dVgs_eff_dVg;
                this->BSIM3v32cgdb = 0.0;
                this->BSIM3v32cgsb = CoxWL * (dVbseff_dVb - dVgs_eff_dVg);

                this->BSIM3v32cdgb = 0.0;
                this->BSIM3v32cddb = 0.0;
                this->BSIM3v32cdsb = 0.0;

                this->BSIM3v32cbgb = -CoxWL * dVgs_eff_dVg;
                this->BSIM3v32cbdb = 0.0;
                this->BSIM3v32cbsb = -this->BSIM3v32cgsb;
                this->BSIM3v32qinv = 0.0;
            }
            else if (Vgst <= 0.0)
            {
                T1 = 0.5 * pParam->BSIM3v32k1ox;
                T2 = sqrt(T1 * T1 + Arg1);
                qgate = CoxWL * pParam->BSIM3v32k1ox * (T2 - T1);
                qbulk = -qgate;
                qdrn = 0.0;

                T0 = CoxWL * T1 / T2;
                this->BSIM3v32cggb = T0 * dVgs_eff_dVg;
                this->BSIM3v32cgdb = 0.0;
                this->BSIM3v32cgsb = T0 * (dVbseff_dVb - dVgs_eff_dVg);

                this->BSIM3v32cdgb = 0.0;
                this->BSIM3v32cddb = 0.0;
                this->BSIM3v32cdsb = 0.0;

                this->BSIM3v32cbgb = -this->BSIM3v32cggb;
                this->BSIM3v32cbdb = 0.0;
                this->BSIM3v32cbsb = -this->BSIM3v32cgsb;
                this->BSIM3v32qinv = 0.0;
            }
            else
            {
                One_Third_CoxWL = CoxWL / 3.0;
                Two_Third_CoxWL = 2.0 * One_Third_CoxWL;

                AbulkCV = Abulk0 * pParam->BSIM3v32abulkCVfactor;
                dAbulkCV_dVb = pParam->BSIM3v32abulkCVfactor * dAbulk0_dVb;
                Vdsat = Vgst / AbulkCV;
                dVdsat_dVg = dVgs_eff_dVg / AbulkCV;
                dVdsat_dVb = -(Vdsat * dAbulkCV_dVb + dVth_dVb) / AbulkCV;

                if (model->BSIM3v32xpart > 0.5)
                { /* 0/100 Charge partition model */
                    if (Vdsat <= Vds)
                    { /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.0;

                        this->BSIM3v32cggb = One_Third_CoxWL * (3.0 - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + T2);
                        this->BSIM3v32cgdb = 0.0;

                        this->BSIM3v32cdgb = 0.0;
                        this->BSIM3v32cddb = 0.0;
                        this->BSIM3v32cdsb = 0.0;

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        this->BSIM3v32cbsb = -(this->BSIM3v32cbgb + T3);
                        this->BSIM3v32cbdb = 0.0;
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                    else
                    { /* linear region */
                        Alphaz = Vgst / Vdsat;
                        T1 = 2.0 * Vdsat - Vds;
                        T2 = Vds / (3.0 * T1);
                        T3 = T2 * Vds;
                        T9 = 0.25 * CoxWL;
                        T4 = T9 * Alphaz;
                        T7 = 2.0 * Vds - T1 - 3.0 * T3;
                        T8 = T3 - T1 - 2.0 * Vds;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - 0.5 * (Vds - T3));
                        T10 = T4 * T8;
                        qdrn = T4 * T7;
                        qbulk = -(qgate + qdrn + T10);

                        T5 = T3 / T1;
                        this->BSIM3v32cggb = CoxWL * (1.0 - T5 * dVdsat_dVg) * dVgs_eff_dVg;
                        T11 = -CoxWL * T5 * dVdsat_dVb;
                        this->BSIM3v32cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + T11 + this->BSIM3v32cgdb);
                        T6 = 1.0 / Vdsat;
                        dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                        dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);
                        T7 = T9 * T7;
                        T8 = T9 * T8;
                        T9 = 2.0 * T4 * (1.0 - 3.0 * T5);
                        this->BSIM3v32cdgb = (T7 * dAlphaz_dVg - T9 * dVdsat_dVg) * dVgs_eff_dVg;
                        T12 = T7 * dAlphaz_dVb - T9 * dVdsat_dVb;
                        this->BSIM3v32cddb = T4 * (3.0 - 6.0 * T2 - 3.0 * T5);
                        this->BSIM3v32cdsb = -(this->BSIM3v32cdgb + T12 + this->BSIM3v32cddb);

                        T9 = 2.0 * T4 * (1.0 + T5);
                        T10 = (T8 * dAlphaz_dVg - T9 * dVdsat_dVg) * dVgs_eff_dVg;
                        T11 = T8 * dAlphaz_dVb - T9 * dVdsat_dVb;
                        T12 = T4 * (2.0 * T2 + T5 - 1.0);
                        T0 = -(T10 + T11 + T12);

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + T10);
                        this->BSIM3v32cbdb = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + T12);
                        this->BSIM3v32cbsb = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + T0);
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                }
                else if (model->BSIM3v32xpart < 0.5)
                { /* 40/60 Charge partition model */
                    if (Vds >= Vdsat)
                    { /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.4 * T2;

                        this->BSIM3v32cggb = One_Third_CoxWL * (3.0 - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + T2);
                        this->BSIM3v32cgdb = 0.0;

                        T3 = 0.4 * Two_Third_CoxWL;
                        this->BSIM3v32cdgb = -T3 * dVgs_eff_dVg;
                        this->BSIM3v32cddb = 0.0;
                        T4 = T3 * dVth_dVb;
                        this->BSIM3v32cdsb = -(T4 + this->BSIM3v32cdgb);

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        this->BSIM3v32cbsb = -(this->BSIM3v32cbgb + T3);
                        this->BSIM3v32cbdb = 0.0;
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                    else
                    { /* linear region  */
                        Alphaz = Vgst / Vdsat;
                        T1 = 2.0 * Vdsat - Vds;
                        T2 = Vds / (3.0 * T1);
                        T3 = T2 * Vds;
                        T9 = 0.25 * CoxWL;
                        T4 = T9 * Alphaz;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - 0.5 * (Vds - T3));

                        T5 = T3 / T1;
                        this->BSIM3v32cggb = CoxWL * (1.0 - T5 * dVdsat_dVg) * dVgs_eff_dVg;
                        tmp = -CoxWL * T5 * dVdsat_dVb;
                        this->BSIM3v32cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + this->BSIM3v32cgdb + tmp);

                        T6 = 1.0 / Vdsat;
                        dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                        dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);

                        T6 = 8.0 * Vdsat * Vdsat - 6.0 * Vdsat * Vds + 1.2 * Vds * Vds;
                        T8 = T2 / T1;
                        T7 = Vds - T1 - T8 * T6;
                        qdrn = T4 * T7;
                        T7 *= T9;
                        tmp = T8 / T1;
                        tmp1 = T4 * (2.0 - 4.0 * tmp * T6 + T8 * (16.0 * Vdsat - 6.0 * Vds));

                        this->BSIM3v32cdgb = (T7 * dAlphaz_dVg - tmp1 * dVdsat_dVg) * dVgs_eff_dVg;
                        T10 = T7 * dAlphaz_dVb - tmp1 * dVdsat_dVb;
                        this->BSIM3v32cddb = T4 * (2.0 - (1.0 / (3.0 * T1 * T1) + 2.0 * tmp) * T6 + T8 * (6.0 * Vdsat - 2.4 * Vds));
                        this->BSIM3v32cdsb = -(this->BSIM3v32cdgb + T10 + this->BSIM3v32cddb);

                        T7 = 2.0 * (T1 + T3);
                        qbulk = -(qgate - T4 * T7);
                        T7 *= T9;
                        T0 = 4.0 * T4 * (1.0 - T5);
                        T12 = (-T7 * dAlphaz_dVg - this->BSIM3v32cdgb - T0 * dVdsat_dVg) * dVgs_eff_dVg;
                        T11 = -T7 * dAlphaz_dVb - T10 - T0 * dVdsat_dVb;
                        T10 = -4.0 * T4 * (T2 - 0.5 + 0.5 * T5) - this->BSIM3v32cddb;
                        tmp = -(T10 + T11 + T12);

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + T12);
                        this->BSIM3v32cbdb = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + T10); /* bug fix */
                        this->BSIM3v32cbsb = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + tmp);
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                }
                else
                { /* 50/50 partitioning */
                    if (Vds >= Vdsat)
                    { /* saturation region */
                        T1 = Vdsat / 3.0;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - T1);
                        T2 = -Two_Third_CoxWL * Vgst;
                        qbulk = -(qgate + T2);
                        qdrn = 0.5 * T2;

                        this->BSIM3v32cggb = One_Third_CoxWL * (3.0 - dVdsat_dVg) * dVgs_eff_dVg;
                        T2 = -One_Third_CoxWL * dVdsat_dVb;
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + T2);
                        this->BSIM3v32cgdb = 0.0;

                        this->BSIM3v32cdgb = -One_Third_CoxWL * dVgs_eff_dVg;
                        this->BSIM3v32cddb = 0.0;
                        T4 = One_Third_CoxWL * dVth_dVb;
                        this->BSIM3v32cdsb = -(T4 + this->BSIM3v32cdgb);

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb - Two_Third_CoxWL * dVgs_eff_dVg);
                        T3 = -(T2 + Two_Third_CoxWL * dVth_dVb);
                        this->BSIM3v32cbsb = -(this->BSIM3v32cbgb + T3);
                        this->BSIM3v32cbdb = 0.0;
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                    else
                    { /* linear region */
                        Alphaz = Vgst / Vdsat;
                        T1 = 2.0 * Vdsat - Vds;
                        T2 = Vds / (3.0 * T1);
                        T3 = T2 * Vds;
                        T9 = 0.25 * CoxWL;
                        T4 = T9 * Alphaz;
                        qgate = CoxWL * (Vgs_eff - Vfb - pParam->BSIM3v32phi - 0.5 * (Vds - T3));

                        T5 = T3 / T1;
                        this->BSIM3v32cggb = CoxWL * (1.0 - T5 * dVdsat_dVg) * dVgs_eff_dVg;
                        tmp = -CoxWL * T5 * dVdsat_dVb;
                        this->BSIM3v32cgdb = CoxWL * (T2 - 0.5 + 0.5 * T5);
                        this->BSIM3v32cgsb = -(this->BSIM3v32cggb + this->BSIM3v32cgdb + tmp);

                        T6 = 1.0 / Vdsat;
                        dAlphaz_dVg = T6 * (1.0 - Alphaz * dVdsat_dVg);
                        dAlphaz_dVb = -T6 * (dVth_dVb + Alphaz * dVdsat_dVb);

                        T7 = T1 + T3;
                        qdrn = -T4 * T7;
                        qbulk = -(qgate + qdrn + qdrn);
                        T7 *= T9;
                        T0 = T4 * (2.0 * T5 - 2.0);

                        this->BSIM3v32cdgb = (T0 * dVdsat_dVg - T7 * dAlphaz_dVg) * dVgs_eff_dVg;
                        T12 = T0 * dVdsat_dVb - T7 * dAlphaz_dVb;
                        this->BSIM3v32cddb = T4 * (1.0 - 2.0 * T2 - T5);
                        this->BSIM3v32cdsb = -(this->BSIM3v32cdgb + T12 + this->BSIM3v32cddb);

                        this->BSIM3v32cbgb = -(this->BSIM3v32cggb + 2.0 * this->BSIM3v32cdgb);
                        this->BSIM3v32cbdb = -(this->BSIM3v32cgdb + 2.0 * this->BSIM3v32cddb);
                        this->BSIM3v32cbsb = -(this->BSIM3v32cgsb + 2.0 * this->BSIM3v32cdsb);
                        this->BSIM3v32qinv = -(qgate + qbulk);
                    }
                }
            }
        }
        else
        {
            if (Vbseff < 0.0)
            {
                VbseffCV = Vbseff;
                dVbseffCV_dVb = 1.0;
            }
            else
            {
                VbseffCV = pParam->BSIM3v32phi - Phis;
                dVbseffCV_dVb = -dPhis_dVb;
            }

            CoxWL = model->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV;

            /* Seperate VgsteffCV with noff and voffcv */
            noff = n * pParam->BSIM3v32noff;
            dnoff_dVd = pParam->BSIM3v32noff * dn_dVd;
            dnoff_dVb = pParam->BSIM3v32noff * dn_dVb;
            T0 = Vtm * noff;
            voffcv = pParam->BSIM3v32voffcv;
            VgstNVt = (Vgst - voffcv) / T0;

            if (VgstNVt > EXP_THRESHOLD)
            {
                Vgsteff = Vgst - voffcv;
                dVgsteff_dVg = dVgs_eff_dVg;
                dVgsteff_dVd = -dVth_dVd;
                dVgsteff_dVb = -dVth_dVb;
            }
            else if (VgstNVt < -EXP_THRESHOLD)
            {
                Vgsteff = T0 * log(1.0 + MIN_EXP);
                dVgsteff_dVg = 0.0;
                dVgsteff_dVd = Vgsteff / noff;
                dVgsteff_dVb = dVgsteff_dVd * dnoff_dVb;
                dVgsteff_dVd *= dnoff_dVd;
            }
            else
            {
                ExpVgst = exp(VgstNVt);
                Vgsteff = T0 * log(1.0 + ExpVgst);
                dVgsteff_dVg = ExpVgst / (1.0 + ExpVgst);
                dVgsteff_dVd = -dVgsteff_dVg * (dVth_dVd + (Vgst - voffcv) / noff * dnoff_dVd) + Vgsteff / noff * dnoff_dVd;
                dVgsteff_dVb = -dVgsteff_dVg * (dVth_dVb + (Vgst - voffcv) / noff * dnoff_dVb) + Vgsteff / noff * dnoff_dVb;
                dVgsteff_dVg *= dVgs_eff_dVg;
            } /* End of VgsteffCV */

            if (model->BSIM3v32capMod == 1)
            {
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    Vfb = this->BSIM3v32vfbzb;
                    break;
                case BSIM3v32V32:
                    Vfb = this->BSIM3v32vfbzb;
                    dVfb_dVb = dVfb_dVd = 0.0;
                    break;
                default:
                    Vfb = Vth - pParam->BSIM3v32phi - pParam->BSIM3v32k1ox * sqrtPhis;
                    dVfb_dVb = dVth_dVb - pParam->BSIM3v32k1ox * dsqrtPhis_dVb;
                    dVfb_dVd = dVth_dVd;
                }

                Arg1 = Vgs_eff - VbseffCV - Vfb - Vgsteff;

                if (Arg1 <= 0.0)
                {
                    qgate = CoxWL * Arg1;
                    Cgg = CoxWL * (dVgs_eff_dVg - dVgsteff_dVg);
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        Cgd = -CoxWL * dVgsteff_dVd;
                        Cgb = -CoxWL * (dVbseffCV_dVb + dVgsteff_dVb);
                        break;
                    case BSIM3v32V32:
                    default:
                        Cgd = -CoxWL * (dVfb_dVd + dVgsteff_dVd);
                        Cgb = -CoxWL * (dVfb_dVb + dVbseffCV_dVb + dVgsteff_dVb);
                    }
                }
                else
                {
                    T0 = 0.5 * pParam->BSIM3v32k1ox;
                    T1 = sqrt(T0 * T0 + Arg1);
                    T2 = CoxWL * T0 / T1;

                    qgate = CoxWL * pParam->BSIM3v32k1ox * (T1 - T0);

                    Cgg = T2 * (dVgs_eff_dVg - dVgsteff_dVg);
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        Cgd = -T2 * dVgsteff_dVd;
                        Cgb = -T2 * (dVbseffCV_dVb + dVgsteff_dVb);
                        break;
                    case BSIM3v32V32:
                    default:
                        Cgd = -T2 * (dVfb_dVd + dVgsteff_dVd);
                        Cgb = -T2 * (dVfb_dVb + dVbseffCV_dVb + dVgsteff_dVb);
                    }
                }
                qbulk = -qgate;
                Cbg = -Cgg;
                Cbd = -Cgd;
                Cbb = -Cgb;

                One_Third_CoxWL = CoxWL / 3.0;
                Two_Third_CoxWL = 2.0 * One_Third_CoxWL;
                AbulkCV = Abulk0 * pParam->BSIM3v32abulkCVfactor;
                dAbulkCV_dVb = pParam->BSIM3v32abulkCVfactor * dAbulk0_dVb;
                VdsatCV = Vgsteff / AbulkCV;
                if (VdsatCV < Vds)
                {
                    dVdsatCV_dVg = 1.0 / AbulkCV;
                    dVdsatCV_dVb = -VdsatCV * dAbulkCV_dVb / AbulkCV;
                    T0 = Vgsteff - VdsatCV / 3.0;
                    dT0_dVg = 1.0 - dVdsatCV_dVg / 3.0;
                    dT0_dVb = -dVdsatCV_dVb / 3.0;
                    qgate += CoxWL * T0;
                    Cgg1 = CoxWL * dT0_dVg;
                    Cgb1 = CoxWL * dT0_dVb + Cgg1 * dVgsteff_dVb;
                    Cgd1 = Cgg1 * dVgsteff_dVd;
                    Cgg1 *= dVgsteff_dVg;
                    Cgg += Cgg1;
                    Cgb += Cgb1;
                    Cgd += Cgd1;

                    T0 = VdsatCV - Vgsteff;
                    dT0_dVg = dVdsatCV_dVg - 1.0;
                    dT0_dVb = dVdsatCV_dVb;
                    qbulk += One_Third_CoxWL * T0;
                    Cbg1 = One_Third_CoxWL * dT0_dVg;
                    Cbb1 = One_Third_CoxWL * dT0_dVb + Cbg1 * dVgsteff_dVb;
                    Cbd1 = Cbg1 * dVgsteff_dVd;
                    Cbg1 *= dVgsteff_dVg;
                    Cbg += Cbg1;
                    Cbb += Cbb1;
                    Cbd += Cbd1;

                    if (model->BSIM3v32xpart > 0.5)
                        T0 = -Two_Third_CoxWL;
                    else if (model->BSIM3v32xpart < 0.5)
                        T0 = -0.4 * CoxWL;
                    else
                        T0 = -One_Third_CoxWL;

                    qsrc = T0 * Vgsteff;
                    Csg = T0 * dVgsteff_dVg;
                    Csb = T0 * dVgsteff_dVb;
                    Csd = T0 * dVgsteff_dVd;
                    Cgb *= dVbseff_dVb;
                    Cbb *= dVbseff_dVb;
                    Csb *= dVbseff_dVb;
                }
                else
                {
                    T0 = AbulkCV * Vds;
                    T1 = 12.0 * (Vgsteff - 0.5 * T0 + 1.e-20);
                    T2 = Vds / T1;
                    T3 = T0 * T2;
                    dT3_dVg = -12.0 * T2 * T2 * AbulkCV;
                    dT3_dVd = 6.0 * T0 * (4.0 * Vgsteff - T0) / T1 / T1 - 0.5;
                    dT3_dVb = 12.0 * T2 * T2 * dAbulkCV_dVb * Vgsteff;

                    qgate += CoxWL * (Vgsteff - 0.5 * Vds + T3);
                    Cgg1 = CoxWL * (1.0 + dT3_dVg);
                    Cgb1 = CoxWL * dT3_dVb + Cgg1 * dVgsteff_dVb;
                    Cgd1 = CoxWL * dT3_dVd + Cgg1 * dVgsteff_dVd;
                    Cgg1 *= dVgsteff_dVg;
                    Cgg += Cgg1;
                    Cgb += Cgb1;
                    Cgd += Cgd1;

                    qbulk += CoxWL * (1.0 - AbulkCV) * (0.5 * Vds - T3);
                    Cbg1 = -CoxWL * ((1.0 - AbulkCV) * dT3_dVg);
                    Cbb1 = -CoxWL * ((1.0 - AbulkCV) * dT3_dVb + (0.5 * Vds - T3) * dAbulkCV_dVb) + Cbg1 * dVgsteff_dVb;
                    Cbd1 = -CoxWL * (1.0 - AbulkCV) * dT3_dVd + Cbg1 * dVgsteff_dVd;
                    Cbg1 *= dVgsteff_dVg;
                    Cbg += Cbg1;
                    Cbb += Cbb1;
                    Cbd += Cbd1;

                    if (model->BSIM3v32xpart > 0.5)
                    { /* 0/100 Charge petition model */
                        T1 = T1 + T1;
                        qsrc = -CoxWL * (0.5 * Vgsteff + 0.25 * T0 - T0 * T0 / T1);
                        Csg = -CoxWL * (0.5 + 24.0 * T0 * Vds / T1 / T1 * AbulkCV);
                        Csb = -CoxWL * (0.25 * Vds * dAbulkCV_dVb - 12.0 * T0 * Vds / T1 / T1 * (4.0 * Vgsteff - T0) * dAbulkCV_dVb) + Csg * dVgsteff_dVb;
                        Csd = -CoxWL * (0.25 * AbulkCV - 12.0 * AbulkCV * T0 / T1 / T1 * (4.0 * Vgsteff - T0)) + Csg * dVgsteff_dVd;
                        Csg *= dVgsteff_dVg;
                    }
                    else if (model->BSIM3v32xpart < 0.5)
                    { /* 40/60 Charge petition model */
                        T1 = T1 / 12.0;
                        T2 = 0.5 * CoxWL / (T1 * T1);
                        T3 = Vgsteff * (2.0 * T0 * T0 / 3.0 + Vgsteff * (Vgsteff - 4.0 * T0 / 3.0)) - 2.0 * T0 * T0 * T0 / 15.0;
                        qsrc = -T2 * T3;
                        T4 = 4.0 / 3.0 * Vgsteff * (Vgsteff - T0) + 0.4 * T0 * T0;
                        Csg = -2.0 * qsrc / T1 - T2 * (Vgsteff * (3.0 * Vgsteff - 8.0 * T0 / 3.0) + 2.0 * T0 * T0 / 3.0);
                        Csb = (qsrc / T1 * Vds + T2 * T4 * Vds) * dAbulkCV_dVb + Csg * dVgsteff_dVb;
                        Csd = (qsrc / T1 + T2 * T4) * AbulkCV + Csg * dVgsteff_dVd;
                        Csg *= dVgsteff_dVg;
                    }
                    else
                    { /* 50/50 Charge petition model */
                        qsrc = -0.5 * (qgate + qbulk);
                        Csg = -0.5 * (Cgg1 + Cbg1);
                        Csb = -0.5 * (Cgb1 + Cbb1);
                        Csd = -0.5 * (Cgd1 + Cbd1);
                    }
                    Cgb *= dVbseff_dVb;
                    Cbb *= dVbseff_dVb;
                    Csb *= dVbseff_dVb;
                }
                qdrn = -(qgate + qbulk + qsrc);
                this->BSIM3v32cggb = Cgg;
                this->BSIM3v32cgsb = -(Cgg + Cgd + Cgb);
                this->BSIM3v32cgdb = Cgd;
                this->BSIM3v32cdgb = -(Cgg + Cbg + Csg);
                this->BSIM3v32cdsb = (Cgg + Cgd + Cgb + Cbg + Cbd + Cbb + Csg + Csd + Csb);
                this->BSIM3v32cddb = -(Cgd + Cbd + Csd);
                this->BSIM3v32cbgb = Cbg;
                this->BSIM3v32cbsb = -(Cbg + Cbd + Cbb);
                this->BSIM3v32cbdb = Cbd;
                this->BSIM3v32qinv = -(qgate + qbulk);
            }

            else if (model->BSIM3v32capMod == 2)
            {
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    Vfb = this->BSIM3v32vfbzb;
                    break;
                case BSIM3v32V32:
                    Vfb = this->BSIM3v32vfbzb;
                    dVfb_dVb = dVfb_dVd = 0.0;
                    break;
                default: /*  old code prior to v3.2 */
                    Vfb = Vth - pParam->BSIM3v32phi - pParam->BSIM3v32k1ox * sqrtPhis;
                    dVfb_dVb = dVth_dVb - pParam->BSIM3v32k1ox * dsqrtPhis_dVb;
                    dVfb_dVd = dVth_dVd;
                }

                V3 = Vfb - Vgs_eff + VbseffCV - DELTA_3;
                if (Vfb <= 0.0)
                {
                    T0 = sqrt(V3 * V3 - 4.0 * DELTA_3 * Vfb);
                    T2 = -DELTA_3 / T0;
                }
                else
                {
                    T0 = sqrt(V3 * V3 + 4.0 * DELTA_3 * Vfb);
                    T2 = DELTA_3 / T0;
                }

                T1 = 0.5 * (1.0 + V3 / T0);
                Vfbeff = Vfb - 0.5 * (V3 + T0);
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    /* Do nothing */
                    break;
                case BSIM3v32V32:
                default:
                    dVfbeff_dVd = (1.0 - T1 - T2) * dVfb_dVd;
                }
                dVfbeff_dVg = T1 * dVgs_eff_dVg;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    dVfbeff_dVb = -T1 * dVbseffCV_dVb;
                    break;
                case BSIM3v32V32:
                default:
                    dVfbeff_dVb = (1.0 - T1 - T2) * dVfb_dVb - T1 * dVbseffCV_dVb;
                }
                Qac0 = CoxWL * (Vfbeff - Vfb);
                dQac0_dVg = CoxWL * dVfbeff_dVg;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    /* Do nothing */
                    break;
                case BSIM3v32V32:
                default:
                    dQac0_dVd = CoxWL * (dVfbeff_dVd - dVfb_dVd);
                }
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    dQac0_dVb = CoxWL * dVfbeff_dVb;
                    break;
                case BSIM3v32V32:
                default:
                    dQac0_dVb = CoxWL * (dVfbeff_dVb - dVfb_dVb);
                }

                T0 = 0.5 * pParam->BSIM3v32k1ox;
                T3 = Vgs_eff - Vfbeff - VbseffCV - Vgsteff;
                if (pParam->BSIM3v32k1ox == 0.0)
                {
                    T1 = 0.0;
                    T2 = 0.0;
                }
                else if (T3 < 0.0)
                {
                    T1 = T0 + T3 / pParam->BSIM3v32k1ox;
                    T2 = CoxWL;
                }
                else
                {
                    T1 = sqrt(T0 * T0 + T3);
                    T2 = CoxWL * T0 / T1;
                }

                Qsub0 = CoxWL * pParam->BSIM3v32k1ox * (T1 - T0);

                dQsub0_dVg = T2 * (dVgs_eff_dVg - dVfbeff_dVg - dVgsteff_dVg);
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    dQsub0_dVd = -T2 * dVgsteff_dVd;
                    break;
                case BSIM3v32V32:
                default:
                    dQsub0_dVd = -T2 * (dVfbeff_dVd + dVgsteff_dVd);
                }
                dQsub0_dVb = -T2 * (dVfbeff_dVb + dVbseffCV_dVb + dVgsteff_dVb);

                AbulkCV = Abulk0 * pParam->BSIM3v32abulkCVfactor;
                dAbulkCV_dVb = pParam->BSIM3v32abulkCVfactor * dAbulk0_dVb;
                VdsatCV = Vgsteff / AbulkCV;

                V4 = VdsatCV - Vds - DELTA_4;
                T0 = sqrt(V4 * V4 + 4.0 * DELTA_4 * VdsatCV);
                VdseffCV = VdsatCV - 0.5 * (V4 + T0);
                T1 = 0.5 * (1.0 + V4 / T0);
                T2 = DELTA_4 / T0;
                T3 = (1.0 - T1 - T2) / AbulkCV;
                dVdseffCV_dVg = T3;
                dVdseffCV_dVd = T1;
                dVdseffCV_dVb = -T3 * VdsatCV * dAbulkCV_dVb;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    /* Added to eliminate non-zero VdseffCV at Vds=0.0 */
                    if (Vds == 0.0)
                    {
                        VdseffCV = 0.0;
                        dVdseffCV_dVg = 0.0;
                        dVdseffCV_dVb = 0.0;
                    }
                    break;
                case BSIM3v32V32:
                default:
                    /* Do nothing */
                    break;
                }

                T0 = AbulkCV * VdseffCV;
                T1 = 12.0 * (Vgsteff - 0.5 * T0 + 1e-20);
                T2 = VdseffCV / T1;
                T3 = T0 * T2;

                T4 = (1.0 - 12.0 * T2 * T2 * AbulkCV);
                T5 = (6.0 * T0 * (4.0 * Vgsteff - T0) / (T1 * T1) - 0.5);
                T6 = 12.0 * T2 * T2 * Vgsteff;

                qinoi = -CoxWL * (Vgsteff - 0.5 * T0 + AbulkCV * T3);
                qgate = CoxWL * (Vgsteff - 0.5 * VdseffCV + T3);
                Cgg1 = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                Cgd1 = CoxWL * T5 * dVdseffCV_dVd + Cgg1 * dVgsteff_dVd;
                Cgb1 = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Cgg1 * dVgsteff_dVb;
                Cgg1 *= dVgsteff_dVg;

                T7 = 1.0 - AbulkCV;
                qbulk = CoxWL * T7 * (0.5 * VdseffCV - T3);
                T4 = -T7 * (T4 - 1.0);
                T5 = -T7 * T5;
                T6 = -(T7 * T6 + (0.5 * VdseffCV - T3));
                Cbg1 = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                Cbd1 = CoxWL * T5 * dVdseffCV_dVd + Cbg1 * dVgsteff_dVd;
                Cbb1 = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Cbg1 * dVgsteff_dVb;
                Cbg1 *= dVgsteff_dVg;

                if (model->BSIM3v32xpart > 0.5)
                { /* 0/100 Charge petition model */
                    T1 = T1 + T1;
                    qsrc = -CoxWL * (0.5 * Vgsteff + 0.25 * T0 - T0 * T0 / T1);
                    T7 = (4.0 * Vgsteff - T0) / (T1 * T1);
                    T4 = -(0.5 + 24.0 * T0 * T0 / (T1 * T1));
                    T5 = -(0.25 * AbulkCV - 12.0 * AbulkCV * T0 * T7);
                    T6 = -(0.25 * VdseffCV - 12.0 * T0 * VdseffCV * T7);
                    Csg = CoxWL * (T4 + T5 * dVdseffCV_dVg);
                    Csd = CoxWL * T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd;
                    Csb = CoxWL * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Csg * dVgsteff_dVb;
                    Csg *= dVgsteff_dVg;
                }
                else if (model->BSIM3v32xpart < 0.5)
                { /* 40/60 Charge petition model */
                    T1 = T1 / 12.0;
                    T2 = 0.5 * CoxWL / (T1 * T1);
                    T3 = Vgsteff * (2.0 * T0 * T0 / 3.0 + Vgsteff * (Vgsteff - 4.0 * T0 / 3.0)) - 2.0 * T0 * T0 * T0 / 15.0;
                    qsrc = -T2 * T3;
                    T7 = 4.0 / 3.0 * Vgsteff * (Vgsteff - T0) + 0.4 * T0 * T0;
                    T4 = -2.0 * qsrc / T1 - T2 * (Vgsteff * (3.0 * Vgsteff - 8.0 * T0 / 3.0) + 2.0 * T0 * T0 / 3.0);
                    T5 = (qsrc / T1 + T2 * T7) * AbulkCV;
                    T6 = (qsrc / T1 * VdseffCV + T2 * T7 * VdseffCV);
                    Csg = (T4 + T5 * dVdseffCV_dVg);
                    Csd = T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd;
                    Csb = (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Csg * dVgsteff_dVb;
                    Csg *= dVgsteff_dVg;
                }
                else
                { /* 50/50 Charge petition model */
                    qsrc = -0.5 * (qgate + qbulk);
                    Csg = -0.5 * (Cgg1 + Cbg1);
                    Csb = -0.5 * (Cgb1 + Cbb1);
                    Csd = -0.5 * (Cgd1 + Cbd1);
                }

                qgate += Qac0 + Qsub0;
                qbulk -= (Qac0 + Qsub0);
                qdrn = -(qgate + qbulk + qsrc);

                Cgg = dQac0_dVg + dQsub0_dVg + Cgg1;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    Cgd = dQsub0_dVd + Cgd1;
                    break;
                case BSIM3v32V32:
                default:
                    Cgd = dQac0_dVd + dQsub0_dVd + Cgd1;
                }
                Cgb = dQac0_dVb + dQsub0_dVb + Cgb1;

                Cbg = Cbg1 - dQac0_dVg - dQsub0_dVg;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    Cbd = Cbd1 - dQsub0_dVd;
                    break;
                case BSIM3v32V32:
                default:
                    Cbd = Cbd1 - dQac0_dVd - dQsub0_dVd;
                }
                Cbb = Cbb1 - dQac0_dVb - dQsub0_dVb;

                Cgb *= dVbseff_dVb;
                Cbb *= dVbseff_dVb;
                Csb *= dVbseff_dVb;

                this->BSIM3v32cggb = Cgg;
                this->BSIM3v32cgsb = -(Cgg + Cgd + Cgb);
                this->BSIM3v32cgdb = Cgd;
                this->BSIM3v32cdgb = -(Cgg + Cbg + Csg);
                this->BSIM3v32cdsb = (Cgg + Cgd + Cgb + Cbg + Cbd + Cbb + Csg + Csd + Csb);
                this->BSIM3v32cddb = -(Cgd + Cbd + Csd);
                this->BSIM3v32cbgb = Cbg;
                this->BSIM3v32cbsb = -(Cbg + Cbd + Cbb);
                this->BSIM3v32cbdb = Cbd;
                this->BSIM3v32qinv = qinoi;
            }

            /* New Charge-Thickness capMod (CTM) begins */
            else if (model->BSIM3v32capMod == 3)
            {
                V3 = this->BSIM3v32vfbzb - Vgs_eff + VbseffCV - DELTA_3;
                if (this->BSIM3v32vfbzb <= 0.0)
                {
                    T0 = sqrt(V3 * V3 - 4.0 * DELTA_3 * this->BSIM3v32vfbzb);
                    T2 = -DELTA_3 / T0;
                }
                else
                {
                    T0 = sqrt(V3 * V3 + 4.0 * DELTA_3 * this->BSIM3v32vfbzb);
                    T2 = DELTA_3 / T0;
                }

                T1 = 0.5 * (1.0 + V3 / T0);
                Vfbeff = this->BSIM3v32vfbzb - 0.5 * (V3 + T0);
                dVfbeff_dVg = T1 * dVgs_eff_dVg;
                dVfbeff_dVb = -T1 * dVbseffCV_dVb;

                Cox = model->BSIM3v32cox;
                Tox = 1.0e8 * model->BSIM3v32tox;
                T0 = (Vgs_eff - VbseffCV - this->BSIM3v32vfbzb) / Tox;
                dT0_dVg = dVgs_eff_dVg / Tox;
                dT0_dVb = -dVbseffCV_dVb / Tox;

                tmp = T0 * pParam->BSIM3v32acde;
                if ((-EXP_THRESHOLD < tmp) && (tmp < EXP_THRESHOLD))
                {
                    Tcen = pParam->BSIM3v32ldeb * exp(tmp);
                    dTcen_dVg = pParam->BSIM3v32acde * Tcen;
                    dTcen_dVb = dTcen_dVg * dT0_dVb;
                    dTcen_dVg *= dT0_dVg;
                }
                else if (tmp <= -EXP_THRESHOLD)
                {
                    Tcen = pParam->BSIM3v32ldeb * MIN_EXP;
                    dTcen_dVg = dTcen_dVb = 0.0;
                }
                else
                {
                    Tcen = pParam->BSIM3v32ldeb * MAX_EXP;
                    dTcen_dVg = dTcen_dVb = 0.0;
                }

                LINK = 1.0e-3 * model->BSIM3v32tox;
                V3 = pParam->BSIM3v32ldeb - Tcen - LINK;
                V4 = sqrt(V3 * V3 + 4.0 * LINK * pParam->BSIM3v32ldeb);
                Tcen = pParam->BSIM3v32ldeb - 0.5 * (V3 + V4);
                T1 = 0.5 * (1.0 + V3 / V4);
                dTcen_dVg *= T1;
                dTcen_dVb *= T1;

                Ccen = EPSSI / Tcen;
                T2 = Cox / (Cox + Ccen);
                Coxeff = T2 * Ccen;
                T3 = -Ccen / Tcen;
                dCoxeff_dVg = T2 * T2 * T3;
                dCoxeff_dVb = dCoxeff_dVg * dTcen_dVb;
                dCoxeff_dVg *= dTcen_dVg;
                CoxWLcen = CoxWL * Coxeff / Cox;

                Qac0 = CoxWLcen * (Vfbeff - this->BSIM3v32vfbzb);
                QovCox = Qac0 / Coxeff;
                dQac0_dVg = CoxWLcen * dVfbeff_dVg + QovCox * dCoxeff_dVg;
                dQac0_dVb = CoxWLcen * dVfbeff_dVb + QovCox * dCoxeff_dVb;

                T0 = 0.5 * pParam->BSIM3v32k1ox;
                T3 = Vgs_eff - Vfbeff - VbseffCV - Vgsteff;
                if (pParam->BSIM3v32k1ox == 0.0)
                {
                    T1 = 0.0;
                    T2 = 0.0;
                }
                else if (T3 < 0.0)
                {
                    T1 = T0 + T3 / pParam->BSIM3v32k1ox;
                    T2 = CoxWLcen;
                }
                else
                {
                    T1 = sqrt(T0 * T0 + T3);
                    T2 = CoxWLcen * T0 / T1;
                }

                Qsub0 = CoxWLcen * pParam->BSIM3v32k1ox * (T1 - T0);
                QovCox = Qsub0 / Coxeff;
                dQsub0_dVg = T2 * (dVgs_eff_dVg - dVfbeff_dVg - dVgsteff_dVg) + QovCox * dCoxeff_dVg;
                dQsub0_dVd = -T2 * dVgsteff_dVd;
                dQsub0_dVb = -T2 * (dVfbeff_dVb + dVbseffCV_dVb + dVgsteff_dVb) + QovCox * dCoxeff_dVb;

                /* Gate-bias dependent delta Phis begins */
                if (pParam->BSIM3v32k1ox <= 0.0)
                {
                    Denomi = 0.25 * pParam->BSIM3v32moin * Vtm;
                    T0 = 0.5 * pParam->BSIM3v32sqrtPhi;
                }
                else
                {
                    Denomi = pParam->BSIM3v32moin * Vtm * pParam->BSIM3v32k1ox * pParam->BSIM3v32k1ox;
                    T0 = pParam->BSIM3v32k1ox * pParam->BSIM3v32sqrtPhi;
                }
                T1 = 2.0 * T0 + Vgsteff;

                DeltaPhi = Vtm * log(1.0 + T1 * Vgsteff / Denomi);
                dDeltaPhi_dVg = 2.0 * Vtm * (T1 - T0) / (Denomi + T1 * Vgsteff);
                dDeltaPhi_dVd = dDeltaPhi_dVg * dVgsteff_dVd;
                dDeltaPhi_dVb = dDeltaPhi_dVg * dVgsteff_dVb;
                /* End of delta Phis */

                T3 = 4.0 * (Vth - this->BSIM3v32vfbzb - pParam->BSIM3v32phi);
                Tox += Tox;
                if (T3 >= 0.0)
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        T0 = (Vgsteff + T3) / Tox;
                        dT0_dVd = (dVgsteff_dVd + 4.0 * dVth_dVd) / Tox;
                        dT0_dVb = (dVgsteff_dVb + 4.0 * dVth_dVb) / Tox;
                        break;
                    case BSIM3v32V32:
                    default:
                        T0 = (Vgsteff + T3) / Tox;
                    }
                }
                else
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                    case BSIM3v32V322:
                        T0 = (Vgsteff + 1.0e-20) / Tox;
                        dT0_dVd = dVgsteff_dVd / Tox;
                        dT0_dVb = dVgsteff_dVb / Tox;
                        break;
                    case BSIM3v32V32:
                    default:
                        T0 = (Vgsteff + 1.0e-20) / Tox;
                    }
                }
                tmp = exp(0.7 * log(T0));
                T1 = 1.0 + tmp;
                T2 = 0.7 * tmp / (T0 * Tox);
                Tcen = 1.9e-9 / T1;
                dTcen_dVg = -1.9e-9 * T2 / T1 / T1;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    dTcen_dVd = Tox * dTcen_dVg;
                    dTcen_dVb = dTcen_dVd * dT0_dVb;
                    dTcen_dVd *= dT0_dVd;
                    break;
                case BSIM3v32V32:
                default:
                    dTcen_dVd = dTcen_dVg * (4.0 * dVth_dVd + dVgsteff_dVd);
                    dTcen_dVb = dTcen_dVg * (4.0 * dVth_dVb + dVgsteff_dVb);
                }
                dTcen_dVg *= dVgsteff_dVg;

                Ccen = EPSSI / Tcen;
                T0 = Cox / (Cox + Ccen);
                Coxeff = T0 * Ccen;
                T1 = -Ccen / Tcen;
                dCoxeff_dVg = T0 * T0 * T1;
                dCoxeff_dVd = dCoxeff_dVg * dTcen_dVd;
                dCoxeff_dVb = dCoxeff_dVg * dTcen_dVb;
                dCoxeff_dVg *= dTcen_dVg;
                CoxWLcen = CoxWL * Coxeff / Cox;

                AbulkCV = Abulk0 * pParam->BSIM3v32abulkCVfactor;
                dAbulkCV_dVb = pParam->BSIM3v32abulkCVfactor * dAbulk0_dVb;
                VdsatCV = (Vgsteff - DeltaPhi) / AbulkCV;
                V4 = VdsatCV - Vds - DELTA_4;
                T0 = sqrt(V4 * V4 + 4.0 * DELTA_4 * VdsatCV);
                VdseffCV = VdsatCV - 0.5 * (V4 + T0);
                T1 = 0.5 * (1.0 + V4 / T0);
                T2 = DELTA_4 / T0;
                T3 = (1.0 - T1 - T2) / AbulkCV;
                T4 = T3 * (1.0 - dDeltaPhi_dVg);
                dVdseffCV_dVg = T4;
                dVdseffCV_dVd = T1;
                dVdseffCV_dVb = -T3 * VdsatCV * dAbulkCV_dVb;
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                case BSIM3v32V322:
                    /* Added to eliminate non-zero VdseffCV at Vds=0.0 */
                    if (Vds == 0.0)
                    {
                        VdseffCV = 0.0;
                        dVdseffCV_dVg = 0.0;
                        dVdseffCV_dVb = 0.0;
                    }
                    break;
                case BSIM3v32V32:
                default:
                    /* Do nothing */
                    break;
                }

                T0 = AbulkCV * VdseffCV;
                T1 = Vgsteff - DeltaPhi;
                T2 = 12.0 * (T1 - 0.5 * T0 + 1.0e-20);
                T3 = T0 / T2;
                T4 = 1.0 - 12.0 * T3 * T3;
                T5 = AbulkCV * (6.0 * T0 * (4.0 * T1 - T0) / (T2 * T2) - 0.5);
                T6 = T5 * VdseffCV / AbulkCV;

                qgate = qinoi = CoxWLcen * (T1 - T0 * (0.5 - T3));
                QovCox = qgate / Coxeff;
                Cgg1 = CoxWLcen * (T4 * (1.0 - dDeltaPhi_dVg) + T5 * dVdseffCV_dVg);
                Cgd1 = CoxWLcen * T5 * dVdseffCV_dVd + Cgg1 * dVgsteff_dVd + QovCox * dCoxeff_dVd;
                Cgb1 = CoxWLcen * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Cgg1 * dVgsteff_dVb + QovCox * dCoxeff_dVb;
                Cgg1 = Cgg1 * dVgsteff_dVg + QovCox * dCoxeff_dVg;

                T7 = 1.0 - AbulkCV;
                T8 = T2 * T2;
                T9 = 12.0 * T7 * T0 * T0 / (T8 * AbulkCV);
                T10 = T9 * (1.0 - dDeltaPhi_dVg);
                T11 = -T7 * T5 / AbulkCV;
                T12 = -(T9 * T1 / AbulkCV + VdseffCV * (0.5 - T0 / T2));

                qbulk = CoxWLcen * T7 * (0.5 * VdseffCV - T0 * VdseffCV / T2);
                QovCox = qbulk / Coxeff;
                Cbg1 = CoxWLcen * (T10 + T11 * dVdseffCV_dVg);
                Cbd1 = CoxWLcen * T11 * dVdseffCV_dVd + Cbg1 * dVgsteff_dVd + QovCox * dCoxeff_dVd;
                Cbb1 = CoxWLcen * (T11 * dVdseffCV_dVb + T12 * dAbulkCV_dVb) + Cbg1 * dVgsteff_dVb + QovCox * dCoxeff_dVb;
                Cbg1 = Cbg1 * dVgsteff_dVg + QovCox * dCoxeff_dVg;

                if (model->BSIM3v32xpart > 0.5)
                { /* 0/100 partition */
                    qsrc = -CoxWLcen * (T1 / 2.0 + T0 / 4.0 - 0.5 * T0 * T0 / T2);
                    QovCox = qsrc / Coxeff;
                    T2 += T2;
                    T3 = T2 * T2;
                    T7 = -(0.25 - 12.0 * T0 * (4.0 * T1 - T0) / T3);
                    T4 = -(0.5 + 24.0 * T0 * T0 / T3) * (1.0 - dDeltaPhi_dVg);
                    T5 = T7 * AbulkCV;
                    T6 = T7 * VdseffCV;

                    Csg = CoxWLcen * (T4 + T5 * dVdseffCV_dVg);
                    Csd = CoxWLcen * T5 * dVdseffCV_dVd + Csg * dVgsteff_dVd + QovCox * dCoxeff_dVd;
                    Csb = CoxWLcen * (T5 * dVdseffCV_dVb + T6 * dAbulkCV_dVb) + Csg * dVgsteff_dVb + QovCox * dCoxeff_dVb;
                    Csg = Csg * dVgsteff_dVg + QovCox * dCoxeff_dVg;
                }
                else if (model->BSIM3v32xpart < 0.5)
                { /* 40/60 partition */
                    T2 = T2 / 12.0;
                    T3 = 0.5 * CoxWLcen / (T2 * T2);
                    T4 = T1 * (2.0 * T0 * T0 / 3.0 + T1 * (T1 - 4.0 * T0 / 3.0)) - 2.0 * T0 * T0 * T0 / 15.0;
                    qsrc = -T3 * T4;
                    QovCox = qsrc / Coxeff;
                    T8 = 4.0 / 3.0 * T1 * (T1 - T0) + 0.4 * T0 * T0;
                    T5 = -2.0 * qsrc / T2 - T3 * (T1 * (3.0 * T1 - 8.0 * T0 / 3.0) + 2.0 * T0 * T0 / 3.0);
                    T6 = AbulkCV * (qsrc / T2 + T3 * T8);
                    T7 = T6 * VdseffCV / AbulkCV;

                    Csg = T5 * (1.0 - dDeltaPhi_dVg) + T6 * dVdseffCV_dVg;
                    Csd = Csg * dVgsteff_dVd + T6 * dVdseffCV_dVd + QovCox * dCoxeff_dVd;
                    Csb = Csg * dVgsteff_dVb + T6 * dVdseffCV_dVb + T7 * dAbulkCV_dVb + QovCox * dCoxeff_dVb;
                    Csg = Csg * dVgsteff_dVg + QovCox * dCoxeff_dVg;
                }
                else
                { /* 50/50 partition */
                    qsrc = -0.5 * qgate;
                    Csg = -0.5 * Cgg1;
                    Csd = -0.5 * Cgd1;
                    Csb = -0.5 * Cgb1;
                }

                qgate += Qac0 + Qsub0 - qbulk;
                qbulk -= (Qac0 + Qsub0);
                qdrn = -(qgate + qbulk + qsrc);

                Cbg = Cbg1 - dQac0_dVg - dQsub0_dVg;
                Cbd = Cbd1 - dQsub0_dVd;
                Cbb = Cbb1 - dQac0_dVb - dQsub0_dVb;

                Cgg = Cgg1 - Cbg;
                Cgd = Cgd1 - Cbd;
                Cgb = Cgb1 - Cbb;

                Cgb *= dVbseff_dVb;
                Cbb *= dVbseff_dVb;
                Csb *= dVbseff_dVb;

                this->BSIM3v32cggb = Cgg;
                this->BSIM3v32cgsb = -(Cgg + Cgd + Cgb);
                this->BSIM3v32cgdb = Cgd;
                this->BSIM3v32cdgb = -(Cgg + Cbg + Csg);
                this->BSIM3v32cdsb = (Cgg + Cgd + Cgb + Cbg + Cbd + Cbb + Csg + Csd + Csb);
                this->BSIM3v32cddb = -(Cgd + Cbd + Csd);
                this->BSIM3v32cbgb = Cbg;
                this->BSIM3v32cbsb = -(Cbg + Cbd + Cbb);
                this->BSIM3v32cbdb = Cbd;
                this->BSIM3v32qinv = -qinoi;
            } /* End of CTM */
        }

    finished:
        /* Returning Values to Calling Routine */
        /*
           *  COMPUTE EQUIVALENT DRAIN CURRENT SOURCE
           */

        this->BSIM3v32qgate = qgate;
        this->BSIM3v32qbulk = qbulk;
        this->BSIM3v32qdrn = qdrn;
        this->BSIM3v32cd = cdrain;

        if (ChargeComputationNeeded)
        { /*  charge storage elements
               *  bulk-drain and bulk-source depletion capacitances
               *  czbd : zero bias drain junction capacitance
               *  czbs : zero bias source junction capacitance
               *  czbdsw: zero bias drain junction sidewall capacitance
                          along field oxide
               *  czbssw: zero bias source junction sidewall capacitance
                          along field oxide
               *  czbdswg: zero bias drain junction sidewall capacitance
                           along gate side
               *  czbsswg: zero bias source junction sidewall capacitance
                           along gate side
               */

            if (model->BSIM3v32acmMod == 0)
            {
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                    czbd = model->BSIM3v32unitAreaTempJctCap * this->BSIM3v32drainArea; /*bug fix */
                    czbs = model->BSIM3v32unitAreaTempJctCap * this->BSIM3v32sourceArea;
                    break;
                case BSIM3v32V322:
                case BSIM3v32V32:
                default:
                    czbd = model->BSIM3v32unitAreaJctCap * this->BSIM3v32drainArea;
                    czbs = model->BSIM3v32unitAreaJctCap * this->BSIM3v32sourceArea;
                }

                if (this->BSIM3v32drainPerimeter < pParam->BSIM3v32weff)
                {
                    czbdsw = 0.0;
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                        czbdswg = model->BSIM3v32unitLengthGateSidewallTempJctCap * this->BSIM3v32drainPerimeter;
                        break;
                    case BSIM3v32V322:
                    case BSIM3v32V32:
                    default:
                        czbdswg = model->BSIM3v32unitLengthGateSidewallJctCap * this->BSIM3v32drainPerimeter;
                    }
                }
                else
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                        czbdsw = model->BSIM3v32unitLengthSidewallTempJctCap * (this->BSIM3v32drainPerimeter - pParam->BSIM3v32weff);
                        czbdswg = model->BSIM3v32unitLengthGateSidewallTempJctCap * pParam->BSIM3v32weff;
                        break;
                    case BSIM3v32V322:
                    case BSIM3v32V32:
                    default:
                        czbdsw = model->BSIM3v32unitLengthSidewallJctCap * (this->BSIM3v32drainPerimeter - pParam->BSIM3v32weff);
                        czbdswg = model->BSIM3v32unitLengthGateSidewallJctCap * pParam->BSIM3v32weff;
                    }
                }
                if (this->BSIM3v32sourcePerimeter < pParam->BSIM3v32weff)
                {
                    czbssw = 0.0;
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                        czbsswg = model->BSIM3v32unitLengthGateSidewallTempJctCap * this->BSIM3v32sourcePerimeter;
                        break;
                    case BSIM3v32V322:
                    case BSIM3v32V32:
                    default:
                        czbsswg = model->BSIM3v32unitLengthGateSidewallJctCap * this->BSIM3v32sourcePerimeter;
                    }
                }
                else
                {
                    /* Added revision dependent code */
                    switch (model->BSIM3v32intVersion)
                    {
                    case BSIM3v32V324:
                    case BSIM3v32V323:
                        czbssw = model->BSIM3v32unitLengthSidewallTempJctCap * (this->BSIM3v32sourcePerimeter - pParam->BSIM3v32weff);
                        czbsswg = model->BSIM3v32unitLengthGateSidewallTempJctCap * pParam->BSIM3v32weff;
                        break;
                    case BSIM3v32V322:
                    case BSIM3v32V32:
                    default:
                        czbssw = model->BSIM3v32unitLengthSidewallJctCap * (this->BSIM3v32sourcePerimeter - pParam->BSIM3v32weff);
                        czbsswg = model->BSIM3v32unitLengthGateSidewallJctCap * pParam->BSIM3v32weff;
                    }
                }
            }
            else
            {
                /* Added revision dependent code */
                switch (model->BSIM3v32intVersion)
                {
                case BSIM3v32V324:
                case BSIM3v32V323:
                    error = ACM_junctionCapacitances(
                        model->BSIM3v32acmMod,
                        model->BSIM3v32calcacm,
                        this->BSIM3v32geo,
                        model->BSIM3v32hdif,
                        model->BSIM3v32wmlt,
                        this->BSIM3v32w,
                        model->BSIM3v32xw,
                        this->BSIM3v32drainAreaGiven,
                        this->BSIM3v32drainArea,
                        this->BSIM3v32drainPerimeterGiven,
                        this->BSIM3v32drainPerimeter,
                        this->BSIM3v32sourceAreaGiven,
                        this->BSIM3v32sourceArea,
                        this->BSIM3v32sourcePerimeterGiven,
                        this->BSIM3v32sourcePerimeter,
                        model->BSIM3v32unitAreaTempJctCap,
                        model->BSIM3v32unitLengthSidewallTempJctCap,
                        model->BSIM3v32unitLengthGateSidewallTempJctCap,
                        &czbd,
                        &czbdsw,
                        &czbdswg,
                        &czbs,
                        &czbssw,
                        &czbsswg);
                    break;
                case BSIM3v32V322:
                case BSIM3v32V32:
                default:
                    error = ACM_junctionCapacitances(
                        model->BSIM3v32acmMod,
                        model->BSIM3v32calcacm,
                        this->BSIM3v32geo,
                        model->BSIM3v32hdif,
                        model->BSIM3v32wmlt,
                        this->BSIM3v32w,
                        model->BSIM3v32xw,
                        this->BSIM3v32drainAreaGiven,
                        this->BSIM3v32drainArea,
                        this->BSIM3v32drainPerimeterGiven,
                        this->BSIM3v32drainPerimeter,
                        this->BSIM3v32sourceAreaGiven,
                        this->BSIM3v32sourceArea,
                        this->BSIM3v32sourcePerimeterGiven,
                        this->BSIM3v32sourcePerimeter,
                        model->BSIM3v32unitAreaJctCap,
                        model->BSIM3v32unitLengthSidewallJctCap,
                        model->BSIM3v32unitLengthGateSidewallJctCap,
                        &czbd,
                        &czbdsw,
                        &czbdswg,
                        &czbs,
                        &czbssw,
                        &czbsswg);
                }
                if (error)
                    return (error);
            }

            MJ = model->BSIM3v32bulkJctBotGradingCoeff;
            MJSW = model->BSIM3v32bulkJctSideGradingCoeff;
            MJSWG = model->BSIM3v32bulkJctGateSideGradingCoeff;

            /* Source Bulk Junction */
            if (vbs == 0.0)
            {
                getIntegratorX(BSIM3v32qbs)->set(0, 0.0);
                this->BSIM3v32capbs = czbs + czbssw + czbsswg;
            }
            else if (vbs < 0.0)
            {
                if (czbs > 0.0)
                {
                    arg = 1.0 - vbs / model->BSIM3v32PhiB;
                    if (MJ == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJ * log(arg));
                    getIntegratorX(BSIM3v32qbs)->set(0, model->BSIM3v32PhiB * czbs * (1.0 - arg * sarg) / (1.0 - MJ));
                    this->BSIM3v32capbs = czbs * sarg;
                }
                else
                {
                    getIntegratorX(BSIM3v32qbs)->set(0, 0.0);
                    this->BSIM3v32capbs = 0.0;
                }
                if (czbssw > 0.0)
                {
                    arg = 1.0 - vbs / model->BSIM3v32PhiBSW;
                    if (MJSW == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJSW * log(arg));
                    getIntegratorX(BSIM3v32qbs)->set(0, getIntegratorX(BSIM3v32qbs)->get(0) + model->BSIM3v32PhiBSW * czbssw * (1.0 - arg * sarg) / (1.0 - MJSW));
                    this->BSIM3v32capbs += czbssw * sarg;
                }
                if (czbsswg > 0.0)
                {
                    arg = 1.0 - vbs / model->BSIM3v32PhiBSWG;
                    if (MJSWG == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJSWG * log(arg));
                    getIntegratorX(BSIM3v32qbs)->set(0, getIntegratorX(BSIM3v32qbs)->get(0) + model->BSIM3v32PhiBSWG * czbsswg * (1.0 - arg * sarg) / (1.0 - MJSWG));
                    this->BSIM3v32capbs += czbsswg * sarg;
                }
            }
            else
            {
                T0 = czbs + czbssw + czbsswg;
                T1 = vbs * (czbs * MJ / model->BSIM3v32PhiB + czbssw * MJSW / model->BSIM3v32PhiBSW + czbsswg * MJSWG / model->BSIM3v32PhiBSWG);
                getIntegratorX(BSIM3v32qbs)->set(0, vbs * (T0 + 0.5 * T1));
                this->BSIM3v32capbs = T0 + T1;
            }

            /* Drain Bulk Junction */
            if (vbd == 0.0)
            {
                getIntegratorX(BSIM3v32qbd)->set(0, 0.0);
                this->BSIM3v32capbd = czbd + czbdsw + czbdswg;
            }
            else if (vbd < 0.0)
            {
                if (czbd > 0.0)
                {
                    arg = 1.0 - vbd / model->BSIM3v32PhiB;
                    if (MJ == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJ * log(arg));
                    getIntegratorX(BSIM3v32qbd)->set(0, model->BSIM3v32PhiB * czbd * (1.0 - arg * sarg) / (1.0 - MJ));
                    this->BSIM3v32capbd = czbd * sarg;
                }
                else
                {
                    getIntegratorX(BSIM3v32qbd)->set(0, 0.0);
                    this->BSIM3v32capbd = 0.0;
                }
                if (czbdsw > 0.0)
                {
                    arg = 1.0 - vbd / model->BSIM3v32PhiBSW;
                    if (MJSW == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJSW * log(arg));
                    getIntegratorX(BSIM3v32qbd)->set(0, getIntegratorX(BSIM3v32qbd)->get(0) + model->BSIM3v32PhiBSW * czbdsw * (1.0 - arg * sarg) / (1.0 - MJSW));
                    this->BSIM3v32capbd += czbdsw * sarg;
                }
                if (czbdswg > 0.0)
                {
                    arg = 1.0 - vbd / model->BSIM3v32PhiBSWG;
                    if (MJSWG == 0.5)
                        sarg = 1.0 / sqrt(arg);
                    else
                        sarg = exp(-MJSWG * log(arg));
                    getIntegratorX(BSIM3v32qbd)->set(0, getIntegratorX(BSIM3v32qbd)->get(0) + model->BSIM3v32PhiBSWG * czbdswg * (1.0 - arg * sarg) / (1.0 - MJSWG));
                    this->BSIM3v32capbd += czbdswg * sarg;
                }
            }
            else
            {
                T0 = czbd + czbdsw + czbdswg;
                T1 = vbd * (czbd * MJ / model->BSIM3v32PhiB + czbdsw * MJSW / model->BSIM3v32PhiBSW + czbdswg * MJSWG / model->BSIM3v32PhiBSWG);
                getIntegratorX(BSIM3v32qbd)->set(0, vbd * (T0 + 0.5 * T1));
                this->BSIM3v32capbd = T0 + T1;
            }
        }

        /*
        *  check convergence
        */
        if ((this->BSIM3v32off == 0) || (!(env->getCKTmode() & MODEINITFIX)))
        {
            if (Check == 1)
            {
                CKTnoncon++;
#ifndef NEWCONV
            }
            else
            {
                if (this->BSIM3v32mode >= 0)
                {
                    Idtot = this->BSIM3v32cd + this->BSIM3v32csub - this->BSIM3v32cbd;
                }
                else
                {
                    Idtot = this->BSIM3v32cd - this->BSIM3v32cbd;
                }
                tol = env->getRelTol() * std::max(fabs(cdhat), fabs(Idtot)) + env->getAbsTol();
                if (fabs(cdhat - Idtot) >= tol)
                {
                    CKTnoncon++;
                }
                else
                {
                    Ibtot = this->BSIM3v32cbs + this->BSIM3v32cbd - this->BSIM3v32csub;
                    tol = env->getRelTol() * std::max(fabs(cbhat), fabs(Ibtot)) + env->getAbsTol();
                    if (fabs(cbhat - Ibtot) > tol)
                    {
                        CKTnoncon++;
                    }
                }
#endif /* NEWCONV */
            }
        }
        getIntegratorX(BSIM3v32vbs)->set(0, vbs);
        std::cout << "set vbd=" << vbd << "\n";
        getIntegratorX(BSIM3v32vbd)->set(0, vbd);
        getIntegratorX(BSIM3v32vgs)->set(0, vgs);
        getIntegratorX(BSIM3v32vds)->set(0, vds);
        getIntegratorX(BSIM3v32qdef)->set(0, qdef);

        /* bulk and channel charge plus overlaps */

        if (!ChargeComputationNeeded)
            goto line850;

#ifndef NOBYPASS
    line755:
#endif
        /* NQS begins */
        if (this->BSIM3v32nqsMod)
        {
            qcheq = -(qbulk + qgate);

            this->BSIM3v32cqgb = -(this->BSIM3v32cggb + this->BSIM3v32cbgb);
            this->BSIM3v32cqdb = -(this->BSIM3v32cgdb + this->BSIM3v32cbdb);
            this->BSIM3v32cqsb = -(this->BSIM3v32cgsb + this->BSIM3v32cbsb);
            this->BSIM3v32cqbb = -(this->BSIM3v32cqgb + this->BSIM3v32cqdb + this->BSIM3v32cqsb);

            gtau_drift = fabs(this->BSIM3v32tconst * qcheq) * ScalingFactor;
            T0 = pParam->BSIM3v32leffCV * pParam->BSIM3v32leffCV;
            gtau_diff = 16.0 * this->BSIM3v32u0temp * model->BSIM3v32vtm / T0 * ScalingFactor;
            this->BSIM3v32gtau = gtau_drift + gtau_diff;
        }

        if (model->BSIM3v32capMod == 0)
        {
            /* code merge -JX */
            cgdo = pParam->BSIM3v32cgdo;
            qgdo = pParam->BSIM3v32cgdo * vgd;
            cgso = pParam->BSIM3v32cgso;
            qgso = pParam->BSIM3v32cgso * vgs;
        }
        else if (model->BSIM3v32capMod == 1)
        {
            if (vgd < 0.0)
            {
                T1 = sqrt(1.0 - 4.0 * vgd / pParam->BSIM3v32ckappa);
                cgdo = pParam->BSIM3v32cgdo + pParam->BSIM3v32weffCV * pParam->BSIM3v32cgdl / T1;
                qgdo = pParam->BSIM3v32cgdo * vgd - pParam->BSIM3v32weffCV * 0.5 * pParam->BSIM3v32cgdl * pParam->BSIM3v32ckappa * (T1 - 1.0);
            }
            else
            {
                cgdo = pParam->BSIM3v32cgdo + pParam->BSIM3v32weffCV * pParam->BSIM3v32cgdl;
                qgdo = (pParam->BSIM3v32weffCV * pParam->BSIM3v32cgdl + pParam->BSIM3v32cgdo) * vgd;
            }

            if (vgs < 0.0)
            {
                T1 = sqrt(1.0 - 4.0 * vgs / pParam->BSIM3v32ckappa);
                cgso = pParam->BSIM3v32cgso + pParam->BSIM3v32weffCV * pParam->BSIM3v32cgsl / T1;
                qgso = pParam->BSIM3v32cgso * vgs - pParam->BSIM3v32weffCV * 0.5 * pParam->BSIM3v32cgsl * pParam->BSIM3v32ckappa * (T1 - 1.0);
            }
            else
            {
                cgso = pParam->BSIM3v32cgso + pParam->BSIM3v32weffCV * pParam->BSIM3v32cgsl;
                qgso = (pParam->BSIM3v32weffCV * pParam->BSIM3v32cgsl + pParam->BSIM3v32cgso) * vgs;
            }
        }
        else
        {
            T0 = vgd + DELTA_1;
            T1 = sqrt(T0 * T0 + 4.0 * DELTA_1);
            T2 = 0.5 * (T0 - T1);

            T3 = pParam->BSIM3v32weffCV * pParam->BSIM3v32cgdl;
            T4 = sqrt(1.0 - 4.0 * T2 / pParam->BSIM3v32ckappa);
            cgdo = pParam->BSIM3v32cgdo + T3 - T3 * (1.0 - 1.0 / T4) * (0.5 - 0.5 * T0 / T1);
            qgdo = (pParam->BSIM3v32cgdo + T3) * vgd - T3 * (T2 + 0.5 * pParam->BSIM3v32ckappa * (T4 - 1.0));

            T0 = vgs + DELTA_1;
            T1 = sqrt(T0 * T0 + 4.0 * DELTA_1);
            T2 = 0.5 * (T0 - T1);
            T3 = pParam->BSIM3v32weffCV * pParam->BSIM3v32cgsl;
            T4 = sqrt(1.0 - 4.0 * T2 / pParam->BSIM3v32ckappa);
            cgso = pParam->BSIM3v32cgso + T3 - T3 * (1.0 - 1.0 / T4) * (0.5 - 0.5 * T0 / T1);
            qgso = (pParam->BSIM3v32cgso + T3) * vgs - T3 * (T2 + 0.5 * pParam->BSIM3v32ckappa * (T4 - 1.0));
        }

        this->BSIM3v32cgdo = cgdo;
        this->BSIM3v32cgso = cgso;

        ag0 = getIntegratorCoeff(0);
        if (this->BSIM3v32mode > 0)
        {
            if (this->BSIM3v32nqsMod == 0)
            {
                gcggb = (this->BSIM3v32cggb + cgdo + cgso + pParam->BSIM3v32cgbo) * ag0;
                gcgdb = (this->BSIM3v32cgdb - cgdo) * ag0;
                gcgsb = (this->BSIM3v32cgsb - cgso) * ag0;

                gcdgb = (this->BSIM3v32cdgb - cgdo) * ag0;
                gcddb = (this->BSIM3v32cddb + this->BSIM3v32capbd + cgdo) * ag0;
                gcdsb = this->BSIM3v32cdsb * ag0;

                gcsgb = -(this->BSIM3v32cggb + this->BSIM3v32cbgb + this->BSIM3v32cdgb + cgso) * ag0;
                gcsdb = -(this->BSIM3v32cgdb + this->BSIM3v32cbdb + this->BSIM3v32cddb) * ag0;
                gcssb = (this->BSIM3v32capbs + cgso - (this->BSIM3v32cgsb + this->BSIM3v32cbsb + this->BSIM3v32cdsb)) * ag0;

                gcbgb = (this->BSIM3v32cbgb - pParam->BSIM3v32cgbo) * ag0;
                gcbdb = (this->BSIM3v32cbdb - this->BSIM3v32capbd) * ag0;
                gcbsb = (this->BSIM3v32cbsb - this->BSIM3v32capbs) * ag0;

                qgd = qgdo;
                qgs = qgso;
                qgb = pParam->BSIM3v32cgbo * vgb;
                qgate += qgd + qgs + qgb;
                qbulk -= qgb;
                qdrn -= qgd;
                qsrc = -(qgate + qbulk + qdrn);

                ggtg = ggtd = ggtb = ggts = 0.0;
                sxpart = 0.6;
                dxpart = 0.4;
                ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
            }
            else
            {
                if (qcheq > 0.0)
                    T0 = this->BSIM3v32tconst * qdef * ScalingFactor;
                else
                    T0 = -this->BSIM3v32tconst * qdef * ScalingFactor;
                ggtg = this->BSIM3v32gtg = T0 * this->BSIM3v32cqgb;
                ggtd = this->BSIM3v32gtd = T0 * this->BSIM3v32cqdb;
                ggts = this->BSIM3v32gts = T0 * this->BSIM3v32cqsb;
                ggtb = this->BSIM3v32gtb = T0 * this->BSIM3v32cqbb;
                gqdef = ScalingFactor * ag0;

                gcqgb = this->BSIM3v32cqgb * ag0;
                gcqdb = this->BSIM3v32cqdb * ag0;
                gcqsb = this->BSIM3v32cqsb * ag0;
                gcqbb = this->BSIM3v32cqbb * ag0;

                gcggb = (cgdo + cgso + pParam->BSIM3v32cgbo) * ag0;
                gcgdb = -cgdo * ag0;
                gcgsb = -cgso * ag0;

                gcdgb = -cgdo * ag0;
                gcddb = (this->BSIM3v32capbd + cgdo) * ag0;
                gcdsb = 0.0;

                gcsgb = -cgso * ag0;
                gcsdb = 0.0;
                gcssb = (this->BSIM3v32capbs + cgso) * ag0;

                gcbgb = -pParam->BSIM3v32cgbo * ag0;
                gcbdb = -this->BSIM3v32capbd * ag0;
                gcbsb = -this->BSIM3v32capbs * ag0;

                CoxWL = model->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV;
                if (fabs(qcheq) <= 1.0e-5 * CoxWL)
                {
                    if (model->BSIM3v32xpart < 0.5)
                    {
                        dxpart = 0.4;
                    }
                    else if (model->BSIM3v32xpart > 0.5)
                    {
                        dxpart = 0.0;
                    }
                    else
                    {
                        dxpart = 0.5;
                    }
                    ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                }
                else
                {
                    dxpart = qdrn / qcheq;
                    Cdd = this->BSIM3v32cddb;
                    Csd = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + this->BSIM3v32cbdb);
                    ddxpart_dVd = (Cdd - dxpart * (Cdd + Csd)) / qcheq;
                    Cdg = this->BSIM3v32cdgb;
                    Csg = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + this->BSIM3v32cbgb);
                    ddxpart_dVg = (Cdg - dxpart * (Cdg + Csg)) / qcheq;

                    Cds = this->BSIM3v32cdsb;
                    Css = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + this->BSIM3v32cbsb);
                    ddxpart_dVs = (Cds - dxpart * (Cds + Css)) / qcheq;

                    ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);
                }
                sxpart = 1.0 - dxpart;
                dsxpart_dVd = -ddxpart_dVd;
                dsxpart_dVg = -ddxpart_dVg;
                dsxpart_dVs = -ddxpart_dVs;
                dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);

                qgd = qgdo;
                qgs = qgso;
                qgb = pParam->BSIM3v32cgbo * vgb;
                qgate = qgd + qgs + qgb;
                qbulk = -qgb;
                qdrn = -qgd;
                qsrc = -(qgate + qbulk + qdrn);
            }
        }
        else
        {
            if (this->BSIM3v32nqsMod == 0)
            {
                gcggb = (this->BSIM3v32cggb + cgdo + cgso + pParam->BSIM3v32cgbo) * ag0;
                gcgdb = (this->BSIM3v32cgsb - cgdo) * ag0;
                gcgsb = (this->BSIM3v32cgdb - cgso) * ag0;

                gcdgb = -(this->BSIM3v32cggb + this->BSIM3v32cbgb + this->BSIM3v32cdgb + cgdo) * ag0;
                gcddb = (this->BSIM3v32capbd + cgdo - (this->BSIM3v32cgsb + this->BSIM3v32cbsb + this->BSIM3v32cdsb)) * ag0;
                gcdsb = -(this->BSIM3v32cgdb + this->BSIM3v32cbdb + this->BSIM3v32cddb) * ag0;

                gcsgb = (this->BSIM3v32cdgb - cgso) * ag0;
                gcsdb = this->BSIM3v32cdsb * ag0;
                gcssb = (this->BSIM3v32cddb + this->BSIM3v32capbs + cgso) * ag0;

                gcbgb = (this->BSIM3v32cbgb - pParam->BSIM3v32cgbo) * ag0;
                gcbdb = (this->BSIM3v32cbsb - this->BSIM3v32capbd) * ag0;
                gcbsb = (this->BSIM3v32cbdb - this->BSIM3v32capbs) * ag0;

                qgd = qgdo;
                qgs = qgso;
                qgb = pParam->BSIM3v32cgbo * vgb;
                qgate += qgd + qgs + qgb;
                qbulk -= qgb;
                qsrc = qdrn - qgs;
                qdrn = -(qgate + qbulk + qsrc);

                ggtg = ggtd = ggtb = ggts = 0.0;
                sxpart = 0.4;
                dxpart = 0.6;
                ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
            }
            else
            {
                if (qcheq > 0.0)
                    T0 = this->BSIM3v32tconst * qdef * ScalingFactor;
                else
                    T0 = -this->BSIM3v32tconst * qdef * ScalingFactor;
                ggtg = this->BSIM3v32gtg = T0 * this->BSIM3v32cqgb;
                ggts = this->BSIM3v32gtd = T0 * this->BSIM3v32cqdb;
                ggtd = this->BSIM3v32gts = T0 * this->BSIM3v32cqsb;
                ggtb = this->BSIM3v32gtb = T0 * this->BSIM3v32cqbb;
                gqdef = ScalingFactor * ag0;

                gcqgb = this->BSIM3v32cqgb * ag0;
                gcqdb = this->BSIM3v32cqsb * ag0;
                gcqsb = this->BSIM3v32cqdb * ag0;
                gcqbb = this->BSIM3v32cqbb * ag0;

                gcggb = (cgdo + cgso + pParam->BSIM3v32cgbo) * ag0;
                gcgdb = -cgdo * ag0;
                gcgsb = -cgso * ag0;

                gcdgb = -cgdo * ag0;
                gcddb = (this->BSIM3v32capbd + cgdo) * ag0;
                gcdsb = 0.0;

                gcsgb = -cgso * ag0;
                gcsdb = 0.0;
                gcssb = (this->BSIM3v32capbs + cgso) * ag0;

                gcbgb = -pParam->BSIM3v32cgbo * ag0;
                gcbdb = -this->BSIM3v32capbd * ag0;
                gcbsb = -this->BSIM3v32capbs * ag0;

                CoxWL = model->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV;
                if (fabs(qcheq) <= 1.0e-5 * CoxWL)
                {
                    if (model->BSIM3v32xpart < 0.5)
                    {
                        sxpart = 0.4;
                    }
                    else if (model->BSIM3v32xpart > 0.5)
                    {
                        sxpart = 0.0;
                    }
                    else
                    {
                        sxpart = 0.5;
                    }
                    dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
                }
                else
                {
                    sxpart = qdrn / qcheq;
                    Css = this->BSIM3v32cddb;
                    Cds = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + this->BSIM3v32cbdb);
                    dsxpart_dVs = (Css - sxpart * (Css + Cds)) / qcheq;
                    Csg = this->BSIM3v32cdgb;
                    Cdg = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + this->BSIM3v32cbgb);
                    dsxpart_dVg = (Csg - sxpart * (Csg + Cdg)) / qcheq;

                    Csd = this->BSIM3v32cdsb;
                    Cdd = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + this->BSIM3v32cbsb);
                    dsxpart_dVd = (Csd - sxpart * (Csd + Cdd)) / qcheq;

                    dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);
                }
                dxpart = 1.0 - sxpart;
                ddxpart_dVd = -dsxpart_dVd;
                ddxpart_dVg = -dsxpart_dVg;
                ddxpart_dVs = -dsxpart_dVs;
                ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);

                qgd = qgdo;
                qgs = qgso;
                qgb = pParam->BSIM3v32cgbo * vgb;
                qgate = qgd + qgs + qgb;
                qbulk = -qgb;
                qsrc = -qgs;
                qdrn = -(qgate + qbulk + qsrc);
            }
        }

        cqdef = cqcheq = 0.0;
        if (ByPass)
            goto line860;

        getIntegratorX(BSIM3v32qg)->set(0, qgate);
        getIntegratorX(BSIM3v32qd)->set(0, qdrn - getIntegratorX(BSIM3v32qbd)->get(0));
        getIntegratorX(BSIM3v32qb)->set(0, qbulk + getIntegratorX(BSIM3v32qbd)->get(0) + getIntegratorX(BSIM3v32qbs)->get(0));

        if (this->BSIM3v32nqsMod)
        {
            getIntegratorX(BSIM3v32qcdump)->set(0, qdef * ScalingFactor);
            getIntegratorX(BSIM3v32qcheq)->set(0, qcheq);
        }

        /* store small signal parameters */
        if (env->getCKTmode() & MODEINITSMSIG)
        {
            goto line1000;
        }
        if (!ChargeComputationNeeded)
            goto line850;

        if (env->getCKTmode() & MODEINITTRAN)
        {
            getIntegratorX(BSIM3v32qb)->set(1, getIntegratorX(BSIM3v32qb)->get(0));
            getIntegratorX(BSIM3v32qg)->set(1, getIntegratorX(BSIM3v32qg)->get(0));
            getIntegratorX(BSIM3v32qd)->set(1, getIntegratorX(BSIM3v32qd)->get(0));

            if (this->BSIM3v32nqsMod)
            {
                getIntegratorX(BSIM3v32qcheq)->set(1, getIntegratorX(BSIM3v32qcheq)->get(0));
                getIntegratorX(BSIM3v32qcdump)->set(1, getIntegratorX(BSIM3v32qcdump)->get(0));
            }
        }

        /* Run integral step */
        integrate(BSIM3v32qb, 1.0 /* in SPICE this is 0.0 */, &geq, &ceq);
        integrate(BSIM3v32qg, 1.0, &geq, &ceq);
        integrate(BSIM3v32qd, 1.0, &geq, &ceq);

        if (this->BSIM3v32nqsMod)
        {
            integrate(BSIM3v32qcdump, 1.0, &geq, &ceq);
            integrate(BSIM3v32qcheq, 1.0, &geq, &ceq);
        }

        goto line860;

    line850:
        /* initialize to zero charge conductance and current */
        ceqqg = ceqqb = ceqqd = 0.0;
        cqcheq = cqdef = 0.0;

        gcdgb = gcddb = gcdsb = 0.0;
        gcsgb = gcsdb = gcssb = 0.0;
        gcggb = gcgdb = gcgsb = 0.0;
        gcbgb = gcbdb = gcbsb = 0.0;

        gqdef = gcqgb = gcqdb = gcqsb = gcqbb = 0.0;
        ggtg = ggtd = ggtb = ggts = 0.0;
        sxpart = (1.0 - (dxpart = (this->BSIM3v32mode > 0) ? 0.4 : 0.6));
        ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
        dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;

        if (this->BSIM3v32nqsMod)
            this->BSIM3v32gtau = 16.0 * this->BSIM3v32u0temp * model->BSIM3v32vtm / pParam->BSIM3v32leffCV / pParam->BSIM3v32leffCV * ScalingFactor;
        else
            this->BSIM3v32gtau = 0.0;

        goto line900;

    line860:
        /* evaluate equivalent charge current */

        cqgate = getIntegratorY(BSIM3v32qg)->get(0);
        cqbulk = getIntegratorY(BSIM3v32qb)->get(0);
        cqdrn = getIntegratorY(BSIM3v32qd)->get(0);

        ceqqg = cqgate - gcggb * vgb + gcgdb * vbd + gcgsb * vbs;
        ceqqb = cqbulk - gcbgb * vgb + gcbdb * vbd + gcbsb * vbs;
        ceqqd = cqdrn - gcdgb * vgb + gcddb * vbd + gcdsb * vbs;

        if (this->BSIM3v32nqsMod)
        {
            T0 = ggtg * vgb - ggtd * vbd - ggts * vbs;
            ceqqg += T0;
            T1 = qdef * this->BSIM3v32gtau;
            ceqqd -= dxpart * T0 + T1 * (ddxpart_dVg * vgb - ddxpart_dVd * vbd - ddxpart_dVs * vbs);
            cqdef = getIntegratorY(BSIM3v32qcdump)->get(0) - gqdef * qdef;
            cqcheq = getIntegratorY(BSIM3v32qcheq)->get(0) - (gcqgb * vgb - gcqdb * vbd - gcqsb * vbs) + T0;
        }

        if (env->getCKTmode() & MODEINITTRAN)
        {
            getIntegratorY(BSIM3v32qb)->set(1, getIntegratorY(BSIM3v32qb)->get(0));
            getIntegratorY(BSIM3v32qg)->set(1, getIntegratorY(BSIM3v32qg)->get(0));
            getIntegratorY(BSIM3v32qd)->set(1, getIntegratorY(BSIM3v32qd)->get(0));

            if (this->BSIM3v32nqsMod)
            {
                getIntegratorY(BSIM3v32qcheq)->set(1, getIntegratorY(BSIM3v32qcheq)->get(0));
                getIntegratorY(BSIM3v32qcdump)->set(1, getIntegratorY(BSIM3v32qcdump)->get(0));
            }
        }

        /*
           *  load current vector
           */
    line900:

        if (this->BSIM3v32mode >= 0)
        {
            Gm = this->BSIM3v32gm;
            Gmbs = this->BSIM3v32gmbs;
            FwdSum = Gm + Gmbs;
            RevSum = 0.0;
            cdreq = model->BSIM3v32type * (cdrain - this->BSIM3v32gds * vds - Gm * vgs - Gmbs * vbs);

            ceqbd = -model->BSIM3v32type * (this->BSIM3v32csub - this->BSIM3v32gbds * vds - this->BSIM3v32gbgs * vgs - this->BSIM3v32gbbs * vbs);
            ceqbs = 0.0;

            gbbdp = -this->BSIM3v32gbds;
            gbbsp = (this->BSIM3v32gbds + this->BSIM3v32gbgs + this->BSIM3v32gbbs);

            gbdpg = this->BSIM3v32gbgs;
            gbdpdp = this->BSIM3v32gbds;
            gbdpb = this->BSIM3v32gbbs;
            gbdpsp = -(gbdpg + gbdpdp + gbdpb);

            gbspg = 0.0;
            gbspdp = 0.0;
            gbspb = 0.0;
            gbspsp = 0.0;
        }
        else
        {
            Gm = -this->BSIM3v32gm;
            Gmbs = -this->BSIM3v32gmbs;
            FwdSum = 0.0;
            RevSum = -(Gm + Gmbs);
            cdreq = -model->BSIM3v32type * (cdrain + this->BSIM3v32gds * vds + Gm * vgd + Gmbs * vbd);

            ceqbs = -model->BSIM3v32type * (this->BSIM3v32csub + this->BSIM3v32gbds * vds - this->BSIM3v32gbgs * vgd - this->BSIM3v32gbbs * vbd);
            ceqbd = 0.0;

            gbbsp = -this->BSIM3v32gbds;
            gbbdp = (this->BSIM3v32gbds + this->BSIM3v32gbgs + this->BSIM3v32gbbs);

            gbdpg = 0.0;
            gbdpsp = 0.0;
            gbdpb = 0.0;
            gbdpdp = 0.0;

            gbspg = this->BSIM3v32gbgs;
            gbspsp = this->BSIM3v32gbds;
            gbspb = this->BSIM3v32gbbs;
            gbspdp = -(gbspg + gbspsp + gbspb);
        }

        if (model->BSIM3v32type > 0)
        {
            ceqbs += (this->BSIM3v32cbs - this->BSIM3v32gbs * vbs);
            ceqbd += (this->BSIM3v32cbd - this->BSIM3v32gbd * vbd);
            /*
               ceqqg = ceqqg;
               ceqqb = ceqqb;
               ceqqd = ceqqd;
               cqdef = cqdef;
               cqcheq = cqcheq;
               */
        }
        else
        {
            ceqbs -= (this->BSIM3v32cbs - this->BSIM3v32gbs * vbs);
            ceqbd -= (this->BSIM3v32cbd - this->BSIM3v32gbd * vbd);
            ceqqg = -ceqqg;
            ceqqb = -ceqqb;
            ceqqd = -ceqqd;
            cqdef = -cqdef;
            cqcheq = -cqcheq;
        }

        m = this->BSIM3v32m;

#ifdef USE_OMP
        this->BSIM3v32rhsG = m * ceqqg;
        this->BSIM3v32rhsB = m * (ceqbs + ceqbd + ceqqb);
        this->BSIM3v32rhsD = m * (ceqbd - cdreq - ceqqd);
        this->BSIM3v32rhsS = m * (cdreq + ceqbs + ceqqg + ceqqb + ceqqd);
        if (this->BSIM3v32nqsMod)
            this->BSIM3v32rhsQ = m * (cqcheq - cqdef);
#else
        addI(BSIM3v32gNode, -(m * ceqqg));
        addI(BSIM3v32bNode, -(m * (ceqbs + ceqbd + ceqqb)));
        addI(BSIM3v32dNodePrime, m * (ceqbd - cdreq - ceqqd));
        addI(BSIM3v32sNodePrime, m * (cdreq + ceqbs + ceqqg + ceqqb + ceqqd));
        if (this->BSIM3v32nqsMod)
            addI(BSIM3v32qNode, m * (cqcheq - cqdef));
#endif

        /*
           *  load y matrix
           */

        T1 = qdef * this->BSIM3v32gtau;
#ifdef USE_OMP
        this->BSIM3v32DdPt = m * this->BSIM3v32drainConductance;
        this->BSIM3v32GgPt = m * (gcggb - ggtg);
        this->BSIM3v32SsPt = m * this->BSIM3v32sourceConductance;
        this->BSIM3v32BbPt = m * (this->BSIM3v32gbd + this->BSIM3v32gbs - gcbgb - gcbdb - gcbsb - this->BSIM3v32gbbs);
        this->BSIM3v32DPdpPt = m * (this->BSIM3v32drainConductance + this->BSIM3v32gds + this->BSIM3v32gbd + RevSum + gcddb + dxpart * ggtd + T1 * ddxpart_dVd + gbdpdp);
        this->BSIM3v32SPspPt = m * (this->BSIM3v32sourceConductance + this->BSIM3v32gds + this->BSIM3v32gbs + FwdSum + gcssb + sxpart * ggts + T1 * dsxpart_dVs + gbspsp);
        this->BSIM3v32DdpPt = m * this->BSIM3v32drainConductance;
        this->BSIM3v32GbPt = m * (gcggb + gcgdb + gcgsb + ggtb);
        this->BSIM3v32GdpPt = m * (gcgdb - ggtd);
        this->BSIM3v32GspPt = m * (gcgsb - ggts);
        this->BSIM3v32SspPt = m * this->BSIM3v32sourceConductance;
        this->BSIM3v32BgPt = m * (gcbgb - this->BSIM3v32gbgs);
        this->BSIM3v32BdpPt = m * (gcbdb - this->BSIM3v32gbd + gbbdp);
        this->BSIM3v32BspPt = m * (gcbsb - this->BSIM3v32gbs + gbbsp);
        this->BSIM3v32DPdPt = m * this->BSIM3v32drainConductance;
        this->BSIM3v32DPgPt = m * (Gm + gcdgb + dxpart * ggtg + T1 * ddxpart_dVg + gbdpg);
        this->BSIM3v32DPbPt = m * (this->BSIM3v32gbd - Gmbs + gcdgb + gcddb + gcdsb - dxpart * ggtb - T1 * ddxpart_dVb - gbdpb);
        this->BSIM3v32DPspPt = m * (this->BSIM3v32gds + FwdSum - gcdsb - dxpart * ggts - T1 * ddxpart_dVs - gbdpsp);
        this->BSIM3v32SPgPt = m * (gcsgb - Gm + sxpart * ggtg + T1 * dsxpart_dVg + gbspg);
        this->BSIM3v32SPsPt = m * this->BSIM3v32sourceConductance;
        this->BSIM3v32SPbPt = m * (this->BSIM3v32gbs + Gmbs + gcsgb + gcsdb + gcssb - sxpart * ggtb - T1 * dsxpart_dVb - gbspb);
        this->BSIM3v32SPdpPt = m * (this->BSIM3v32gds + RevSum - gcsdb - sxpart * ggtd - T1 * dsxpart_dVd - gbspdp);

        if (this->BSIM3v32nqsMod)
        {
            this->BSIM3v32QqPt = m * (gqdef + this->BSIM3v32gtau);

            this->BSIM3v32DPqPt = m * (dxpart * this->BSIM3v32gtau);
            this->BSIM3v32SPqPt = m * (sxpart * this->BSIM3v32gtau);
            this->BSIM3v32GqPt = m * this->BSIM3v32gtau;

            this->BSIM3v32QgPt = m * (ggtg - gcqgb);
            this->BSIM3v32QdpPt = m * (ggtd - gcqdb);
            this->BSIM3v32QspPt = m * (ggts - gcqsb);
            this->BSIM3v32QbPt = m * (ggtb - gcqbb);
        }
#else
        (*(this->BSIM3v32DdPtr) += m * this->BSIM3v32drainConductance);
        (*(this->BSIM3v32GgPtr) += m * (gcggb - ggtg));
        (*(this->BSIM3v32SsPtr) += m * this->BSIM3v32sourceConductance);
        (*(this->BSIM3v32BbPtr) += m * (this->BSIM3v32gbd + this->BSIM3v32gbs - gcbgb - gcbdb - gcbsb - this->BSIM3v32gbbs));
        (*(this->BSIM3v32DPdpPtr) += m * (this->BSIM3v32drainConductance + this->BSIM3v32gds + this->BSIM3v32gbd + RevSum + gcddb + dxpart * ggtd + T1 * ddxpart_dVd + gbdpdp));
        (*(this->BSIM3v32SPspPtr) += m * (this->BSIM3v32sourceConductance + this->BSIM3v32gds + this->BSIM3v32gbs + FwdSum + gcssb + sxpart * ggts + T1 * dsxpart_dVs + gbspsp));
        (*(this->BSIM3v32DdpPtr) -= m * this->BSIM3v32drainConductance);
        (*(this->BSIM3v32GbPtr) -= m * (gcggb + gcgdb + gcgsb + ggtb));
        (*(this->BSIM3v32GdpPtr) += m * (gcgdb - ggtd));
        (*(this->BSIM3v32GspPtr) += m * (gcgsb - ggts));
        (*(this->BSIM3v32SspPtr) -= m * this->BSIM3v32sourceConductance);
        (*(this->BSIM3v32BgPtr) += m * (gcbgb - this->BSIM3v32gbgs));
        (*(this->BSIM3v32BdpPtr) += m * (gcbdb - this->BSIM3v32gbd + gbbdp));
        (*(this->BSIM3v32BspPtr) += m * (gcbsb - this->BSIM3v32gbs + gbbsp));
        (*(this->BSIM3v32DPdPtr) -= m * this->BSIM3v32drainConductance);
        (*(this->BSIM3v32DPgPtr) += m * (Gm + gcdgb + dxpart * ggtg + T1 * ddxpart_dVg + gbdpg));
        (*(this->BSIM3v32DPbPtr) -= m * (this->BSIM3v32gbd - Gmbs + gcdgb + gcddb + gcdsb - dxpart * ggtb - T1 * ddxpart_dVb - gbdpb));
        (*(this->BSIM3v32DPspPtr) -= m * (this->BSIM3v32gds + FwdSum - gcdsb - dxpart * ggts - T1 * ddxpart_dVs - gbdpsp));
        (*(this->BSIM3v32SPgPtr) += m * (gcsgb - Gm + sxpart * ggtg + T1 * dsxpart_dVg + gbspg));
        (*(this->BSIM3v32SPsPtr) -= m * this->BSIM3v32sourceConductance);
        (*(this->BSIM3v32SPbPtr) -= m * (this->BSIM3v32gbs + Gmbs + gcsgb + gcsdb + gcssb - sxpart * ggtb - T1 * dsxpart_dVb - gbspb));
        (*(this->BSIM3v32SPdpPtr) -= m * (this->BSIM3v32gds + RevSum - gcsdb - sxpart * ggtd - T1 * dsxpart_dVd - gbspdp));

        if (this->BSIM3v32nqsMod)
        {
            *(this->BSIM3v32QqPtr) += m * (gqdef + this->BSIM3v32gtau);

            *(this->BSIM3v32DPqPtr) += m * (dxpart * this->BSIM3v32gtau);
            *(this->BSIM3v32SPqPtr) += m * (sxpart * this->BSIM3v32gtau);
            *(this->BSIM3v32GqPtr) -= m * this->BSIM3v32gtau;

            *(this->BSIM3v32QgPtr) += m * (ggtg - gcqgb);
            *(this->BSIM3v32QdpPtr) += m * (ggtd - gcqdb);
            *(this->BSIM3v32QspPtr) += m * (ggts - gcqsb);
            *(this->BSIM3v32QbPtr) += m * (ggtb - gcqbb);
        }
#endif

    line1000:;

        (void)dVgst_dVg;
        (void)dVgst_dVb;
        (void)dDeltaPhi_dVd;
        (void)dDeltaPhi_dVb;

        return (CKTnoncon == 0) ? 0 : CERR_NON_CONVERGENCE;
    }

    int BSIM3V32::stampAC(Environment *env, double omega)
    {
        double xcggb, xcgdb, xcgsb, xcbgb, xcbdb, xcbsb, xcddb, xcssb, xcdgb;
        double gdpr, gspr, gds, gbd, gbs, capbd, capbs, xcsgb, xcdsb, xcsdb;
        double cggb, cgdb, cgsb, cbgb, cbdb, cbsb, cddb, cdgb, cdsb;
        double GSoverlapCap, GDoverlapCap, GBoverlapCap, FwdSum, RevSum, Gm, Gmbs;
        double dxpart, sxpart, xgtg, xgtd, xgts, xgtb;
        double xcqgb = 0.0, xcqdb = 0.0, xcqsb = 0.0, xcqbb = 0.0;
        double gbspsp, gbbdp, gbbsp, gbspg, gbspb;
        double gbspdp, gbdpdp, gbdpg, gbdpb, gbdpsp;
        double ddxpart_dVd, ddxpart_dVg, ddxpart_dVb, ddxpart_dVs;
        double dsxpart_dVd, dsxpart_dVg, dsxpart_dVb, dsxpart_dVs;
        double T1, CoxWL, qcheq, Cdg, Cdd, Cds, Csg, Csd, Css;
        double ScalingFactor = 1.0e-9;
        double m;

        if (this->BSIM3v32mode >= 0)
        {
            Gm = this->BSIM3v32gm;
            Gmbs = this->BSIM3v32gmbs;
            FwdSum = Gm + Gmbs;
            RevSum = 0.0;

            gbbdp = -this->BSIM3v32gbds;
            gbbsp = this->BSIM3v32gbds + this->BSIM3v32gbgs + this->BSIM3v32gbbs;

            gbdpg = this->BSIM3v32gbgs;
            gbdpb = this->BSIM3v32gbbs;
            gbdpdp = this->BSIM3v32gbds;
            gbdpsp = -(gbdpg + gbdpb + gbdpdp);

            gbspdp = 0.0;
            gbspg = 0.0;
            gbspb = 0.0;
            gbspsp = 0.0;

            if (this->BSIM3v32nqsMod == 0)
            {
                cggb = this->BSIM3v32cggb;
                cgsb = this->BSIM3v32cgsb;
                cgdb = this->BSIM3v32cgdb;

                cbgb = this->BSIM3v32cbgb;
                cbsb = this->BSIM3v32cbsb;
                cbdb = this->BSIM3v32cbdb;

                cdgb = this->BSIM3v32cdgb;
                cdsb = this->BSIM3v32cdsb;
                cddb = this->BSIM3v32cddb;

                xgtg = xgtd = xgts = xgtb = 0.0;
                sxpart = 0.6;
                dxpart = 0.4;
                ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
            }
            else
            {
                cggb = cgdb = cgsb = 0.0;
                cbgb = cbdb = cbsb = 0.0;
                cdgb = cddb = cdsb = 0.0;

                xgtg = this->BSIM3v32gtg;
                xgtd = this->BSIM3v32gtd;
                xgts = this->BSIM3v32gts;
                xgtb = this->BSIM3v32gtb;

                xcqgb = this->BSIM3v32cqgb * omega;
                xcqdb = this->BSIM3v32cqdb * omega;
                xcqsb = this->BSIM3v32cqsb * omega;
                xcqbb = this->BSIM3v32cqbb * omega;

                CoxWL = model->BSIM3v32cox * this->pParam->BSIM3v32weffCV * this->pParam->BSIM3v32leffCV;
                qcheq = -(this->BSIM3v32qgate + this->BSIM3v32qbulk);
                if (fabs(qcheq) <= 1.0e-5 * CoxWL)
                {
                    if (model->BSIM3v32xpart < 0.5)
                    {
                        dxpart = 0.4;
                    }
                    else if (model->BSIM3v32xpart > 0.5)
                    {
                        dxpart = 0.0;
                    }
                    else
                    {
                        dxpart = 0.5;
                    }
                    ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                }
                else
                {
                    dxpart = this->BSIM3v32qdrn / qcheq;
                    Cdd = this->BSIM3v32cddb;
                    Csd = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + this->BSIM3v32cbdb);
                    ddxpart_dVd = (Cdd - dxpart * (Cdd + Csd)) / qcheq;
                    Cdg = this->BSIM3v32cdgb;
                    Csg = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + this->BSIM3v32cbgb);
                    ddxpart_dVg = (Cdg - dxpart * (Cdg + Csg)) / qcheq;

                    Cds = this->BSIM3v32cdsb;
                    Css = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + this->BSIM3v32cbsb);
                    ddxpart_dVs = (Cds - dxpart * (Cds + Css)) / qcheq;

                    ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);
                }
                sxpart = 1.0 - dxpart;
                dsxpart_dVd = -ddxpart_dVd;
                dsxpart_dVg = -ddxpart_dVg;
                dsxpart_dVs = -ddxpart_dVs;
                dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);
            }
        }
        else
        {
            Gm = -this->BSIM3v32gm;
            Gmbs = -this->BSIM3v32gmbs;
            FwdSum = 0.0;
            RevSum = -(Gm + Gmbs);

            gbbsp = -this->BSIM3v32gbds;
            gbbdp = this->BSIM3v32gbds + this->BSIM3v32gbgs + this->BSIM3v32gbbs;

            gbdpg = 0.0;
            gbdpsp = 0.0;
            gbdpb = 0.0;
            gbdpdp = 0.0;

            gbspg = this->BSIM3v32gbgs;
            gbspsp = this->BSIM3v32gbds;
            gbspb = this->BSIM3v32gbbs;
            gbspdp = -(gbspg + gbspsp + gbspb);

            if (this->BSIM3v32nqsMod == 0)
            {
                cggb = this->BSIM3v32cggb;
                cgsb = this->BSIM3v32cgdb;
                cgdb = this->BSIM3v32cgsb;

                cbgb = this->BSIM3v32cbgb;
                cbsb = this->BSIM3v32cbdb;
                cbdb = this->BSIM3v32cbsb;

                cdgb = -(this->BSIM3v32cdgb + cggb + cbgb);
                cdsb = -(this->BSIM3v32cddb + cgsb + cbsb);
                cddb = -(this->BSIM3v32cdsb + cgdb + cbdb);

                xgtg = xgtd = xgts = xgtb = 0.0;
                sxpart = 0.4;
                dxpart = 0.6;
                ddxpart_dVd = ddxpart_dVg = ddxpart_dVb = ddxpart_dVs = 0.0;
                dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
            }
            else
            {
                cggb = cgdb = cgsb = 0.0;
                cbgb = cbdb = cbsb = 0.0;
                cdgb = cddb = cdsb = 0.0;

                xgtg = this->BSIM3v32gtg;
                xgtd = this->BSIM3v32gts;
                xgts = this->BSIM3v32gtd;
                xgtb = this->BSIM3v32gtb;

                xcqgb = this->BSIM3v32cqgb * omega;
                xcqdb = this->BSIM3v32cqsb * omega;
                xcqsb = this->BSIM3v32cqdb * omega;
                xcqbb = this->BSIM3v32cqbb * omega;

                CoxWL = model->BSIM3v32cox * this->pParam->BSIM3v32weffCV * this->pParam->BSIM3v32leffCV;
                qcheq = -(this->BSIM3v32qgate + this->BSIM3v32qbulk);
                if (fabs(qcheq) <= 1.0e-5 * CoxWL)
                {
                    if (model->BSIM3v32xpart < 0.5)
                    {
                        sxpart = 0.4;
                    }
                    else if (model->BSIM3v32xpart > 0.5)
                    {
                        sxpart = 0.0;
                    }
                    else
                    {
                        sxpart = 0.5;
                    }
                    dsxpart_dVd = dsxpart_dVg = dsxpart_dVb = dsxpart_dVs = 0.0;
                }
                else
                {
                    sxpart = this->BSIM3v32qdrn / qcheq;
                    Css = this->BSIM3v32cddb;
                    Cds = -(this->BSIM3v32cgdb + this->BSIM3v32cddb + this->BSIM3v32cbdb);
                    dsxpart_dVs = (Css - sxpart * (Css + Cds)) / qcheq;
                    Csg = this->BSIM3v32cdgb;
                    Cdg = -(this->BSIM3v32cggb + this->BSIM3v32cdgb + this->BSIM3v32cbgb);
                    dsxpart_dVg = (Csg - sxpart * (Csg + Cdg)) / qcheq;

                    Csd = this->BSIM3v32cdsb;
                    Cdd = -(this->BSIM3v32cgsb + this->BSIM3v32cdsb + this->BSIM3v32cbsb);
                    dsxpart_dVd = (Csd - sxpart * (Csd + Cdd)) / qcheq;

                    dsxpart_dVb = -(dsxpart_dVd + dsxpart_dVg + dsxpart_dVs);
                }
                dxpart = 1.0 - sxpart;
                ddxpart_dVd = -dsxpart_dVd;
                ddxpart_dVg = -dsxpart_dVg;
                ddxpart_dVs = -dsxpart_dVs;
                ddxpart_dVb = -(ddxpart_dVd + ddxpart_dVg + ddxpart_dVs);
            }
        }

        T1 = getIntegratorX(BSIM3v32qdef)->get(0) * this->BSIM3v32gtau;
        gdpr = this->BSIM3v32drainConductance;
        gspr = this->BSIM3v32sourceConductance;
        gds = this->BSIM3v32gds;
        gbd = this->BSIM3v32gbd;
        gbs = this->BSIM3v32gbs;
        capbd = this->BSIM3v32capbd;
        capbs = this->BSIM3v32capbs;

        GSoverlapCap = this->BSIM3v32cgso;
        GDoverlapCap = this->BSIM3v32cgdo;
        GBoverlapCap = this->pParam->BSIM3v32cgbo;

        xcdgb = (cdgb - GDoverlapCap) * omega;
        xcddb = (cddb + capbd + GDoverlapCap) * omega;
        xcdsb = cdsb * omega;
        xcsgb = -(cggb + cbgb + cdgb + GSoverlapCap) * omega;
        xcsdb = -(cgdb + cbdb + cddb) * omega;
        xcssb = (capbs + GSoverlapCap - (cgsb + cbsb + cdsb)) * omega;
        xcggb = (cggb + GDoverlapCap + GSoverlapCap + GBoverlapCap) * omega;
        xcgdb = (cgdb - GDoverlapCap) * omega;
        xcgsb = (cgsb - GSoverlapCap) * omega;
        xcbgb = (cbgb - GBoverlapCap) * omega;
        xcbdb = (cbdb - capbd) * omega;
        xcbsb = (cbsb - capbs) * omega;

        m = this->BSIM3v32m;

        *(this->BSIM3v32GgPtr + 1) += m * xcggb;
        *(this->BSIM3v32BbPtr + 1) -=
            m * (xcbgb + xcbdb + xcbsb);
        *(this->BSIM3v32DPdpPtr + 1) += m * xcddb;
        *(this->BSIM3v32SPspPtr + 1) += m * xcssb;
        *(this->BSIM3v32GbPtr + 1) -=
            m * (xcggb + xcgdb + xcgsb);
        *(this->BSIM3v32GdpPtr + 1) += m * xcgdb;
        *(this->BSIM3v32GspPtr + 1) += m * xcgsb;
        *(this->BSIM3v32BgPtr + 1) += m * xcbgb;
        *(this->BSIM3v32BdpPtr + 1) += m * xcbdb;
        *(this->BSIM3v32BspPtr + 1) += m * xcbsb;
        *(this->BSIM3v32DPgPtr + 1) += m * xcdgb;
        *(this->BSIM3v32DPbPtr + 1) -=
            m * (xcdgb + xcddb + xcdsb);
        *(this->BSIM3v32DPspPtr + 1) += m * xcdsb;
        *(this->BSIM3v32SPgPtr + 1) += m * xcsgb;
        *(this->BSIM3v32SPbPtr + 1) -=
            m * (xcsgb + xcsdb + xcssb);
        *(this->BSIM3v32SPdpPtr + 1) += m * xcsdb;

        *(this->BSIM3v32DdPtr) += m * gdpr;
        *(this->BSIM3v32SsPtr) += m * gspr;
        *(this->BSIM3v32BbPtr) +=
            m * (gbd + gbs - this->BSIM3v32gbbs);
        *(this->BSIM3v32DPdpPtr) +=
            m * (gdpr + gds + gbd + RevSum +
                 dxpart * xgtd + T1 * ddxpart_dVd +
                 gbdpdp);
        *(this->BSIM3v32SPspPtr) +=
            m * (gspr + gds + gbs + FwdSum +
                 sxpart * xgts + T1 * dsxpart_dVs +
                 gbspsp);

        *(this->BSIM3v32DdpPtr) -= m * gdpr;
        *(this->BSIM3v32SspPtr) -= m * gspr;

        *(this->BSIM3v32BgPtr) -= m * this->BSIM3v32gbgs;
        *(this->BSIM3v32BdpPtr) -= m * (gbd - gbbdp);
        *(this->BSIM3v32BspPtr) -= m * (gbs - gbbsp);

        *(this->BSIM3v32DPdPtr) -= m * gdpr;
        *(this->BSIM3v32DPgPtr) +=
            m * (Gm + dxpart * xgtg + T1 * ddxpart_dVg +
                 gbdpg);
        *(this->BSIM3v32DPbPtr) -=
            m * (gbd - Gmbs - dxpart * xgtb -
                 T1 * ddxpart_dVb - gbdpb);
        *(this->BSIM3v32DPspPtr) -=
            m * (gds + FwdSum - dxpart * xgts -
                 T1 * ddxpart_dVs - gbdpsp);

        *(this->BSIM3v32SPgPtr) -=
            m * (Gm - sxpart * xgtg - T1 * dsxpart_dVg -
                 gbspg);
        *(this->BSIM3v32SPsPtr) -= m * gspr;
        *(this->BSIM3v32SPbPtr) -=
            m * (gbs + Gmbs - sxpart * xgtb -
                 T1 * dsxpart_dVb - gbspb);
        *(this->BSIM3v32SPdpPtr) -=
            m * (gds + RevSum - sxpart * xgtd -
                 T1 * dsxpart_dVd - gbspdp);

        *(this->BSIM3v32GgPtr) -= m * xgtg;
        *(this->BSIM3v32GbPtr) -= m * xgtb;
        *(this->BSIM3v32GdpPtr) -= m * xgtd;
        *(this->BSIM3v32GspPtr) -= m * xgts;

        if (this->BSIM3v32nqsMod)
        {
            *(this->BSIM3v32QqPtr + 1) +=
                m * omega * ScalingFactor;
            *(this->BSIM3v32QgPtr + 1) -= m * xcqgb;
            *(this->BSIM3v32QdpPtr + 1) -= m * xcqdb;
            *(this->BSIM3v32QspPtr + 1) -= m * xcqsb;
            *(this->BSIM3v32QbPtr + 1) -= m * xcqbb;

            *(this->BSIM3v32QqPtr) += m * this->BSIM3v32gtau;

            *(this->BSIM3v32DPqPtr) +=
                m * (dxpart * this->BSIM3v32gtau);
            *(this->BSIM3v32SPqPtr) +=
                m * (sxpart * this->BSIM3v32gtau);
            *(this->BSIM3v32GqPtr) -= m * this->BSIM3v32gtau;

            *(this->BSIM3v32QgPtr) += m * xgtg;
            *(this->BSIM3v32QdpPtr) += m * xgtd;
            *(this->BSIM3v32QspPtr) += m * xgts;
            *(this->BSIM3v32QbPtr) += m * xgtb;
        }

        return 0;
    }

}
