/**** BSIM3v3.2.4, Released by Xuemei Xi 12/21/2001 ****/

/**********
 * Copyright 2001 Regents of the University of California. All rights reserved.
 * File: b3set.c of BSIM3v3.2.4
 * Author: 1995 Min-Chie Jeng and Mansun Chan.
 * Author: 1997-1999 Weidong Liu.
 * Author: 2001  Xuemei Xi
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
#include <cstring>
#include "BSIM3V32.h"

#define KboQ 8.617087e-5 /* Kb / q  where q = 1.60219e-19 */

namespace csimModel
{
#define IOP(_entry, _id, _type, _desc)                                                          \
    {                                                                                           \
        _entry, _id, _type, _desc, csimModel::PropertyBag::Write | csimModel::PropertyBag::Read \
    }
#define IOPR(_entry, _id, _type, _desc)                                                         \
    {                                                                                           \
        _entry, _id, _type, _desc, csimModel::PropertyBag::Write | csimModel::PropertyBag::Read \
    }
#define IP(_entry, _id, _type, _desc)                            \
    {                                                            \
        _entry, _id, _type, _desc, csimModel::PropertyBag::Write \
    }

    static const PropertyMdlPropDescriptor bsim3v32_mdl_props[] = {
        IOP("capmod", BSIM3v32_MOD_CAPMOD, Variant::VariantInt32, "Capacitance model selector"),
        IOP("mobmod", BSIM3v32_MOD_MOBMOD, Variant::VariantInt32, "Mobility model selector"),
        IOP("noimod", BSIM3v32_MOD_NOIMOD, Variant::VariantInt32, "Noise model selector"),
        IOP("nqsmod", BSIM3v32_MOD_NQSMOD, Variant::VariantInt32, "Non-quasi-static model selector"),
        IOP("acm", BSIM3v32_MOD_ACMMOD, Variant::VariantInt32, "Area calculation method selector"),
        IOP("calcacm", BSIM3v32_MOD_CALCACM, Variant::VariantInt32, "Area calculation method ACM=12"),
        IOP("paramchk", BSIM3v32_MOD_PARAMCHK, Variant::VariantInt32, "Model parameter checking selector"),
        IOP("binunit", BSIM3v32_MOD_BINUNIT, Variant::VariantInt32, "Bin  unit  selector"),
        IOP("version", BSIM3v32_MOD_VERSION, Variant::VariantString, " parameter for model version"),
        IOP("tox", BSIM3v32_MOD_TOX, Variant::VariantDouble, "Gate oxide thickness in meters"),

        IOP("toxm", BSIM3v32_MOD_TOXM, Variant::VariantDouble, "Gate oxide thickness used in extraction"),
        IOP("cdsc", BSIM3v32_MOD_CDSC, Variant::VariantDouble, "Drain/Source and channel coupling capacitance"),
        IOP("cdscb", BSIM3v32_MOD_CDSCB, Variant::VariantDouble, "Body-bias dependence of cdsc"),
        IOP("cdscd", BSIM3v32_MOD_CDSCD, Variant::VariantDouble, "Drain-bias dependence of cdsc"),
        IOP("cit", BSIM3v32_MOD_CIT, Variant::VariantDouble, "Interface state capacitance"),
        IOP("nfactor", BSIM3v32_MOD_NFACTOR, Variant::VariantDouble, "Subthreshold swing Coefficient"),
        IOP("xj", BSIM3v32_MOD_XJ, Variant::VariantDouble, "Junction depth in meters"),
        IOP("vsat", BSIM3v32_MOD_VSAT, Variant::VariantDouble, "Saturation velocity at tnom"),
        IOP("at", BSIM3v32_MOD_AT, Variant::VariantDouble, "Temperature coefficient of vsat"),
        IOP("a0", BSIM3v32_MOD_A0, Variant::VariantDouble, "Non-uniform depletion width effect coefficient."),
        IOP("ags", BSIM3v32_MOD_AGS, Variant::VariantDouble, "Gate bias  coefficient of Abulk."),
        IOP("a1", BSIM3v32_MOD_A1, Variant::VariantDouble, "Non-saturation effect coefficient"),
        IOP("a2", BSIM3v32_MOD_A2, Variant::VariantDouble, "Non-saturation effect coefficient"),
        IOP("keta", BSIM3v32_MOD_KETA, Variant::VariantDouble, "Body-bias coefficient of non-uniform depletion width effect."),
        IOP("nsub", BSIM3v32_MOD_NSUB, Variant::VariantDouble, "Substrate doping concentration"),
        IOP("nch", BSIM3v32_MOD_NPEAK, Variant::VariantDouble, "Channel doping concentration"),
        IOP("ngate", BSIM3v32_MOD_NGATE, Variant::VariantDouble, "Poly-gate doping concentration"),
        IOP("gamma1", BSIM3v32_MOD_GAMMA1, Variant::VariantDouble, "Vth body coefficient"),
        IOP("gamma2", BSIM3v32_MOD_GAMMA2, Variant::VariantDouble, "Vth body coefficient"),
        IOP("vbx", BSIM3v32_MOD_VBX, Variant::VariantDouble, "Vth transition body Voltage"),
        IOP("vbm", BSIM3v32_MOD_VBM, Variant::VariantDouble, "Maximum body voltage"),

        IOP("xt", BSIM3v32_MOD_XT, Variant::VariantDouble, "Doping depth"),
        IOP("k1", BSIM3v32_MOD_K1, Variant::VariantDouble, "Bulk effect coefficient 1"),
        IOP("kt1", BSIM3v32_MOD_KT1, Variant::VariantDouble, "Temperature coefficient of Vth"),
        IOP("kt1l", BSIM3v32_MOD_KT1L, Variant::VariantDouble, "Temperature coefficient of Vth"),
        IOP("kt2", BSIM3v32_MOD_KT2, Variant::VariantDouble, "Body-coefficient of kt1"),
        IOP("k2", BSIM3v32_MOD_K2, Variant::VariantDouble, "Bulk effect coefficient 2"),
        IOP("k3", BSIM3v32_MOD_K3, Variant::VariantDouble, "Narrow width effect coefficient"),
        IOP("k3b", BSIM3v32_MOD_K3B, Variant::VariantDouble, "Body effect coefficient of k3"),
        IOP("w0", BSIM3v32_MOD_W0, Variant::VariantDouble, "Narrow width effect parameter"),
        IOP("nlx", BSIM3v32_MOD_NLX, Variant::VariantDouble, "Lateral non-uniform doping effect"),
        IOP("dvt0", BSIM3v32_MOD_DVT0, Variant::VariantDouble, "Short channel effect coeff. 0"),
        IOP("dvt1", BSIM3v32_MOD_DVT1, Variant::VariantDouble, "Short channel effect coeff. 1"),
        IOP("dvt2", BSIM3v32_MOD_DVT2, Variant::VariantDouble, "Short channel effect coeff. 2"),
        IOP("dvt0w", BSIM3v32_MOD_DVT0W, Variant::VariantDouble, "Narrow Width coeff. 0"),
        IOP("dvt1w", BSIM3v32_MOD_DVT1W, Variant::VariantDouble, "Narrow Width effect coeff. 1"),
        IOP("dvt2w", BSIM3v32_MOD_DVT2W, Variant::VariantDouble, "Narrow Width effect coeff. 2"),
        IOP("drout", BSIM3v32_MOD_DROUT, Variant::VariantDouble, "DIBL coefficient of output resistance"),
        IOP("dsub", BSIM3v32_MOD_DSUB, Variant::VariantDouble, "DIBL coefficient in the subthreshold region"),
        IOP("vth0", BSIM3v32_MOD_VTH0, Variant::VariantDouble, "Threshold voltage"),
        IOPR("vtho", BSIM3v32_MOD_VTH0, Variant::VariantDouble, "Threshold voltage"),
        IOP("ua", BSIM3v32_MOD_UA, Variant::VariantDouble, "Linear gate dependence of mobility"),
        IOP("ua1", BSIM3v32_MOD_UA1, Variant::VariantDouble, "Temperature coefficient of ua"),
        IOP("ub", BSIM3v32_MOD_UB, Variant::VariantDouble, "Quadratic gate dependence of mobility"),
        IOP("ub1", BSIM3v32_MOD_UB1, Variant::VariantDouble, "Temperature coefficient of ub"),
        IOP("uc", BSIM3v32_MOD_UC, Variant::VariantDouble, "Body-bias dependence of mobility"),
        IOP("uc1", BSIM3v32_MOD_UC1, Variant::VariantDouble, "Temperature coefficient of uc"),
        IOP("u0", BSIM3v32_MOD_U0, Variant::VariantDouble, "Low-field mobility at Tnom"),
        IOP("ute", BSIM3v32_MOD_UTE, Variant::VariantDouble, "Temperature coefficient of mobility"),
        IOP("voff", BSIM3v32_MOD_VOFF, Variant::VariantDouble, "Threshold voltage offset"),
        IOP("tnom", BSIM3v32_MOD_TNOM, Variant::VariantDouble, "Parameter measurement temperature"),
        IOP("cgso", BSIM3v32_MOD_CGSO, Variant::VariantDouble, "Gate-source overlap capacitance per width"),
        IOP("cgdo", BSIM3v32_MOD_CGDO, Variant::VariantDouble, "Gate-drain overlap capacitance per width"),
        IOP("cgbo", BSIM3v32_MOD_CGBO, Variant::VariantDouble, "Gate-bulk overlap capacitance per length"),
        IOP("xpart", BSIM3v32_MOD_XPART, Variant::VariantDouble, "Channel charge partitioning"),
        IOP("elm", BSIM3v32_MOD_ELM, Variant::VariantDouble, "Non-quasi-static Elmore Constant Parameter"),
        IOP("delta", BSIM3v32_MOD_DELTA, Variant::VariantDouble, "Effective Vds parameter"),
        IOP("rsh", BSIM3v32_MOD_RSH, Variant::VariantDouble, "Source-drain sheet resistance"),
        IOP("rdsw", BSIM3v32_MOD_RDSW, Variant::VariantDouble, "Source-drain resistance per width"),

        IOP("prwg", BSIM3v32_MOD_PRWG, Variant::VariantDouble, "Gate-bias effect on parasitic resistance "),
        IOP("prwb", BSIM3v32_MOD_PRWB, Variant::VariantDouble, "Body-effect on parasitic resistance "),

        IOP("prt", BSIM3v32_MOD_PRT, Variant::VariantDouble, "Temperature coefficient of parasitic resistance "),
        IOP("eta0", BSIM3v32_MOD_ETA0, Variant::VariantDouble, "Subthreshold region DIBL coefficient"),
        IOP("etab", BSIM3v32_MOD_ETAB, Variant::VariantDouble, "Subthreshold region DIBL coefficient"),
        IOP("pclm", BSIM3v32_MOD_PCLM, Variant::VariantDouble, "Channel length modulation Coefficient"),
        IOP("pdiblc1", BSIM3v32_MOD_PDIBL1, Variant::VariantDouble, "Drain-induced barrier lowering coefficient"),
        IOP("pdiblc2", BSIM3v32_MOD_PDIBL2, Variant::VariantDouble, "Drain-induced barrier lowering coefficient"),
        IOP("pdiblcb", BSIM3v32_MOD_PDIBLB, Variant::VariantDouble, "Body-effect on drain-induced barrier lowering"),
        IOP("pscbe1", BSIM3v32_MOD_PSCBE1, Variant::VariantDouble, "Substrate current body-effect coefficient"),
        IOP("pscbe2", BSIM3v32_MOD_PSCBE2, Variant::VariantDouble, "Substrate current body-effect coefficient"),
        IOP("pvag", BSIM3v32_MOD_PVAG, Variant::VariantDouble, "Gate dependence of output resistance parameter"),
        IOP("js", BSIM3v32_MOD_JS, Variant::VariantDouble, "Source/drain junction reverse saturation current density"),
        IOP("jsw", BSIM3v32_MOD_JSW, Variant::VariantDouble, "Sidewall junction reverse saturation current density"),
        IOP("pb", BSIM3v32_MOD_PB, Variant::VariantDouble, "Source/drain junction built-in potential"),
        IOP("nj", BSIM3v32_MOD_NJ, Variant::VariantDouble, "Source/drain junction emission coefficient"),
        IOP("xti", BSIM3v32_MOD_XTI, Variant::VariantDouble, "Junction current temperature exponent"),
        IOP("mj", BSIM3v32_MOD_MJ, Variant::VariantDouble, "Source/drain bottom junction capacitance grading coefficient"),
        IOP("pbsw", BSIM3v32_MOD_PBSW, Variant::VariantDouble, "Source/drain sidewall junction capacitance built in potential"),
        IOP("mjsw", BSIM3v32_MOD_MJSW, Variant::VariantDouble, "Source/drain sidewall junction capacitance grading coefficient"),
        IOP("pbswg", BSIM3v32_MOD_PBSWG, Variant::VariantDouble, "Source/drain (gate side) sidewall junction capacitance built in potential"),
        IOP("mjswg", BSIM3v32_MOD_MJSWG, Variant::VariantDouble, "Source/drain (gate side) sidewall junction capacitance grading coefficient"),
        IOP("cj", BSIM3v32_MOD_CJ, Variant::VariantDouble, "Source/drain bottom junction capacitance per unit area"),
        IOP("vfbcv", BSIM3v32_MOD_VFBCV, Variant::VariantDouble, "Flat Band Voltage parameter for capmod=0 only"),
        IOP("vfb", BSIM3v32_MOD_VFB, Variant::VariantDouble, "Flat Band Voltage"),
        IOP("cjsw", BSIM3v32_MOD_CJSW, Variant::VariantDouble, "Source/drain sidewall junction capacitance per unit periphery"),
        IOP("cjswg", BSIM3v32_MOD_CJSWG, Variant::VariantDouble, "Source/drain (gate side) sidewall junction capacitance per unit width"),
        IOP("tpb", BSIM3v32_MOD_TPB, Variant::VariantDouble, "Temperature coefficient of pb"),
        IOP("tcj", BSIM3v32_MOD_TCJ, Variant::VariantDouble, "Temperature coefficient of cj"),
        IOP("tpbsw", BSIM3v32_MOD_TPBSW, Variant::VariantDouble, "Temperature coefficient of pbsw"),
        IOP("tcjsw", BSIM3v32_MOD_TCJSW, Variant::VariantDouble, "Temperature coefficient of cjsw"),
        IOP("tpbswg", BSIM3v32_MOD_TPBSWG, Variant::VariantDouble, "Temperature coefficient of pbswg"),
        IOP("tcjswg", BSIM3v32_MOD_TCJSWG, Variant::VariantDouble, "Temperature coefficient of cjswg"),
        IOP("acde", BSIM3v32_MOD_ACDE, Variant::VariantDouble, "Exponential coefficient for finite charge thickness"),
        IOP("moin", BSIM3v32_MOD_MOIN, Variant::VariantDouble, "Coefficient for gate-bias dependent surface potential"),
        IOP("noff", BSIM3v32_MOD_NOFF, Variant::VariantDouble, "C-V turn-on/off parameter"),
        IOP("voffcv", BSIM3v32_MOD_VOFFCV, Variant::VariantDouble, "C-V lateral-shift parameter"),
        IOP("lint", BSIM3v32_MOD_LINT, Variant::VariantDouble, "Length reduction parameter"),
        IOP("ll", BSIM3v32_MOD_LL, Variant::VariantDouble, "Length reduction parameter"),
        IOP("llc", BSIM3v32_MOD_LLC, Variant::VariantDouble, "Length reduction parameter for CV"),
        IOP("lln", BSIM3v32_MOD_LLN, Variant::VariantDouble, "Length reduction parameter"),
        IOP("lw", BSIM3v32_MOD_LW, Variant::VariantDouble, "Length reduction parameter"),
        IOP("lwc", BSIM3v32_MOD_LWC, Variant::VariantDouble, "Length reduction parameter for CV"),
        IOP("lwn", BSIM3v32_MOD_LWN, Variant::VariantDouble, "Length reduction parameter"),
        IOP("lwl", BSIM3v32_MOD_LWL, Variant::VariantDouble, "Length reduction parameter"),
        IOP("lwlc", BSIM3v32_MOD_LWLC, Variant::VariantDouble, "Length reduction parameter for CV"),
        IOP("lmin", BSIM3v32_MOD_LMIN, Variant::VariantDouble, "Minimum length for the model"),
        IOP("lmax", BSIM3v32_MOD_LMAX, Variant::VariantDouble, "Maximum length for the model"),

        IOP("xl", BSIM3v32_MOD_XL, Variant::VariantDouble, "Length correction parameter"),
        IOP("xw", BSIM3v32_MOD_XW, Variant::VariantDouble, "Width correction parameter"),

        IOP("wr", BSIM3v32_MOD_WR, Variant::VariantDouble, "Width dependence of rds"),
        IOP("wint", BSIM3v32_MOD_WINT, Variant::VariantDouble, "Width reduction parameter"),
        IOP("dwg", BSIM3v32_MOD_DWG, Variant::VariantDouble, "Width reduction parameter"),
        IOP("dwb", BSIM3v32_MOD_DWB, Variant::VariantDouble, "Width reduction parameter"),

        IOP("wl", BSIM3v32_MOD_WL, Variant::VariantDouble, "Width reduction parameter"),
        IOP("wlc", BSIM3v32_MOD_WLC, Variant::VariantDouble, "Width reduction parameter for CV"),
        IOP("wln", BSIM3v32_MOD_WLN, Variant::VariantDouble, "Width reduction parameter"),
        IOP("ww", BSIM3v32_MOD_WW, Variant::VariantDouble, "Width reduction parameter"),
        IOP("wwc", BSIM3v32_MOD_WWC, Variant::VariantDouble, "Width reduction parameter for CV"),
        IOP("wwn", BSIM3v32_MOD_WWN, Variant::VariantDouble, "Width reduction parameter"),
        IOP("wwl", BSIM3v32_MOD_WWL, Variant::VariantDouble, "Width reduction parameter"),
        IOP("wwlc", BSIM3v32_MOD_WWLC, Variant::VariantDouble, "Width reduction parameter for CV"),
        IOP("wmin", BSIM3v32_MOD_WMIN, Variant::VariantDouble, "Minimum width for the model"),
        IOP("wmax", BSIM3v32_MOD_WMAX, Variant::VariantDouble, "Maximum width for the model"),

        IOP("b0", BSIM3v32_MOD_B0, Variant::VariantDouble, "Abulk narrow width parameter"),
        IOP("b1", BSIM3v32_MOD_B1, Variant::VariantDouble, "Abulk narrow width parameter"),

        IOP("cgsl", BSIM3v32_MOD_CGSL, Variant::VariantDouble, "New C-V model parameter"),
        IOP("cgdl", BSIM3v32_MOD_CGDL, Variant::VariantDouble, "New C-V model parameter"),
        IOP("ckappa", BSIM3v32_MOD_CKAPPA, Variant::VariantDouble, "New C-V model parameter"),
        IOP("cf", BSIM3v32_MOD_CF, Variant::VariantDouble, "Fringe capacitance parameter"),
        IOP("clc", BSIM3v32_MOD_CLC, Variant::VariantDouble, "Vdsat parameter for C-V model"),
        IOP("cle", BSIM3v32_MOD_CLE, Variant::VariantDouble, "Vdsat parameter for C-V model"),
        IOP("dwc", BSIM3v32_MOD_DWC, Variant::VariantDouble, "Delta W for C-V model"),
        IOP("dlc", BSIM3v32_MOD_DLC, Variant::VariantDouble, "Delta L for C-V model"),

        IOP("hdif", BSIM3v32_MOD_HDIF, Variant::VariantDouble, "ACM Parameter: Distance Gate - contact"),
        IOP("ldif", BSIM3v32_MOD_LDIF, Variant::VariantDouble, "ACM Parameter: Length of LDD Gate-Source/Drain"),
        IOP("ld", BSIM3v32_MOD_LD, Variant::VariantDouble, "ACM Parameter: Length of LDD under Gate"),
        IOP("rd", BSIM3v32_MOD_RD, Variant::VariantDouble, "ACM Parameter: Resistance of LDD drain side"),
        IOP("rs", BSIM3v32_MOD_RS, Variant::VariantDouble, "ACM Parameter: Resistance of LDD source side"),
        IOP("rdc", BSIM3v32_MOD_RDC, Variant::VariantDouble, "ACM Parameter: Resistance contact drain side"),
        IOP("rsc", BSIM3v32_MOD_RSC, Variant::VariantDouble, "ACM Parameter: Resistance contact source side"),
        IOP("wmlt", BSIM3v32_MOD_WMLT, Variant::VariantDouble, "ACM Parameter: Width shrink factor"),

        IOP("lmlt", BSIM3v32_MOD_LMLT, Variant::VariantDouble, "Channel length shrink factor"),

        IOP("alpha0", BSIM3v32_MOD_ALPHA0, Variant::VariantDouble, "substrate current model parameter"),
        IOP("alpha1", BSIM3v32_MOD_ALPHA1, Variant::VariantDouble, "substrate current model parameter"),
        IOP("beta0", BSIM3v32_MOD_BETA0, Variant::VariantDouble, "substrate current model parameter"),
        IOP("ijth", BSIM3v32_MOD_IJTH, Variant::VariantDouble, "Diode limiting current"),

        IOP("lcdsc", BSIM3v32_MOD_LCDSC, Variant::VariantDouble, "Length dependence of cdsc"),
        IOP("lcdscb", BSIM3v32_MOD_LCDSCB, Variant::VariantDouble, "Length dependence of cdscb"),
        IOP("lcdscd", BSIM3v32_MOD_LCDSCD, Variant::VariantDouble, "Length dependence of cdscd"),
        IOP("lcit", BSIM3v32_MOD_LCIT, Variant::VariantDouble, "Length dependence of cit"),
        IOP("lnfactor", BSIM3v32_MOD_LNFACTOR, Variant::VariantDouble, "Length dependence of nfactor"),
        IOP("lxj", BSIM3v32_MOD_LXJ, Variant::VariantDouble, "Length dependence of xj"),
        IOP("lvsat", BSIM3v32_MOD_LVSAT, Variant::VariantDouble, "Length dependence of vsat"),
        IOP("lat", BSIM3v32_MOD_LAT, Variant::VariantDouble, "Length dependence of at"),
        IOP("la0", BSIM3v32_MOD_LA0, Variant::VariantDouble, "Length dependence of a0"),
        IOP("lags", BSIM3v32_MOD_LAGS, Variant::VariantDouble, "Length dependence of ags"),
        IOP("la1", BSIM3v32_MOD_LA1, Variant::VariantDouble, "Length dependence of a1"),
        IOP("la2", BSIM3v32_MOD_LA2, Variant::VariantDouble, "Length dependence of a2"),
        IOP("lketa", BSIM3v32_MOD_LKETA, Variant::VariantDouble, "Length dependence of keta"),
        IOP("lnsub", BSIM3v32_MOD_LNSUB, Variant::VariantDouble, "Length dependence of nsub"),
        IOP("lnch", BSIM3v32_MOD_LNPEAK, Variant::VariantDouble, "Length dependence of nch"),
        IOP("lngate", BSIM3v32_MOD_LNGATE, Variant::VariantDouble, "Length dependence of ngate"),
        IOP("lgamma1", BSIM3v32_MOD_LGAMMA1, Variant::VariantDouble, "Length dependence of gamma1"),
        IOP("lgamma2", BSIM3v32_MOD_LGAMMA2, Variant::VariantDouble, "Length dependence of gamma2"),
        IOP("lvbx", BSIM3v32_MOD_LVBX, Variant::VariantDouble, "Length dependence of vbx"),
        IOP("lvbm", BSIM3v32_MOD_LVBM, Variant::VariantDouble, "Length dependence of vbm"),
        IOP("lxt", BSIM3v32_MOD_LXT, Variant::VariantDouble, "Length dependence of xt"),
        IOP("lk1", BSIM3v32_MOD_LK1, Variant::VariantDouble, "Length dependence of k1"),
        IOP("lkt1", BSIM3v32_MOD_LKT1, Variant::VariantDouble, "Length dependence of kt1"),
        IOP("lkt1l", BSIM3v32_MOD_LKT1L, Variant::VariantDouble, "Length dependence of kt1l"),
        IOP("lkt2", BSIM3v32_MOD_LKT2, Variant::VariantDouble, "Length dependence of kt2"),
        IOP("lk2", BSIM3v32_MOD_LK2, Variant::VariantDouble, "Length dependence of k2"),
        IOP("lk3", BSIM3v32_MOD_LK3, Variant::VariantDouble, "Length dependence of k3"),
        IOP("lk3b", BSIM3v32_MOD_LK3B, Variant::VariantDouble, "Length dependence of k3b"),
        IOP("lw0", BSIM3v32_MOD_LW0, Variant::VariantDouble, "Length dependence of w0"),
        IOP("lnlx", BSIM3v32_MOD_LNLX, Variant::VariantDouble, "Length dependence of nlx"),
        IOP("ldvt0", BSIM3v32_MOD_LDVT0, Variant::VariantDouble, "Length dependence of dvt0"),
        IOP("ldvt1", BSIM3v32_MOD_LDVT1, Variant::VariantDouble, "Length dependence of dvt1"),
        IOP("ldvt2", BSIM3v32_MOD_LDVT2, Variant::VariantDouble, "Length dependence of dvt2"),
        IOP("ldvt0w", BSIM3v32_MOD_LDVT0W, Variant::VariantDouble, "Length dependence of dvt0w"),
        IOP("ldvt1w", BSIM3v32_MOD_LDVT1W, Variant::VariantDouble, "Length dependence of dvt1w"),
        IOP("ldvt2w", BSIM3v32_MOD_LDVT2W, Variant::VariantDouble, "Length dependence of dvt2w"),
        IOP("ldrout", BSIM3v32_MOD_LDROUT, Variant::VariantDouble, "Length dependence of drout"),
        IOP("ldsub", BSIM3v32_MOD_LDSUB, Variant::VariantDouble, "Length dependence of dsub"),
        IOP("lvth0", BSIM3v32_MOD_LVTH0, Variant::VariantDouble, "Length dependence of vth0"),
        IOPR("lvtho", BSIM3v32_MOD_LVTH0, Variant::VariantDouble, "Length dependence of vtho"),
        IOP("lua", BSIM3v32_MOD_LUA, Variant::VariantDouble, "Length dependence of ua"),
        IOP("lua1", BSIM3v32_MOD_LUA1, Variant::VariantDouble, "Length dependence of ua1"),
        IOP("lub", BSIM3v32_MOD_LUB, Variant::VariantDouble, "Length dependence of ub"),
        IOP("lub1", BSIM3v32_MOD_LUB1, Variant::VariantDouble, "Length dependence of ub1"),
        IOP("luc", BSIM3v32_MOD_LUC, Variant::VariantDouble, "Length dependence of uc"),
        IOP("luc1", BSIM3v32_MOD_LUC1, Variant::VariantDouble, "Length dependence of uc1"),
        IOP("lu0", BSIM3v32_MOD_LU0, Variant::VariantDouble, "Length dependence of u0"),
        IOP("lute", BSIM3v32_MOD_LUTE, Variant::VariantDouble, "Length dependence of ute"),
        IOP("lvoff", BSIM3v32_MOD_LVOFF, Variant::VariantDouble, "Length dependence of voff"),
        IOP("lelm", BSIM3v32_MOD_LELM, Variant::VariantDouble, "Length dependence of elm"),
        IOP("ldelta", BSIM3v32_MOD_LDELTA, Variant::VariantDouble, "Length dependence of delta"),
        IOP("lrdsw", BSIM3v32_MOD_LRDSW, Variant::VariantDouble, "Length dependence of rdsw "),

        IOP("lprwg", BSIM3v32_MOD_LPRWG, Variant::VariantDouble, "Length dependence of prwg "),
        IOP("lprwb", BSIM3v32_MOD_LPRWB, Variant::VariantDouble, "Length dependence of prwb "),

        IOP("lprt", BSIM3v32_MOD_LPRT, Variant::VariantDouble, "Length dependence of prt "),
        IOP("leta0", BSIM3v32_MOD_LETA0, Variant::VariantDouble, "Length dependence of eta0"),
        IOP("letab", BSIM3v32_MOD_LETAB, Variant::VariantDouble, "Length dependence of etab"),
        IOP("lpclm", BSIM3v32_MOD_LPCLM, Variant::VariantDouble, "Length dependence of pclm"),
        IOP("lpdiblc1", BSIM3v32_MOD_LPDIBL1, Variant::VariantDouble, "Length dependence of pdiblc1"),
        IOP("lpdiblc2", BSIM3v32_MOD_LPDIBL2, Variant::VariantDouble, "Length dependence of pdiblc2"),
        IOP("lpdiblcb", BSIM3v32_MOD_LPDIBLB, Variant::VariantDouble, "Length dependence of pdiblcb"),
        IOP("lpscbe1", BSIM3v32_MOD_LPSCBE1, Variant::VariantDouble, "Length dependence of pscbe1"),
        IOP("lpscbe2", BSIM3v32_MOD_LPSCBE2, Variant::VariantDouble, "Length dependence of pscbe2"),
        IOP("lpvag", BSIM3v32_MOD_LPVAG, Variant::VariantDouble, "Length dependence of pvag"),
        IOP("lwr", BSIM3v32_MOD_LWR, Variant::VariantDouble, "Length dependence of wr"),
        IOP("ldwg", BSIM3v32_MOD_LDWG, Variant::VariantDouble, "Length dependence of dwg"),
        IOP("ldwb", BSIM3v32_MOD_LDWB, Variant::VariantDouble, "Length dependence of dwb"),
        IOP("lb0", BSIM3v32_MOD_LB0, Variant::VariantDouble, "Length dependence of b0"),
        IOP("lb1", BSIM3v32_MOD_LB1, Variant::VariantDouble, "Length dependence of b1"),
        IOP("lcgsl", BSIM3v32_MOD_LCGSL, Variant::VariantDouble, "Length dependence of cgsl"),
        IOP("lcgdl", BSIM3v32_MOD_LCGDL, Variant::VariantDouble, "Length dependence of cgdl"),
        IOP("lckappa", BSIM3v32_MOD_LCKAPPA, Variant::VariantDouble, "Length dependence of ckappa"),
        IOP("lcf", BSIM3v32_MOD_LCF, Variant::VariantDouble, "Length dependence of cf"),
        IOP("lclc", BSIM3v32_MOD_LCLC, Variant::VariantDouble, "Length dependence of clc"),
        IOP("lcle", BSIM3v32_MOD_LCLE, Variant::VariantDouble, "Length dependence of cle"),
        IOP("lalpha0", BSIM3v32_MOD_LALPHA0, Variant::VariantDouble, "Length dependence of alpha0"),
        IOP("lalpha1", BSIM3v32_MOD_LALPHA1, Variant::VariantDouble, "Length dependence of alpha1"),
        IOP("lbeta0", BSIM3v32_MOD_LBETA0, Variant::VariantDouble, "Length dependence of beta0"),
        IOP("lvfbcv", BSIM3v32_MOD_LVFBCV, Variant::VariantDouble, "Length dependence of vfbcv"),
        IOP("lvfb", BSIM3v32_MOD_LVFB, Variant::VariantDouble, "Length dependence of vfb"),
        IOP("lacde", BSIM3v32_MOD_LACDE, Variant::VariantDouble, "Length dependence of acde"),
        IOP("lmoin", BSIM3v32_MOD_LMOIN, Variant::VariantDouble, "Length dependence of moin"),
        IOP("lnoff", BSIM3v32_MOD_LNOFF, Variant::VariantDouble, "Length dependence of noff"),
        IOP("lvoffcv", BSIM3v32_MOD_LVOFFCV, Variant::VariantDouble, "Length dependence of voffcv"),
        IOP("wcdsc", BSIM3v32_MOD_WCDSC, Variant::VariantDouble, "Width dependence of cdsc"),
        IOP("wcdscb", BSIM3v32_MOD_WCDSCB, Variant::VariantDouble, "Width dependence of cdscb"),
        IOP("wcdscd", BSIM3v32_MOD_WCDSCD, Variant::VariantDouble, "Width dependence of cdscd"),
        IOP("wcit", BSIM3v32_MOD_WCIT, Variant::VariantDouble, "Width dependence of cit"),
        IOP("wnfactor", BSIM3v32_MOD_WNFACTOR, Variant::VariantDouble, "Width dependence of nfactor"),
        IOP("wxj", BSIM3v32_MOD_WXJ, Variant::VariantDouble, "Width dependence of xj"),
        IOP("wvsat", BSIM3v32_MOD_WVSAT, Variant::VariantDouble, "Width dependence of vsat"),
        IOP("wat", BSIM3v32_MOD_WAT, Variant::VariantDouble, "Width dependence of at"),
        IOP("wa0", BSIM3v32_MOD_WA0, Variant::VariantDouble, "Width dependence of a0"),
        IOP("wags", BSIM3v32_MOD_WAGS, Variant::VariantDouble, "Width dependence of ags"),
        IOP("wa1", BSIM3v32_MOD_WA1, Variant::VariantDouble, "Width dependence of a1"),
        IOP("wa2", BSIM3v32_MOD_WA2, Variant::VariantDouble, "Width dependence of a2"),
        IOP("wketa", BSIM3v32_MOD_WKETA, Variant::VariantDouble, "Width dependence of keta"),
        IOP("wnsub", BSIM3v32_MOD_WNSUB, Variant::VariantDouble, "Width dependence of nsub"),
        IOP("wnch", BSIM3v32_MOD_WNPEAK, Variant::VariantDouble, "Width dependence of nch"),
        IOP("wngate", BSIM3v32_MOD_WNGATE, Variant::VariantDouble, "Width dependence of ngate"),
        IOP("wgamma1", BSIM3v32_MOD_WGAMMA1, Variant::VariantDouble, "Width dependence of gamma1"),
        IOP("wgamma2", BSIM3v32_MOD_WGAMMA2, Variant::VariantDouble, "Width dependence of gamma2"),
        IOP("wvbx", BSIM3v32_MOD_WVBX, Variant::VariantDouble, "Width dependence of vbx"),
        IOP("wvbm", BSIM3v32_MOD_WVBM, Variant::VariantDouble, "Width dependence of vbm"),
        IOP("wxt", BSIM3v32_MOD_WXT, Variant::VariantDouble, "Width dependence of xt"),
        IOP("wk1", BSIM3v32_MOD_WK1, Variant::VariantDouble, "Width dependence of k1"),
        IOP("wkt1", BSIM3v32_MOD_WKT1, Variant::VariantDouble, "Width dependence of kt1"),
        IOP("wkt1l", BSIM3v32_MOD_WKT1L, Variant::VariantDouble, "Width dependence of kt1l"),
        IOP("wkt2", BSIM3v32_MOD_WKT2, Variant::VariantDouble, "Width dependence of kt2"),
        IOP("wk2", BSIM3v32_MOD_WK2, Variant::VariantDouble, "Width dependence of k2"),
        IOP("wk3", BSIM3v32_MOD_WK3, Variant::VariantDouble, "Width dependence of k3"),
        IOP("wk3b", BSIM3v32_MOD_WK3B, Variant::VariantDouble, "Width dependence of k3b"),
        IOP("ww0", BSIM3v32_MOD_WW0, Variant::VariantDouble, "Width dependence of w0"),
        IOP("wnlx", BSIM3v32_MOD_WNLX, Variant::VariantDouble, "Width dependence of nlx"),
        IOP("wdvt0", BSIM3v32_MOD_WDVT0, Variant::VariantDouble, "Width dependence of dvt0"),
        IOP("wdvt1", BSIM3v32_MOD_WDVT1, Variant::VariantDouble, "Width dependence of dvt1"),
        IOP("wdvt2", BSIM3v32_MOD_WDVT2, Variant::VariantDouble, "Width dependence of dvt2"),
        IOP("wdvt0w", BSIM3v32_MOD_WDVT0W, Variant::VariantDouble, "Width dependence of dvt0w"),
        IOP("wdvt1w", BSIM3v32_MOD_WDVT1W, Variant::VariantDouble, "Width dependence of dvt1w"),
        IOP("wdvt2w", BSIM3v32_MOD_WDVT2W, Variant::VariantDouble, "Width dependence of dvt2w"),
        IOP("wdrout", BSIM3v32_MOD_WDROUT, Variant::VariantDouble, "Width dependence of drout"),
        IOP("wdsub", BSIM3v32_MOD_WDSUB, Variant::VariantDouble, "Width dependence of dsub"),
        IOP("wvth0", BSIM3v32_MOD_WVTH0, Variant::VariantDouble, "Width dependence of vth0"),
        IOPR("wvtho", BSIM3v32_MOD_WVTH0, Variant::VariantDouble, "Width dependence of vtho"),
        IOP("wua", BSIM3v32_MOD_WUA, Variant::VariantDouble, "Width dependence of ua"),
        IOP("wua1", BSIM3v32_MOD_WUA1, Variant::VariantDouble, "Width dependence of ua1"),
        IOP("wub", BSIM3v32_MOD_WUB, Variant::VariantDouble, "Width dependence of ub"),
        IOP("wub1", BSIM3v32_MOD_WUB1, Variant::VariantDouble, "Width dependence of ub1"),
        IOP("wuc", BSIM3v32_MOD_WUC, Variant::VariantDouble, "Width dependence of uc"),
        IOP("wuc1", BSIM3v32_MOD_WUC1, Variant::VariantDouble, "Width dependence of uc1"),
        IOP("wu0", BSIM3v32_MOD_WU0, Variant::VariantDouble, "Width dependence of u0"),
        IOP("wute", BSIM3v32_MOD_WUTE, Variant::VariantDouble, "Width dependence of ute"),
        IOP("wvoff", BSIM3v32_MOD_WVOFF, Variant::VariantDouble, "Width dependence of voff"),
        IOP("welm", BSIM3v32_MOD_WELM, Variant::VariantDouble, "Width dependence of elm"),
        IOP("wdelta", BSIM3v32_MOD_WDELTA, Variant::VariantDouble, "Width dependence of delta"),
        IOP("wrdsw", BSIM3v32_MOD_WRDSW, Variant::VariantDouble, "Width dependence of rdsw "),

        IOP("wprwg", BSIM3v32_MOD_WPRWG, Variant::VariantDouble, "Width dependence of prwg "),
        IOP("wprwb", BSIM3v32_MOD_WPRWB, Variant::VariantDouble, "Width dependence of prwb "),

        IOP("wprt", BSIM3v32_MOD_WPRT, Variant::VariantDouble, "Width dependence of prt"),
        IOP("weta0", BSIM3v32_MOD_WETA0, Variant::VariantDouble, "Width dependence of eta0"),
        IOP("wetab", BSIM3v32_MOD_WETAB, Variant::VariantDouble, "Width dependence of etab"),
        IOP("wpclm", BSIM3v32_MOD_WPCLM, Variant::VariantDouble, "Width dependence of pclm"),
        IOP("wpdiblc1", BSIM3v32_MOD_WPDIBL1, Variant::VariantDouble, "Width dependence of pdiblc1"),
        IOP("wpdiblc2", BSIM3v32_MOD_WPDIBL2, Variant::VariantDouble, "Width dependence of pdiblc2"),
        IOP("wpdiblcb", BSIM3v32_MOD_WPDIBLB, Variant::VariantDouble, "Width dependence of pdiblcb"),
        IOP("wpscbe1", BSIM3v32_MOD_WPSCBE1, Variant::VariantDouble, "Width dependence of pscbe1"),
        IOP("wpscbe2", BSIM3v32_MOD_WPSCBE2, Variant::VariantDouble, "Width dependence of pscbe2"),
        IOP("wpvag", BSIM3v32_MOD_WPVAG, Variant::VariantDouble, "Width dependence of pvag"),
        IOP("wwr", BSIM3v32_MOD_WWR, Variant::VariantDouble, "Width dependence of wr"),
        IOP("wdwg", BSIM3v32_MOD_WDWG, Variant::VariantDouble, "Width dependence of dwg"),
        IOP("wdwb", BSIM3v32_MOD_WDWB, Variant::VariantDouble, "Width dependence of dwb"),
        IOP("wb0", BSIM3v32_MOD_WB0, Variant::VariantDouble, "Width dependence of b0"),
        IOP("wb1", BSIM3v32_MOD_WB1, Variant::VariantDouble, "Width dependence of b1"),
        IOP("wcgsl", BSIM3v32_MOD_WCGSL, Variant::VariantDouble, "Width dependence of cgsl"),
        IOP("wcgdl", BSIM3v32_MOD_WCGDL, Variant::VariantDouble, "Width dependence of cgdl"),
        IOP("wckappa", BSIM3v32_MOD_WCKAPPA, Variant::VariantDouble, "Width dependence of ckappa"),
        IOP("wcf", BSIM3v32_MOD_WCF, Variant::VariantDouble, "Width dependence of cf"),
        IOP("wclc", BSIM3v32_MOD_WCLC, Variant::VariantDouble, "Width dependence of clc"),
        IOP("wcle", BSIM3v32_MOD_WCLE, Variant::VariantDouble, "Width dependence of cle"),
        IOP("walpha0", BSIM3v32_MOD_WALPHA0, Variant::VariantDouble, "Width dependence of alpha0"),
        IOP("walpha1", BSIM3v32_MOD_WALPHA1, Variant::VariantDouble, "Width dependence of alpha1"),
        IOP("wbeta0", BSIM3v32_MOD_WBETA0, Variant::VariantDouble, "Width dependence of beta0"),
        IOP("wvfbcv", BSIM3v32_MOD_WVFBCV, Variant::VariantDouble, "Width dependence of vfbcv"),
        IOP("wvfb", BSIM3v32_MOD_WVFB, Variant::VariantDouble, "Width dependence of vfb"),
        IOP("wacde", BSIM3v32_MOD_WACDE, Variant::VariantDouble, "Width dependence of acde"),
        IOP("wmoin", BSIM3v32_MOD_WMOIN, Variant::VariantDouble, "Width dependence of moin"),
        IOP("wnoff", BSIM3v32_MOD_WNOFF, Variant::VariantDouble, "Width dependence of noff"),
        IOP("wvoffcv", BSIM3v32_MOD_WVOFFCV, Variant::VariantDouble, "Width dependence of voffcv"),

        IOP("pcdsc", BSIM3v32_MOD_PCDSC, Variant::VariantDouble, "Cross-term dependence of cdsc"),
        IOP("pcdscb", BSIM3v32_MOD_PCDSCB, Variant::VariantDouble, "Cross-term dependence of cdscb"),
        IOP("pcdscd", BSIM3v32_MOD_PCDSCD, Variant::VariantDouble, "Cross-term dependence of cdscd"),
        IOP("pcit", BSIM3v32_MOD_PCIT, Variant::VariantDouble, "Cross-term dependence of cit"),
        IOP("pnfactor", BSIM3v32_MOD_PNFACTOR, Variant::VariantDouble, "Cross-term dependence of nfactor"),
        IOP("pxj", BSIM3v32_MOD_PXJ, Variant::VariantDouble, "Cross-term dependence of xj"),
        IOP("pvsat", BSIM3v32_MOD_PVSAT, Variant::VariantDouble, "Cross-term dependence of vsat"),
        IOP("pat", BSIM3v32_MOD_PAT, Variant::VariantDouble, "Cross-term dependence of at"),
        IOP("pa0", BSIM3v32_MOD_PA0, Variant::VariantDouble, "Cross-term dependence of a0"),
        IOP("pags", BSIM3v32_MOD_PAGS, Variant::VariantDouble, "Cross-term dependence of ags"),
        IOP("pa1", BSIM3v32_MOD_PA1, Variant::VariantDouble, "Cross-term dependence of a1"),
        IOP("pa2", BSIM3v32_MOD_PA2, Variant::VariantDouble, "Cross-term dependence of a2"),
        IOP("pketa", BSIM3v32_MOD_PKETA, Variant::VariantDouble, "Cross-term dependence of keta"),
        IOP("pnsub", BSIM3v32_MOD_PNSUB, Variant::VariantDouble, "Cross-term dependence of nsub"),
        IOP("pnch", BSIM3v32_MOD_PNPEAK, Variant::VariantDouble, "Cross-term dependence of nch"),
        IOP("pngate", BSIM3v32_MOD_PNGATE, Variant::VariantDouble, "Cross-term dependence of ngate"),
        IOP("pgamma1", BSIM3v32_MOD_PGAMMA1, Variant::VariantDouble, "Cross-term dependence of gamma1"),
        IOP("pgamma2", BSIM3v32_MOD_PGAMMA2, Variant::VariantDouble, "Cross-term dependence of gamma2"),
        IOP("pvbx", BSIM3v32_MOD_PVBX, Variant::VariantDouble, "Cross-term dependence of vbx"),
        IOP("pvbm", BSIM3v32_MOD_PVBM, Variant::VariantDouble, "Cross-term dependence of vbm"),
        IOP("pxt", BSIM3v32_MOD_PXT, Variant::VariantDouble, "Cross-term dependence of xt"),
        IOP("pk1", BSIM3v32_MOD_PK1, Variant::VariantDouble, "Cross-term dependence of k1"),
        IOP("pkt1", BSIM3v32_MOD_PKT1, Variant::VariantDouble, "Cross-term dependence of kt1"),
        IOP("pkt1l", BSIM3v32_MOD_PKT1L, Variant::VariantDouble, "Cross-term dependence of kt1l"),
        IOP("pkt2", BSIM3v32_MOD_PKT2, Variant::VariantDouble, "Cross-term dependence of kt2"),
        IOP("pk2", BSIM3v32_MOD_PK2, Variant::VariantDouble, "Cross-term dependence of k2"),
        IOP("pk3", BSIM3v32_MOD_PK3, Variant::VariantDouble, "Cross-term dependence of k3"),
        IOP("pk3b", BSIM3v32_MOD_PK3B, Variant::VariantDouble, "Cross-term dependence of k3b"),
        IOP("pw0", BSIM3v32_MOD_PW0, Variant::VariantDouble, "Cross-term dependence of w0"),
        IOP("pnlx", BSIM3v32_MOD_PNLX, Variant::VariantDouble, "Cross-term dependence of nlx"),
        IOP("pdvt0", BSIM3v32_MOD_PDVT0, Variant::VariantDouble, "Cross-term dependence of dvt0"),
        IOP("pdvt1", BSIM3v32_MOD_PDVT1, Variant::VariantDouble, "Cross-term dependence of dvt1"),
        IOP("pdvt2", BSIM3v32_MOD_PDVT2, Variant::VariantDouble, "Cross-term dependence of dvt2"),
        IOP("pdvt0w", BSIM3v32_MOD_PDVT0W, Variant::VariantDouble, "Cross-term dependence of dvt0w"),
        IOP("pdvt1w", BSIM3v32_MOD_PDVT1W, Variant::VariantDouble, "Cross-term dependence of dvt1w"),
        IOP("pdvt2w", BSIM3v32_MOD_PDVT2W, Variant::VariantDouble, "Cross-term dependence of dvt2w"),
        IOP("pdrout", BSIM3v32_MOD_PDROUT, Variant::VariantDouble, "Cross-term dependence of drout"),
        IOP("pdsub", BSIM3v32_MOD_PDSUB, Variant::VariantDouble, "Cross-term dependence of dsub"),
        IOP("pvth0", BSIM3v32_MOD_PVTH0, Variant::VariantDouble, "Cross-term dependence of vth0"),
        IOPR("pvtho", BSIM3v32_MOD_PVTH0, Variant::VariantDouble, "Cross-term dependence of vtho"),
        IOP("pua", BSIM3v32_MOD_PUA, Variant::VariantDouble, "Cross-term dependence of ua"),
        IOP("pua1", BSIM3v32_MOD_PUA1, Variant::VariantDouble, "Cross-term dependence of ua1"),
        IOP("pub", BSIM3v32_MOD_PUB, Variant::VariantDouble, "Cross-term dependence of ub"),
        IOP("pub1", BSIM3v32_MOD_PUB1, Variant::VariantDouble, "Cross-term dependence of ub1"),
        IOP("puc", BSIM3v32_MOD_PUC, Variant::VariantDouble, "Cross-term dependence of uc"),
        IOP("puc1", BSIM3v32_MOD_PUC1, Variant::VariantDouble, "Cross-term dependence of uc1"),
        IOP("pu0", BSIM3v32_MOD_PU0, Variant::VariantDouble, "Cross-term dependence of u0"),
        IOP("pute", BSIM3v32_MOD_PUTE, Variant::VariantDouble, "Cross-term dependence of ute"),
        IOP("pvoff", BSIM3v32_MOD_PVOFF, Variant::VariantDouble, "Cross-term dependence of voff"),
        IOP("pelm", BSIM3v32_MOD_PELM, Variant::VariantDouble, "Cross-term dependence of elm"),
        IOP("pdelta", BSIM3v32_MOD_PDELTA, Variant::VariantDouble, "Cross-term dependence of delta"),
        IOP("prdsw", BSIM3v32_MOD_PRDSW, Variant::VariantDouble, "Cross-term dependence of rdsw "),

        IOP("pprwg", BSIM3v32_MOD_PPRWG, Variant::VariantDouble, "Cross-term dependence of prwg "),
        IOP("pprwb", BSIM3v32_MOD_PPRWB, Variant::VariantDouble, "Cross-term dependence of prwb "),

        IOP("pprt", BSIM3v32_MOD_PPRT, Variant::VariantDouble, "Cross-term dependence of prt "),
        IOP("peta0", BSIM3v32_MOD_PETA0, Variant::VariantDouble, "Cross-term dependence of eta0"),
        IOP("petab", BSIM3v32_MOD_PETAB, Variant::VariantDouble, "Cross-term dependence of etab"),
        IOP("ppclm", BSIM3v32_MOD_PPCLM, Variant::VariantDouble, "Cross-term dependence of pclm"),
        IOP("ppdiblc1", BSIM3v32_MOD_PPDIBL1, Variant::VariantDouble, "Cross-term dependence of pdiblc1"),
        IOP("ppdiblc2", BSIM3v32_MOD_PPDIBL2, Variant::VariantDouble, "Cross-term dependence of pdiblc2"),
        IOP("ppdiblcb", BSIM3v32_MOD_PPDIBLB, Variant::VariantDouble, "Cross-term dependence of pdiblcb"),
        IOP("ppscbe1", BSIM3v32_MOD_PPSCBE1, Variant::VariantDouble, "Cross-term dependence of pscbe1"),
        IOP("ppscbe2", BSIM3v32_MOD_PPSCBE2, Variant::VariantDouble, "Cross-term dependence of pscbe2"),
        IOP("ppvag", BSIM3v32_MOD_PPVAG, Variant::VariantDouble, "Cross-term dependence of pvag"),
        IOP("pwr", BSIM3v32_MOD_PWR, Variant::VariantDouble, "Cross-term dependence of wr"),
        IOP("pdwg", BSIM3v32_MOD_PDWG, Variant::VariantDouble, "Cross-term dependence of dwg"),
        IOP("pdwb", BSIM3v32_MOD_PDWB, Variant::VariantDouble, "Cross-term dependence of dwb"),
        IOP("pb0", BSIM3v32_MOD_PB0, Variant::VariantDouble, "Cross-term dependence of b0"),
        IOP("pb1", BSIM3v32_MOD_PB1, Variant::VariantDouble, "Cross-term dependence of b1"),
        IOP("pcgsl", BSIM3v32_MOD_PCGSL, Variant::VariantDouble, "Cross-term dependence of cgsl"),
        IOP("pcgdl", BSIM3v32_MOD_PCGDL, Variant::VariantDouble, "Cross-term dependence of cgdl"),
        IOP("pckappa", BSIM3v32_MOD_PCKAPPA, Variant::VariantDouble, "Cross-term dependence of ckappa"),
        IOP("pcf", BSIM3v32_MOD_PCF, Variant::VariantDouble, "Cross-term dependence of cf"),
        IOP("pclc", BSIM3v32_MOD_PCLC, Variant::VariantDouble, "Cross-term dependence of clc"),
        IOP("pcle", BSIM3v32_MOD_PCLE, Variant::VariantDouble, "Cross-term dependence of cle"),
        IOP("palpha0", BSIM3v32_MOD_PALPHA0, Variant::VariantDouble, "Cross-term dependence of alpha0"),
        IOP("palpha1", BSIM3v32_MOD_PALPHA1, Variant::VariantDouble, "Cross-term dependence of alpha1"),
        IOP("pbeta0", BSIM3v32_MOD_PBETA0, Variant::VariantDouble, "Cross-term dependence of beta0"),
        IOP("pvfbcv", BSIM3v32_MOD_PVFBCV, Variant::VariantDouble, "Cross-term dependence of vfbcv"),
        IOP("pvfb", BSIM3v32_MOD_PVFB, Variant::VariantDouble, "Cross-term dependence of vfb"),
        IOP("pacde", BSIM3v32_MOD_PACDE, Variant::VariantDouble, "Cross-term dependence of acde"),
        IOP("pmoin", BSIM3v32_MOD_PMOIN, Variant::VariantDouble, "Cross-term dependence of moin"),
        IOP("pnoff", BSIM3v32_MOD_PNOFF, Variant::VariantDouble, "Cross-term dependence of noff"),
        IOP("pvoffcv", BSIM3v32_MOD_PVOFFCV, Variant::VariantDouble, "Cross-term dependence of voffcv"),

        IOP("noia", BSIM3v32_MOD_NOIA, Variant::VariantDouble, "Flicker noise parameter"),
        IOP("noib", BSIM3v32_MOD_NOIB, Variant::VariantDouble, "Flicker noise parameter"),
        IOP("noic", BSIM3v32_MOD_NOIC, Variant::VariantDouble, "Flicker noise parameter"),
        IOP("em", BSIM3v32_MOD_EM, Variant::VariantDouble, "Flicker noise parameter"),
        IOP("ef", BSIM3v32_MOD_EF, Variant::VariantDouble, "Flicker noise frequency exponent"),
        IOP("af", BSIM3v32_MOD_AF, Variant::VariantDouble, "Flicker noise exponent"),
        IOP("kf", BSIM3v32_MOD_KF, Variant::VariantDouble, "Flicker noise coefficient"),

        IOP("vgs_max", BSIM3v32_MOD_VGS_MAX, Variant::VariantDouble, "maximum voltage G-S branch"),
        IOP("vgd_max", BSIM3v32_MOD_VGD_MAX, Variant::VariantDouble, "maximum voltage G-D branch"),
        IOP("vgb_max", BSIM3v32_MOD_VGB_MAX, Variant::VariantDouble, "maximum voltage G-B branch"),
        IOP("vds_max", BSIM3v32_MOD_VDS_MAX, Variant::VariantDouble, "maximum voltage D-S branch"),
        IOP("vbs_max", BSIM3v32_MOD_VBS_MAX, Variant::VariantDouble, "maximum voltage B-S branch"),
        IOP("vbd_max", BSIM3v32_MOD_VBD_MAX, Variant::VariantDouble, "maximum voltage B-D branch"),
        IOP("vgsr_max", BSIM3v32_MOD_VGSR_MAX, Variant::VariantDouble, "maximum voltage G-S branch"),
        IOP("vgdr_max", BSIM3v32_MOD_VGDR_MAX, Variant::VariantDouble, "maximum voltage G-D branch"),
        IOP("vgbr_max", BSIM3v32_MOD_VGBR_MAX, Variant::VariantDouble, "maximum voltage G-B branch"),
        IOP("vbsr_max", BSIM3v32_MOD_VBSR_MAX, Variant::VariantDouble, "maximum voltage B-S branch"),
        IOP("vbdr_max", BSIM3v32_MOD_VBDR_MAX, Variant::VariantDouble, "maximum voltage B-D branch"),

        IP("nmos", BSIM3v32_MOD_NMOS, Variant::VariantBoolean, "Flag to indicate NMOS"),
        IP("pmos", BSIM3v32_MOD_PMOS, Variant::VariantBoolean, "Flag to indicate PMOS"),
    };

    BSIM3v32model::BSIM3v32model()
        : BSIM3v32type(0),

          BSIM3v32mobMod(0),
          BSIM3v32capMod(0),
          BSIM3v32acmMod(0),
          BSIM3v32calcacm(0),
          BSIM3v32noiMod(0),
          BSIM3v32nqsMod(0),
          BSIM3v32binUnit(0),
          BSIM3v32paramChk(0),
          BSIM3v32version(nullptr),
          BSIM3v32intVersion(0),
          BSIM3v32tox(0),
          BSIM3v32toxm(0),
          BSIM3v32cdsc(0),
          BSIM3v32cdscb(0),
          BSIM3v32cdscd(0),
          BSIM3v32cit(0),
          BSIM3v32nfactor(0),
          BSIM3v32xj(0),
          BSIM3v32vsat(0),
          BSIM3v32at(0),
          BSIM3v32a0(0),
          BSIM3v32ags(0),
          BSIM3v32a1(0),
          BSIM3v32a2(0),
          BSIM3v32keta(0),
          BSIM3v32nsub(0),
          BSIM3v32npeak(0),
          BSIM3v32ngate(0),
          BSIM3v32gamma1(0),
          BSIM3v32gamma2(0),
          BSIM3v32vbx(0),
          BSIM3v32vbm(0),
          BSIM3v32xt(0),
          BSIM3v32k1(0),
          BSIM3v32kt1(0),
          BSIM3v32kt1l(0),
          BSIM3v32kt2(0),
          BSIM3v32k2(0),
          BSIM3v32k3(0),
          BSIM3v32k3b(0),
          BSIM3v32w0(0),
          BSIM3v32nlx(0),
          BSIM3v32dvt0(0),
          BSIM3v32dvt1(0),
          BSIM3v32dvt2(0),
          BSIM3v32dvt0w(0),
          BSIM3v32dvt1w(0),
          BSIM3v32dvt2w(0),
          BSIM3v32drout(0),
          BSIM3v32dsub(0),
          BSIM3v32vth0(0),
          BSIM3v32ua(0),
          BSIM3v32ua1(0),
          BSIM3v32ub(0),
          BSIM3v32ub1(0),
          BSIM3v32uc(0),
          BSIM3v32uc1(0),
          BSIM3v32u0(0),
          BSIM3v32ute(0),
          BSIM3v32voff(0),
          BSIM3v32delta(0),
          BSIM3v32rdsw(0),
          BSIM3v32prwg(0),
          BSIM3v32prwb(0),
          BSIM3v32prt(0),
          BSIM3v32eta0(0),
          BSIM3v32etab(0),
          BSIM3v32pclm(0),
          BSIM3v32pdibl1(0),
          BSIM3v32pdibl2(0),
          BSIM3v32pdiblb(0),
          BSIM3v32pscbe1(0),
          BSIM3v32pscbe2(0),
          BSIM3v32pvag(0),
          BSIM3v32wr(0),
          BSIM3v32dwg(0),
          BSIM3v32dwb(0),
          BSIM3v32b0(0),
          BSIM3v32b1(0),
          BSIM3v32alpha0(0),
          BSIM3v32alpha1(0),
          BSIM3v32beta0(0),
          BSIM3v32ijth(0),
          BSIM3v32vfb(0),

          /* CV model */
          BSIM3v32elm(0),
          BSIM3v32cgsl(0),
          BSIM3v32cgdl(0),
          BSIM3v32ckappa(0),
          BSIM3v32cf(0),
          BSIM3v32vfbcv(0),
          BSIM3v32clc(0),
          BSIM3v32cle(0),
          BSIM3v32dwc(0),
          BSIM3v32dlc(0),
          BSIM3v32noff(0),
          BSIM3v32voffcv(0),
          BSIM3v32acde(0),
          BSIM3v32moin(0),
          BSIM3v32tcj(0),
          BSIM3v32tcjsw(0),
          BSIM3v32tcjswg(0),
          BSIM3v32tpb(0),
          BSIM3v32tpbsw(0),
          BSIM3v32tpbswg(0),

          /* ACM model */
          BSIM3v32xl(0),
          BSIM3v32xw(0),
          BSIM3v32hdif(0),
          BSIM3v32ldif(0),
          BSIM3v32ld(0),
          BSIM3v32rd(0),
          BSIM3v32rs(0),
          BSIM3v32rdc(0),
          BSIM3v32rsc(0),
          BSIM3v32wmlt(0),

          BSIM3v32lmlt(0),

          /* Length Dependence */
          BSIM3v32lcdsc(0),
          BSIM3v32lcdscb(0),
          BSIM3v32lcdscd(0),
          BSIM3v32lcit(0),
          BSIM3v32lnfactor(0),
          BSIM3v32lxj(0),
          BSIM3v32lvsat(0),
          BSIM3v32lat(0),
          BSIM3v32la0(0),
          BSIM3v32lags(0),
          BSIM3v32la1(0),
          BSIM3v32la2(0),
          BSIM3v32lketa(0),
          BSIM3v32lnsub(0),
          BSIM3v32lnpeak(0),
          BSIM3v32lngate(0),
          BSIM3v32lgamma1(0),
          BSIM3v32lgamma2(0),
          BSIM3v32lvbx(0),
          BSIM3v32lvbm(0),
          BSIM3v32lxt(0),
          BSIM3v32lk1(0),
          BSIM3v32lkt1(0),
          BSIM3v32lkt1l(0),
          BSIM3v32lkt2(0),
          BSIM3v32lk2(0),
          BSIM3v32lk3(0),
          BSIM3v32lk3b(0),
          BSIM3v32lw0(0),
          BSIM3v32lnlx(0),
          BSIM3v32ldvt0(0),
          BSIM3v32ldvt1(0),
          BSIM3v32ldvt2(0),
          BSIM3v32ldvt0w(0),
          BSIM3v32ldvt1w(0),
          BSIM3v32ldvt2w(0),
          BSIM3v32ldrout(0),
          BSIM3v32ldsub(0),
          BSIM3v32lvth0(0),
          BSIM3v32lua(0),
          BSIM3v32lua1(0),
          BSIM3v32lub(0),
          BSIM3v32lub1(0),
          BSIM3v32luc(0),
          BSIM3v32luc1(0),
          BSIM3v32lu0(0),
          BSIM3v32lute(0),
          BSIM3v32lvoff(0),
          BSIM3v32ldelta(0),
          BSIM3v32lrdsw(0),
          BSIM3v32lprwg(0),
          BSIM3v32lprwb(0),
          BSIM3v32lprt(0),
          BSIM3v32leta0(0),
          BSIM3v32letab(0),
          BSIM3v32lpclm(0),
          BSIM3v32lpdibl1(0),
          BSIM3v32lpdibl2(0),
          BSIM3v32lpdiblb(0),
          BSIM3v32lpscbe1(0),
          BSIM3v32lpscbe2(0),
          BSIM3v32lpvag(0),
          BSIM3v32lwr(0),
          BSIM3v32ldwg(0),
          BSIM3v32ldwb(0),
          BSIM3v32lb0(0),
          BSIM3v32lb1(0),
          BSIM3v32lalpha0(0),
          BSIM3v32lalpha1(0),
          BSIM3v32lbeta0(0),
          BSIM3v32lvfb(0),

          /* CV model */
          BSIM3v32lelm(0),
          BSIM3v32lcgsl(0),
          BSIM3v32lcgdl(0),
          BSIM3v32lckappa(0),
          BSIM3v32lcf(0),
          BSIM3v32lclc(0),
          BSIM3v32lcle(0),
          BSIM3v32lvfbcv(0),
          BSIM3v32lnoff(0),
          BSIM3v32lvoffcv(0),
          BSIM3v32lacde(0),
          BSIM3v32lmoin(0),

          /* Width Dependence */
          BSIM3v32wcdsc(0),
          BSIM3v32wcdscb(0),
          BSIM3v32wcdscd(0),
          BSIM3v32wcit(0),
          BSIM3v32wnfactor(0),
          BSIM3v32wxj(0),
          BSIM3v32wvsat(0),
          BSIM3v32wat(0),
          BSIM3v32wa0(0),
          BSIM3v32wags(0),
          BSIM3v32wa1(0),
          BSIM3v32wa2(0),
          BSIM3v32wketa(0),
          BSIM3v32wnsub(0),
          BSIM3v32wnpeak(0),
          BSIM3v32wngate(0),
          BSIM3v32wgamma1(0),
          BSIM3v32wgamma2(0),
          BSIM3v32wvbx(0),
          BSIM3v32wvbm(0),
          BSIM3v32wxt(0),
          BSIM3v32wk1(0),
          BSIM3v32wkt1(0),
          BSIM3v32wkt1l(0),
          BSIM3v32wkt2(0),
          BSIM3v32wk2(0),
          BSIM3v32wk3(0),
          BSIM3v32wk3b(0),
          BSIM3v32ww0(0),
          BSIM3v32wnlx(0),
          BSIM3v32wdvt0(0),
          BSIM3v32wdvt1(0),
          BSIM3v32wdvt2(0),
          BSIM3v32wdvt0w(0),
          BSIM3v32wdvt1w(0),
          BSIM3v32wdvt2w(0),
          BSIM3v32wdrout(0),
          BSIM3v32wdsub(0),
          BSIM3v32wvth0(0),
          BSIM3v32wua(0),
          BSIM3v32wua1(0),
          BSIM3v32wub(0),
          BSIM3v32wub1(0),
          BSIM3v32wuc(0),
          BSIM3v32wuc1(0),
          BSIM3v32wu0(0),
          BSIM3v32wute(0),
          BSIM3v32wvoff(0),
          BSIM3v32wdelta(0),
          BSIM3v32wrdsw(0),
          BSIM3v32wprwg(0),
          BSIM3v32wprwb(0),
          BSIM3v32wprt(0),
          BSIM3v32weta0(0),
          BSIM3v32wetab(0),
          BSIM3v32wpclm(0),
          BSIM3v32wpdibl1(0),
          BSIM3v32wpdibl2(0),
          BSIM3v32wpdiblb(0),
          BSIM3v32wpscbe1(0),
          BSIM3v32wpscbe2(0),
          BSIM3v32wpvag(0),
          BSIM3v32wwr(0),
          BSIM3v32wdwg(0),
          BSIM3v32wdwb(0),
          BSIM3v32wb0(0),
          BSIM3v32wb1(0),
          BSIM3v32walpha0(0),
          BSIM3v32walpha1(0),
          BSIM3v32wbeta0(0),
          BSIM3v32wvfb(0),

          /* CV model */
          BSIM3v32welm(0),
          BSIM3v32wcgsl(0),
          BSIM3v32wcgdl(0),
          BSIM3v32wckappa(0),
          BSIM3v32wcf(0),
          BSIM3v32wclc(0),
          BSIM3v32wcle(0),
          BSIM3v32wvfbcv(0),
          BSIM3v32wnoff(0),
          BSIM3v32wvoffcv(0),
          BSIM3v32wacde(0),
          BSIM3v32wmoin(0),

          /* Cross-term Dependence */
          BSIM3v32pcdsc(0),
          BSIM3v32pcdscb(0),
          BSIM3v32pcdscd(0),
          BSIM3v32pcit(0),
          BSIM3v32pnfactor(0),
          BSIM3v32pxj(0),
          BSIM3v32pvsat(0),
          BSIM3v32pat(0),
          BSIM3v32pa0(0),
          BSIM3v32pags(0),
          BSIM3v32pa1(0),
          BSIM3v32pa2(0),
          BSIM3v32pketa(0),
          BSIM3v32pnsub(0),
          BSIM3v32pnpeak(0),
          BSIM3v32pngate(0),
          BSIM3v32pgamma1(0),
          BSIM3v32pgamma2(0),
          BSIM3v32pvbx(0),
          BSIM3v32pvbm(0),
          BSIM3v32pxt(0),
          BSIM3v32pk1(0),
          BSIM3v32pkt1(0),
          BSIM3v32pkt1l(0),
          BSIM3v32pkt2(0),
          BSIM3v32pk2(0),
          BSIM3v32pk3(0),
          BSIM3v32pk3b(0),
          BSIM3v32pw0(0),
          BSIM3v32pnlx(0),
          BSIM3v32pdvt0(0),
          BSIM3v32pdvt1(0),
          BSIM3v32pdvt2(0),
          BSIM3v32pdvt0w(0),
          BSIM3v32pdvt1w(0),
          BSIM3v32pdvt2w(0),
          BSIM3v32pdrout(0),
          BSIM3v32pdsub(0),
          BSIM3v32pvth0(0),
          BSIM3v32pua(0),
          BSIM3v32pua1(0),
          BSIM3v32pub(0),
          BSIM3v32pub1(0),
          BSIM3v32puc(0),
          BSIM3v32puc1(0),
          BSIM3v32pu0(0),
          BSIM3v32pute(0),
          BSIM3v32pvoff(0),
          BSIM3v32pdelta(0),
          BSIM3v32prdsw(0),
          BSIM3v32pprwg(0),
          BSIM3v32pprwb(0),
          BSIM3v32pprt(0),
          BSIM3v32peta0(0),
          BSIM3v32petab(0),
          BSIM3v32ppclm(0),
          BSIM3v32ppdibl1(0),
          BSIM3v32ppdibl2(0),
          BSIM3v32ppdiblb(0),
          BSIM3v32ppscbe1(0),
          BSIM3v32ppscbe2(0),
          BSIM3v32ppvag(0),
          BSIM3v32pwr(0),
          BSIM3v32pdwg(0),
          BSIM3v32pdwb(0),
          BSIM3v32pb0(0),
          BSIM3v32pb1(0),
          BSIM3v32palpha0(0),
          BSIM3v32palpha1(0),
          BSIM3v32pbeta0(0),
          BSIM3v32pvfb(0),

          /* CV model */
          BSIM3v32pelm(0),
          BSIM3v32pcgsl(0),
          BSIM3v32pcgdl(0),
          BSIM3v32pckappa(0),
          BSIM3v32pcf(0),
          BSIM3v32pclc(0),
          BSIM3v32pcle(0),
          BSIM3v32pvfbcv(0),
          BSIM3v32pnoff(0),
          BSIM3v32pvoffcv(0),
          BSIM3v32pacde(0),
          BSIM3v32pmoin(0),

          BSIM3v32tnom(0),
          BSIM3v32cgso(0),
          BSIM3v32cgdo(0),
          BSIM3v32cgbo(0),
          BSIM3v32xpart(0),
          BSIM3v32cFringOut(0),
          BSIM3v32cFringMax(0),

          BSIM3v32sheetResistance(0),
          BSIM3v32jctSatCurDensity(0),
          BSIM3v32jctSidewallSatCurDensity(0),
          BSIM3v32bulkJctPotential(0),
          BSIM3v32bulkJctBotGradingCoeff(0),
          BSIM3v32bulkJctSideGradingCoeff(0),
          BSIM3v32bulkJctGateSideGradingCoeff(0),
          BSIM3v32sidewallJctPotential(0),
          BSIM3v32GatesidewallJctPotential(0),
          BSIM3v32unitAreaJctCap(0),
          BSIM3v32unitLengthSidewallJctCap(0),
          BSIM3v32unitLengthGateSidewallJctCap(0),
          BSIM3v32jctEmissionCoeff(0),
          BSIM3v32jctTempExponent(0),

          BSIM3v32Lint(0),
          BSIM3v32Ll(0),
          BSIM3v32Llc(0),
          BSIM3v32Lln(0),
          BSIM3v32Lw(0),
          BSIM3v32Lwc(0),
          BSIM3v32Lwn(0),
          BSIM3v32Lwl(0),
          BSIM3v32Lwlc(0),
          BSIM3v32Lmin(0),
          BSIM3v32Lmax(0),

          BSIM3v32Wint(0),
          BSIM3v32Wl(0),
          BSIM3v32Wlc(0),
          BSIM3v32Wln(0),
          BSIM3v32Ww(0),
          BSIM3v32Wwc(0),
          BSIM3v32Wwn(0),
          BSIM3v32Wwl(0),
          BSIM3v32Wwlc(0),
          BSIM3v32Wmin(0),
          BSIM3v32Wmax(0),

          /* Pre-calculated constants */
          /* MCJ: move to size-dependent param. */
          BSIM3v32vtm(0),
          BSIM3v32cox(0),
          BSIM3v32cof1(0),
          BSIM3v32cof2(0),
          BSIM3v32cof3(0),
          BSIM3v32cof4(0),
          BSIM3v32vcrit(0),
          BSIM3v32factor1(0),
          BSIM3v32PhiB(0),
          BSIM3v32PhiBSW(0),
          BSIM3v32PhiBSWG(0),
          BSIM3v32jctTempSatCurDensity(0),
          BSIM3v32jctSidewallTempSatCurDensity(0),
          BSIM3v32unitAreaTempJctCap(0),
          BSIM3v32unitLengthSidewallTempJctCap(0),
          BSIM3v32unitLengthGateSidewallTempJctCap(0),

          BSIM3v32oxideTrapDensityA(0),
          BSIM3v32oxideTrapDensityB(0),
          BSIM3v32oxideTrapDensityC(0),
          BSIM3v32em(0),
          BSIM3v32ef(0),
          BSIM3v32af(0),
          BSIM3v32kf(0),

          BSIM3v32vgsMax(0),
          BSIM3v32vgdMax(0),
          BSIM3v32vgbMax(0),
          BSIM3v32vdsMax(0),
          BSIM3v32vbsMax(0),
          BSIM3v32vbdMax(0),
          BSIM3v32vgsrMax(0),
          BSIM3v32vgdrMax(0),
          BSIM3v32vgbrMax(0),
          BSIM3v32vbsrMax(0),
          BSIM3v32vbdrMax(0),

          pSizeDependParamKnot(nullptr),

#ifdef USE_OMP
          BSIM3v32InstCount(0),
          BSIM3v32InstanceArray(nullptr),
#endif

          /* Flags */
          BSIM3v32mobModGiven(0),
          BSIM3v32binUnitGiven(0),
          BSIM3v32capModGiven(0),
          BSIM3v32acmModGiven(0),
          BSIM3v32calcacmGiven(0),
          BSIM3v32paramChkGiven(0),
          BSIM3v32noiModGiven(0),
          BSIM3v32nqsModGiven(0),
          BSIM3v32typeGiven(0),
          BSIM3v32toxGiven(0),
          BSIM3v32versionGiven(0),
          BSIM3v32toxmGiven(0),
          BSIM3v32cdscGiven(0),
          BSIM3v32cdscbGiven(0),
          BSIM3v32cdscdGiven(0),
          BSIM3v32citGiven(0),
          BSIM3v32nfactorGiven(0),
          BSIM3v32xjGiven(0),
          BSIM3v32vsatGiven(0),
          BSIM3v32atGiven(0),
          BSIM3v32a0Given(0),
          BSIM3v32agsGiven(0),
          BSIM3v32a1Given(0),
          BSIM3v32a2Given(0),
          BSIM3v32ketaGiven(0),
          BSIM3v32nsubGiven(0),
          BSIM3v32npeakGiven(0),
          BSIM3v32ngateGiven(0),
          BSIM3v32gamma1Given(0),
          BSIM3v32gamma2Given(0),
          BSIM3v32vbxGiven(0),
          BSIM3v32vbmGiven(0),
          BSIM3v32xtGiven(0),
          BSIM3v32k1Given(0),
          BSIM3v32kt1Given(0),
          BSIM3v32kt1lGiven(0),
          BSIM3v32kt2Given(0),
          BSIM3v32k2Given(0),
          BSIM3v32k3Given(0),
          BSIM3v32k3bGiven(0),
          BSIM3v32w0Given(0),
          BSIM3v32nlxGiven(0),
          BSIM3v32dvt0Given(0),
          BSIM3v32dvt1Given(0),
          BSIM3v32dvt2Given(0),
          BSIM3v32dvt0wGiven(0),
          BSIM3v32dvt1wGiven(0),
          BSIM3v32dvt2wGiven(0),
          BSIM3v32droutGiven(0),
          BSIM3v32dsubGiven(0),
          BSIM3v32vth0Given(0),
          BSIM3v32uaGiven(0),
          BSIM3v32ua1Given(0),
          BSIM3v32ubGiven(0),
          BSIM3v32ub1Given(0),
          BSIM3v32ucGiven(0),
          BSIM3v32uc1Given(0),
          BSIM3v32u0Given(0),
          BSIM3v32uteGiven(0),
          BSIM3v32voffGiven(0),
          BSIM3v32rdswGiven(0),
          BSIM3v32prwgGiven(0),
          BSIM3v32prwbGiven(0),
          BSIM3v32prtGiven(0),
          BSIM3v32eta0Given(0),
          BSIM3v32etabGiven(0),
          BSIM3v32pclmGiven(0),
          BSIM3v32pdibl1Given(0),
          BSIM3v32pdibl2Given(0),
          BSIM3v32pdiblbGiven(0),
          BSIM3v32pscbe1Given(0),
          BSIM3v32pscbe2Given(0),
          BSIM3v32pvagGiven(0),
          BSIM3v32deltaGiven(0),
          BSIM3v32wrGiven(0),
          BSIM3v32dwgGiven(0),
          BSIM3v32dwbGiven(0),
          BSIM3v32b0Given(0),
          BSIM3v32b1Given(0),
          BSIM3v32alpha0Given(0),
          BSIM3v32alpha1Given(0),
          BSIM3v32beta0Given(0),
          BSIM3v32ijthGiven(0),
          BSIM3v32vfbGiven(0),

          /* CV model */
          BSIM3v32elmGiven(0),
          BSIM3v32cgslGiven(0),
          BSIM3v32cgdlGiven(0),
          BSIM3v32ckappaGiven(0),
          BSIM3v32cfGiven(0),
          BSIM3v32vfbcvGiven(0),
          BSIM3v32clcGiven(0),
          BSIM3v32cleGiven(0),
          BSIM3v32dwcGiven(0),
          BSIM3v32dlcGiven(0),
          BSIM3v32noffGiven(0),
          BSIM3v32voffcvGiven(0),
          BSIM3v32acdeGiven(0),
          BSIM3v32moinGiven(0),
          BSIM3v32tcjGiven(0),
          BSIM3v32tcjswGiven(0),
          BSIM3v32tcjswgGiven(0),
          BSIM3v32tpbGiven(0),
          BSIM3v32tpbswGiven(0),
          BSIM3v32tpbswgGiven(0),

          /* ACM model */
          BSIM3v32xlGiven(0),
          BSIM3v32xwGiven(0),
          BSIM3v32hdifGiven(0),
          BSIM3v32ldifGiven(0),
          BSIM3v32ldGiven(0),
          BSIM3v32rdGiven(0),
          BSIM3v32rsGiven(0),
          BSIM3v32rdcGiven(0),
          BSIM3v32rscGiven(0),
          BSIM3v32wmltGiven(0),

          BSIM3v32lmltGiven(0),

          /* Length dependence */
          BSIM3v32lcdscGiven(0),
          BSIM3v32lcdscbGiven(0),
          BSIM3v32lcdscdGiven(0),
          BSIM3v32lcitGiven(0),
          BSIM3v32lnfactorGiven(0),
          BSIM3v32lxjGiven(0),
          BSIM3v32lvsatGiven(0),
          BSIM3v32latGiven(0),
          BSIM3v32la0Given(0),
          BSIM3v32lagsGiven(0),
          BSIM3v32la1Given(0),
          BSIM3v32la2Given(0),
          BSIM3v32lketaGiven(0),
          BSIM3v32lnsubGiven(0),
          BSIM3v32lnpeakGiven(0),
          BSIM3v32lngateGiven(0),
          BSIM3v32lgamma1Given(0),
          BSIM3v32lgamma2Given(0),
          BSIM3v32lvbxGiven(0),
          BSIM3v32lvbmGiven(0),
          BSIM3v32lxtGiven(0),
          BSIM3v32lk1Given(0),
          BSIM3v32lkt1Given(0),
          BSIM3v32lkt1lGiven(0),
          BSIM3v32lkt2Given(0),
          BSIM3v32lk2Given(0),
          BSIM3v32lk3Given(0),
          BSIM3v32lk3bGiven(0),
          BSIM3v32lw0Given(0),
          BSIM3v32lnlxGiven(0),
          BSIM3v32ldvt0Given(0),
          BSIM3v32ldvt1Given(0),
          BSIM3v32ldvt2Given(0),
          BSIM3v32ldvt0wGiven(0),
          BSIM3v32ldvt1wGiven(0),
          BSIM3v32ldvt2wGiven(0),
          BSIM3v32ldroutGiven(0),
          BSIM3v32ldsubGiven(0),
          BSIM3v32lvth0Given(0),
          BSIM3v32luaGiven(0),
          BSIM3v32lua1Given(0),
          BSIM3v32lubGiven(0),
          BSIM3v32lub1Given(0),
          BSIM3v32lucGiven(0),
          BSIM3v32luc1Given(0),
          BSIM3v32lu0Given(0),
          BSIM3v32luteGiven(0),
          BSIM3v32lvoffGiven(0),
          BSIM3v32lrdswGiven(0),
          BSIM3v32lprwgGiven(0),
          BSIM3v32lprwbGiven(0),
          BSIM3v32lprtGiven(0),
          BSIM3v32leta0Given(0),
          BSIM3v32letabGiven(0),
          BSIM3v32lpclmGiven(0),
          BSIM3v32lpdibl1Given(0),
          BSIM3v32lpdibl2Given(0),
          BSIM3v32lpdiblbGiven(0),
          BSIM3v32lpscbe1Given(0),
          BSIM3v32lpscbe2Given(0),
          BSIM3v32lpvagGiven(0),
          BSIM3v32ldeltaGiven(0),
          BSIM3v32lwrGiven(0),
          BSIM3v32ldwgGiven(0),
          BSIM3v32ldwbGiven(0),
          BSIM3v32lb0Given(0),
          BSIM3v32lb1Given(0),
          BSIM3v32lalpha0Given(0),
          BSIM3v32lalpha1Given(0),
          BSIM3v32lbeta0Given(0),
          BSIM3v32lvfbGiven(0),

          /* CV model */
          BSIM3v32lelmGiven(0),
          BSIM3v32lcgslGiven(0),
          BSIM3v32lcgdlGiven(0),
          BSIM3v32lckappaGiven(0),
          BSIM3v32lcfGiven(0),
          BSIM3v32lclcGiven(0),
          BSIM3v32lcleGiven(0),
          BSIM3v32lvfbcvGiven(0),
          BSIM3v32lnoffGiven(0),
          BSIM3v32lvoffcvGiven(0),
          BSIM3v32lacdeGiven(0),
          BSIM3v32lmoinGiven(0),

          /* Width dependence */
          BSIM3v32wcdscGiven(0),
          BSIM3v32wcdscbGiven(0),
          BSIM3v32wcdscdGiven(0),
          BSIM3v32wcitGiven(0),
          BSIM3v32wnfactorGiven(0),
          BSIM3v32wxjGiven(0),
          BSIM3v32wvsatGiven(0),
          BSIM3v32watGiven(0),
          BSIM3v32wa0Given(0),
          BSIM3v32wagsGiven(0),
          BSIM3v32wa1Given(0),
          BSIM3v32wa2Given(0),
          BSIM3v32wketaGiven(0),
          BSIM3v32wnsubGiven(0),
          BSIM3v32wnpeakGiven(0),
          BSIM3v32wngateGiven(0),
          BSIM3v32wgamma1Given(0),
          BSIM3v32wgamma2Given(0),
          BSIM3v32wvbxGiven(0),
          BSIM3v32wvbmGiven(0),
          BSIM3v32wxtGiven(0),
          BSIM3v32wk1Given(0),
          BSIM3v32wkt1Given(0),
          BSIM3v32wkt1lGiven(0),
          BSIM3v32wkt2Given(0),
          BSIM3v32wk2Given(0),
          BSIM3v32wk3Given(0),
          BSIM3v32wk3bGiven(0),
          BSIM3v32ww0Given(0),
          BSIM3v32wnlxGiven(0),
          BSIM3v32wdvt0Given(0),
          BSIM3v32wdvt1Given(0),
          BSIM3v32wdvt2Given(0),
          BSIM3v32wdvt0wGiven(0),
          BSIM3v32wdvt1wGiven(0),
          BSIM3v32wdvt2wGiven(0),
          BSIM3v32wdroutGiven(0),
          BSIM3v32wdsubGiven(0),
          BSIM3v32wvth0Given(0),
          BSIM3v32wuaGiven(0),
          BSIM3v32wua1Given(0),
          BSIM3v32wubGiven(0),
          BSIM3v32wub1Given(0),
          BSIM3v32wucGiven(0),
          BSIM3v32wuc1Given(0),
          BSIM3v32wu0Given(0),
          BSIM3v32wuteGiven(0),
          BSIM3v32wvoffGiven(0),
          BSIM3v32wrdswGiven(0),
          BSIM3v32wprwgGiven(0),
          BSIM3v32wprwbGiven(0),
          BSIM3v32wprtGiven(0),
          BSIM3v32weta0Given(0),
          BSIM3v32wetabGiven(0),
          BSIM3v32wpclmGiven(0),
          BSIM3v32wpdibl1Given(0),
          BSIM3v32wpdibl2Given(0),
          BSIM3v32wpdiblbGiven(0),
          BSIM3v32wpscbe1Given(0),
          BSIM3v32wpscbe2Given(0),
          BSIM3v32wpvagGiven(0),
          BSIM3v32wdeltaGiven(0),
          BSIM3v32wwrGiven(0),
          BSIM3v32wdwgGiven(0),
          BSIM3v32wdwbGiven(0),
          BSIM3v32wb0Given(0),
          BSIM3v32wb1Given(0),
          BSIM3v32walpha0Given(0),
          BSIM3v32walpha1Given(0),
          BSIM3v32wbeta0Given(0),
          BSIM3v32wvfbGiven(0),

          /* CV model */
          BSIM3v32welmGiven(0),
          BSIM3v32wcgslGiven(0),
          BSIM3v32wcgdlGiven(0),
          BSIM3v32wckappaGiven(0),
          BSIM3v32wcfGiven(0),
          BSIM3v32wclcGiven(0),
          BSIM3v32wcleGiven(0),
          BSIM3v32wvfbcvGiven(0),
          BSIM3v32wnoffGiven(0),
          BSIM3v32wvoffcvGiven(0),
          BSIM3v32wacdeGiven(0),
          BSIM3v32wmoinGiven(0),

          /* Cross-term dependence */
          BSIM3v32pcdscGiven(0),
          BSIM3v32pcdscbGiven(0),
          BSIM3v32pcdscdGiven(0),
          BSIM3v32pcitGiven(0),
          BSIM3v32pnfactorGiven(0),
          BSIM3v32pxjGiven(0),
          BSIM3v32pvsatGiven(0),
          BSIM3v32patGiven(0),
          BSIM3v32pa0Given(0),
          BSIM3v32pagsGiven(0),
          BSIM3v32pa1Given(0),
          BSIM3v32pa2Given(0),
          BSIM3v32pketaGiven(0),
          BSIM3v32pnsubGiven(0),
          BSIM3v32pnpeakGiven(0),
          BSIM3v32pngateGiven(0),
          BSIM3v32pgamma1Given(0),
          BSIM3v32pgamma2Given(0),
          BSIM3v32pvbxGiven(0),
          BSIM3v32pvbmGiven(0),
          BSIM3v32pxtGiven(0),
          BSIM3v32pk1Given(0),
          BSIM3v32pkt1Given(0),
          BSIM3v32pkt1lGiven(0),
          BSIM3v32pkt2Given(0),
          BSIM3v32pk2Given(0),
          BSIM3v32pk3Given(0),
          BSIM3v32pk3bGiven(0),
          BSIM3v32pw0Given(0),
          BSIM3v32pnlxGiven(0),
          BSIM3v32pdvt0Given(0),
          BSIM3v32pdvt1Given(0),
          BSIM3v32pdvt2Given(0),
          BSIM3v32pdvt0wGiven(0),
          BSIM3v32pdvt1wGiven(0),
          BSIM3v32pdvt2wGiven(0),
          BSIM3v32pdroutGiven(0),
          BSIM3v32pdsubGiven(0),
          BSIM3v32pvth0Given(0),
          BSIM3v32puaGiven(0),
          BSIM3v32pua1Given(0),
          BSIM3v32pubGiven(0),
          BSIM3v32pub1Given(0),
          BSIM3v32pucGiven(0),
          BSIM3v32puc1Given(0),
          BSIM3v32pu0Given(0),
          BSIM3v32puteGiven(0),
          BSIM3v32pvoffGiven(0),
          BSIM3v32prdswGiven(0),
          BSIM3v32pprwgGiven(0),
          BSIM3v32pprwbGiven(0),
          BSIM3v32pprtGiven(0),
          BSIM3v32peta0Given(0),
          BSIM3v32petabGiven(0),
          BSIM3v32ppclmGiven(0),
          BSIM3v32ppdibl1Given(0),
          BSIM3v32ppdibl2Given(0),
          BSIM3v32ppdiblbGiven(0),
          BSIM3v32ppscbe1Given(0),
          BSIM3v32ppscbe2Given(0),
          BSIM3v32ppvagGiven(0),
          BSIM3v32pdeltaGiven(0),
          BSIM3v32pwrGiven(0),
          BSIM3v32pdwgGiven(0),
          BSIM3v32pdwbGiven(0),
          BSIM3v32pb0Given(0),
          BSIM3v32pb1Given(0),
          BSIM3v32palpha0Given(0),
          BSIM3v32palpha1Given(0),
          BSIM3v32pbeta0Given(0),
          BSIM3v32pvfbGiven(0),

          /* CV model */
          BSIM3v32pelmGiven(0),
          BSIM3v32pcgslGiven(0),
          BSIM3v32pcgdlGiven(0),
          BSIM3v32pckappaGiven(0),
          BSIM3v32pcfGiven(0),
          BSIM3v32pclcGiven(0),
          BSIM3v32pcleGiven(0),
          BSIM3v32pvfbcvGiven(0),
          BSIM3v32pnoffGiven(0),
          BSIM3v32pvoffcvGiven(0),
          BSIM3v32pacdeGiven(0),
          BSIM3v32pmoinGiven(0),

          BSIM3v32useFringeGiven(0),

          BSIM3v32tnomGiven(0),
          BSIM3v32cgsoGiven(0),
          BSIM3v32cgdoGiven(0),
          BSIM3v32cgboGiven(0),
          BSIM3v32xpartGiven(0),
          BSIM3v32sheetResistanceGiven(0),
          BSIM3v32jctSatCurDensityGiven(0),
          BSIM3v32jctSidewallSatCurDensityGiven(0),
          BSIM3v32bulkJctPotentialGiven(0),
          BSIM3v32bulkJctBotGradingCoeffGiven(0),
          BSIM3v32sidewallJctPotentialGiven(0),
          BSIM3v32GatesidewallJctPotentialGiven(0),
          BSIM3v32bulkJctSideGradingCoeffGiven(0),
          BSIM3v32unitAreaJctCapGiven(0),
          BSIM3v32unitLengthSidewallJctCapGiven(0),
          BSIM3v32bulkJctGateSideGradingCoeffGiven(0),
          BSIM3v32unitLengthGateSidewallJctCapGiven(0),
          BSIM3v32jctEmissionCoeffGiven(0),
          BSIM3v32jctTempExponentGiven(0),

          BSIM3v32oxideTrapDensityAGiven(0),
          BSIM3v32oxideTrapDensityBGiven(0),
          BSIM3v32oxideTrapDensityCGiven(0),
          BSIM3v32emGiven(0),
          BSIM3v32efGiven(0),
          BSIM3v32afGiven(0),
          BSIM3v32kfGiven(0),

          BSIM3v32LintGiven(0),
          BSIM3v32LlGiven(0),
          BSIM3v32LlcGiven(0),
          BSIM3v32LlnGiven(0),
          BSIM3v32LwGiven(0),
          BSIM3v32LwcGiven(0),
          BSIM3v32LwnGiven(0),
          BSIM3v32LwlGiven(0),
          BSIM3v32LwlcGiven(0),
          BSIM3v32LminGiven(0),
          BSIM3v32LmaxGiven(0),

          BSIM3v32WintGiven(0),
          BSIM3v32WlGiven(0),
          BSIM3v32WlcGiven(0),
          BSIM3v32WlnGiven(0),
          BSIM3v32WwGiven(0),
          BSIM3v32WwcGiven(0),
          BSIM3v32WwnGiven(0),
          BSIM3v32WwlGiven(0),
          BSIM3v32WwlcGiven(0),
          BSIM3v32WminGiven(0),
          BSIM3v32WmaxGiven(0),

          BSIM3v32vgsMaxGiven(0),
          BSIM3v32vgdMaxGiven(0),
          BSIM3v32vgbMaxGiven(0),
          BSIM3v32vdsMaxGiven(0),
          BSIM3v32vbsMaxGiven(0),
          BSIM3v32vbdMaxGiven(0),
          BSIM3v32vgsrMaxGiven(0),
          BSIM3v32vgdrMaxGiven(0),
          BSIM3v32vgbrMaxGiven(0),
          BSIM3v32vbsrMaxGiven(0),
          BSIM3v32vbdrMaxGiven(0)
    {
    }

    BSIM3v32model::~BSIM3v32model()
    {
        struct bsim3v32SizeDependParam *p = this->pSizeDependParamKnot;
        while (p)
        {
            struct bsim3v32SizeDependParam *next_p = p->pNext;
            delete p;
            p = next_p;
        }
    }

    const char *BSIM3v32model::name() const
    {
        return m_modelName.c_str();
    }

    int BSIM3v32model::setProperty(int id, const Variant &value)
    {
        int rc;
        switch (id)
        {
        case BSIM3v32_MOD_MOBMOD:
            this->BSIM3v32mobMod = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32mobModGiven = true;
            break;
        case BSIM3v32_MOD_BINUNIT:
            this->BSIM3v32binUnit = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32binUnitGiven = true;
            break;
        case BSIM3v32_MOD_PARAMCHK:
            this->BSIM3v32paramChk = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32paramChkGiven = true;
            break;
        case BSIM3v32_MOD_CAPMOD:
            this->BSIM3v32capMod = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32capModGiven = true;
            break;
        case BSIM3v32_MOD_ACMMOD:
            this->BSIM3v32acmMod = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32acmModGiven = true;
            break;
        case BSIM3v32_MOD_CALCACM:
            this->BSIM3v32calcacm = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32calcacmGiven = true;
            break;
        case BSIM3v32_MOD_NOIMOD:
            this->BSIM3v32noiMod = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32noiModGiven = true;
            break;
        case BSIM3v32_MOD_NQSMOD:
            this->BSIM3v32nqsMod = value.getInt32Cast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32nqsModGiven = true;
            break;
        case BSIM3v32_MOD_VERSION:
        {
            Variant c(value);
            this->BSIM3v32version = c.getStringCast(&rc);
            std::cout<<"ver="<<this->BSIM3v32version<<"\n";
            UPDATE_RC(rc);
            this->BSIM3v32versionGiven = true;
            break;
        }
        case BSIM3v32_MOD_TOX:
            this->BSIM3v32tox = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32toxGiven = true;
            break;
        case BSIM3v32_MOD_TOXM:
            this->BSIM3v32toxm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32toxmGiven = true;
            break;

        case BSIM3v32_MOD_CDSC:
            this->BSIM3v32cdsc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cdscGiven = true;
            break;
        case BSIM3v32_MOD_CDSCB:
            this->BSIM3v32cdscb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cdscbGiven = true;
            break;

        case BSIM3v32_MOD_CDSCD:
            this->BSIM3v32cdscd = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cdscdGiven = true;
            break;

        case BSIM3v32_MOD_CIT:
            this->BSIM3v32cit = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32citGiven = true;
            break;
        case BSIM3v32_MOD_NFACTOR:
            this->BSIM3v32nfactor = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32nfactorGiven = true;
            break;
        case BSIM3v32_MOD_XJ:
            this->BSIM3v32xj = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32xjGiven = true;
            break;
        case BSIM3v32_MOD_VSAT:
            this->BSIM3v32vsat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vsatGiven = true;
            break;
        case BSIM3v32_MOD_A0:
            this->BSIM3v32a0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32a0Given = true;
            break;

        case BSIM3v32_MOD_AGS:
            this->BSIM3v32ags = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32agsGiven = true;
            break;

        case BSIM3v32_MOD_A1:
            this->BSIM3v32a1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32a1Given = true;
            break;
        case BSIM3v32_MOD_A2:
            this->BSIM3v32a2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32a2Given = true;
            break;
        case BSIM3v32_MOD_AT:
            this->BSIM3v32at = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32atGiven = true;
            break;
        case BSIM3v32_MOD_KETA:
            this->BSIM3v32keta = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ketaGiven = true;
            break;
        case BSIM3v32_MOD_NSUB:
            this->BSIM3v32nsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32nsubGiven = true;
            break;
        case BSIM3v32_MOD_NPEAK:
            this->BSIM3v32npeak = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32npeakGiven = true;
            if (this->BSIM3v32npeak > 1.0e20)
                this->BSIM3v32npeak *= 1.0e-6;
            break;
        case BSIM3v32_MOD_NGATE:
            this->BSIM3v32ngate = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ngateGiven = true;
            if (this->BSIM3v32ngate > 1.000001e24)
                this->BSIM3v32ngate *= 1.0e-6;
            break;
        case BSIM3v32_MOD_GAMMA1:
            this->BSIM3v32gamma1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32gamma1Given = true;
            break;
        case BSIM3v32_MOD_GAMMA2:
            this->BSIM3v32gamma2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32gamma2Given = true;
            break;
        case BSIM3v32_MOD_VBX:
            this->BSIM3v32vbx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbxGiven = true;
            break;
        case BSIM3v32_MOD_VBM:
            this->BSIM3v32vbm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbmGiven = true;
            break;
        case BSIM3v32_MOD_XT:
            this->BSIM3v32xt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32xtGiven = true;
            break;
        case BSIM3v32_MOD_K1:
            this->BSIM3v32k1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32k1Given = true;
            break;
        case BSIM3v32_MOD_KT1:
            this->BSIM3v32kt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32kt1Given = true;
            break;
        case BSIM3v32_MOD_KT1L:
            this->BSIM3v32kt1l = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32kt1lGiven = true;
            break;
        case BSIM3v32_MOD_KT2:
            this->BSIM3v32kt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32kt2Given = true;
            break;
        case BSIM3v32_MOD_K2:
            this->BSIM3v32k2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32k2Given = true;
            break;
        case BSIM3v32_MOD_K3:
            this->BSIM3v32k3 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32k3Given = true;
            break;
        case BSIM3v32_MOD_K3B:
            this->BSIM3v32k3b = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32k3bGiven = true;
            break;
        case BSIM3v32_MOD_NLX:
            this->BSIM3v32nlx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32nlxGiven = true;
            break;
        case BSIM3v32_MOD_W0:
            this->BSIM3v32w0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32w0Given = true;
            break;
        case BSIM3v32_MOD_DVT0:
            this->BSIM3v32dvt0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt0Given = true;
            break;
        case BSIM3v32_MOD_DVT1:
            this->BSIM3v32dvt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt1Given = true;
            break;
        case BSIM3v32_MOD_DVT2:
            this->BSIM3v32dvt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt2Given = true;
            break;
        case BSIM3v32_MOD_DVT0W:
            this->BSIM3v32dvt0w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt0wGiven = true;
            break;
        case BSIM3v32_MOD_DVT1W:
            this->BSIM3v32dvt1w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt1wGiven = true;
            break;
        case BSIM3v32_MOD_DVT2W:
            this->BSIM3v32dvt2w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dvt2wGiven = true;
            break;
        case BSIM3v32_MOD_DROUT:
            this->BSIM3v32drout = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32droutGiven = true;
            break;
        case BSIM3v32_MOD_DSUB:
            this->BSIM3v32dsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dsubGiven = true;
            break;
        case BSIM3v32_MOD_VTH0:
            this->BSIM3v32vth0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vth0Given = true;
            break;
        case BSIM3v32_MOD_UA:
            this->BSIM3v32ua = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32uaGiven = true;
            break;
        case BSIM3v32_MOD_UA1:
            this->BSIM3v32ua1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ua1Given = true;
            break;
        case BSIM3v32_MOD_UB:
            this->BSIM3v32ub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ubGiven = true;
            break;
        case BSIM3v32_MOD_UB1:
            this->BSIM3v32ub1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ub1Given = true;
            break;
        case BSIM3v32_MOD_UC:
            this->BSIM3v32uc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ucGiven = true;
            break;
        case BSIM3v32_MOD_UC1:
            this->BSIM3v32uc1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32uc1Given = true;
            break;
        case BSIM3v32_MOD_U0:
            this->BSIM3v32u0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32u0Given = true;
            break;
        case BSIM3v32_MOD_UTE:
            this->BSIM3v32ute = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32uteGiven = true;
            break;
        case BSIM3v32_MOD_VOFF:
            this->BSIM3v32voff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32voffGiven = true;
            break;
        case BSIM3v32_MOD_DELTA:
            this->BSIM3v32delta = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32deltaGiven = true;
            break;
        case BSIM3v32_MOD_RDSW:
            this->BSIM3v32rdsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32rdswGiven = true;
            break;
        case BSIM3v32_MOD_PRWG:
            this->BSIM3v32prwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32prwgGiven = true;
            break;
        case BSIM3v32_MOD_PRWB:
            this->BSIM3v32prwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32prwbGiven = true;
            break;
        case BSIM3v32_MOD_PRT:
            this->BSIM3v32prt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32prtGiven = true;
            break;
        case BSIM3v32_MOD_ETA0:
            this->BSIM3v32eta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32eta0Given = true;
            break;
        case BSIM3v32_MOD_ETAB:
            this->BSIM3v32etab = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32etabGiven = true;
            break;
        case BSIM3v32_MOD_PCLM:
            this->BSIM3v32pclm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pclmGiven = true;
            break;
        case BSIM3v32_MOD_PDIBL1:
            this->BSIM3v32pdibl1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdibl1Given = true;
            break;
        case BSIM3v32_MOD_PDIBL2:
            this->BSIM3v32pdibl2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdibl2Given = true;
            break;
        case BSIM3v32_MOD_PDIBLB:
            this->BSIM3v32pdiblb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdiblbGiven = true;
            break;
        case BSIM3v32_MOD_PSCBE1:
            this->BSIM3v32pscbe1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pscbe1Given = true;
            break;
        case BSIM3v32_MOD_PSCBE2:
            this->BSIM3v32pscbe2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pscbe2Given = true;
            break;
        case BSIM3v32_MOD_PVAG:
            this->BSIM3v32pvag = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvagGiven = true;
            break;
        case BSIM3v32_MOD_WR:
            this->BSIM3v32wr = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wrGiven = true;
            break;
        case BSIM3v32_MOD_DWG:
            this->BSIM3v32dwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dwgGiven = true;
            break;
        case BSIM3v32_MOD_DWB:
            this->BSIM3v32dwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dwbGiven = true;
            break;
        case BSIM3v32_MOD_B0:
            this->BSIM3v32b0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32b0Given = true;
            break;
        case BSIM3v32_MOD_B1:
            this->BSIM3v32b1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32b1Given = true;
            break;
        case BSIM3v32_MOD_ALPHA0:
            this->BSIM3v32alpha0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32alpha0Given = true;
            break;
        case BSIM3v32_MOD_ALPHA1:
            this->BSIM3v32alpha1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32alpha1Given = true;
            break;
        case BSIM3v32_MOD_BETA0:
            this->BSIM3v32beta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32beta0Given = true;
            break;
        case BSIM3v32_MOD_IJTH:
            this->BSIM3v32ijth = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ijthGiven = true;
            break;
        case BSIM3v32_MOD_VFB:
            this->BSIM3v32vfb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vfbGiven = true;
            break;

        case BSIM3v32_MOD_ELM:
            this->BSIM3v32elm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32elmGiven = true;
            break;
        case BSIM3v32_MOD_CGSL:
            this->BSIM3v32cgsl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cgslGiven = true;
            break;
        case BSIM3v32_MOD_CGDL:
            this->BSIM3v32cgdl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cgdlGiven = true;
            break;
        case BSIM3v32_MOD_CKAPPA:
            this->BSIM3v32ckappa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ckappaGiven = true;
            break;
        case BSIM3v32_MOD_CF:
            this->BSIM3v32cf = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cfGiven = true;
            break;
        case BSIM3v32_MOD_CLC:
            this->BSIM3v32clc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32clcGiven = true;
            break;
        case BSIM3v32_MOD_CLE:
            this->BSIM3v32cle = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cleGiven = true;
            break;
        case BSIM3v32_MOD_DWC:
            this->BSIM3v32dwc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dwcGiven = true;
            break;
        case BSIM3v32_MOD_DLC:
            this->BSIM3v32dlc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32dlcGiven = true;
            break;
        case BSIM3v32_MOD_VFBCV:
            this->BSIM3v32vfbcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vfbcvGiven = true;
            break;
        case BSIM3v32_MOD_ACDE:
            this->BSIM3v32acde = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32acdeGiven = true;
            break;
        case BSIM3v32_MOD_MOIN:
            this->BSIM3v32moin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32moinGiven = true;
            break;
        case BSIM3v32_MOD_NOFF:
            this->BSIM3v32noff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32noffGiven = true;
            break;
        case BSIM3v32_MOD_VOFFCV:
            this->BSIM3v32voffcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32voffcvGiven = true;
            break;
        case BSIM3v32_MOD_TCJ:
            this->BSIM3v32tcj = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tcjGiven = true;
            break;
        case BSIM3v32_MOD_TPB:
            this->BSIM3v32tpb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tpbGiven = true;
            break;
        case BSIM3v32_MOD_TCJSW:
            this->BSIM3v32tcjsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tcjswGiven = true;
            break;
        case BSIM3v32_MOD_TPBSW:
            this->BSIM3v32tpbsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tpbswGiven = true;
            break;
        case BSIM3v32_MOD_TCJSWG:
            this->BSIM3v32tcjswg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tcjswgGiven = true;
            break;
        case BSIM3v32_MOD_TPBSWG:
            this->BSIM3v32tpbswg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32tpbswgGiven = true;
            break;

            /* acm model */
        case BSIM3v32_MOD_HDIF:
            this->BSIM3v32hdif = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32hdifGiven = true;
            break;
        case BSIM3v32_MOD_LDIF:
            this->BSIM3v32ldif = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldifGiven = true;
            break;
        case BSIM3v32_MOD_LD:
            this->BSIM3v32ld = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldGiven = true;
            break;
        case BSIM3v32_MOD_RD:
            this->BSIM3v32rd = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32rdGiven = true;
            break;
        case BSIM3v32_MOD_RS:
            this->BSIM3v32rs = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32rsGiven = true;
            break;
        case BSIM3v32_MOD_RDC:
            this->BSIM3v32rdc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32rdcGiven = true;
            break;
        case BSIM3v32_MOD_RSC:
            this->BSIM3v32rsc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32rscGiven = true;
            break;
        case BSIM3v32_MOD_WMLT:
            this->BSIM3v32wmlt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wmltGiven = true;
            break;

            /* Length shrink */
        case BSIM3v32_MOD_LMLT:
            this->BSIM3v32lmlt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lmltGiven = true;
            break;

        /* Length dependence */
        case BSIM3v32_MOD_LCDSC:
            this->BSIM3v32lcdsc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcdscGiven = true;
            break;

        case BSIM3v32_MOD_LCDSCB:
            this->BSIM3v32lcdscb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcdscbGiven = true;
            break;
        case BSIM3v32_MOD_LCDSCD:
            this->BSIM3v32lcdscd = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcdscdGiven = true;
            break;
        case BSIM3v32_MOD_LCIT:
            this->BSIM3v32lcit = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcitGiven = true;
            break;
        case BSIM3v32_MOD_LNFACTOR:
            this->BSIM3v32lnfactor = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lnfactorGiven = true;
            break;
        case BSIM3v32_MOD_LXJ:
            this->BSIM3v32lxj = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lxjGiven = true;
            break;
        case BSIM3v32_MOD_LVSAT:
            this->BSIM3v32lvsat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvsatGiven = true;
            break;

        case BSIM3v32_MOD_LA0:
            this->BSIM3v32la0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32la0Given = true;
            break;
        case BSIM3v32_MOD_LAGS:
            this->BSIM3v32lags = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lagsGiven = true;
            break;
        case BSIM3v32_MOD_LA1:
            this->BSIM3v32la1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32la1Given = true;
            break;
        case BSIM3v32_MOD_LA2:
            this->BSIM3v32la2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32la2Given = true;
            break;
        case BSIM3v32_MOD_LAT:
            this->BSIM3v32lat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32latGiven = true;
            break;
        case BSIM3v32_MOD_LKETA:
            this->BSIM3v32lketa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lketaGiven = true;
            break;
        case BSIM3v32_MOD_LNSUB:
            this->BSIM3v32lnsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lnsubGiven = true;
            break;
        case BSIM3v32_MOD_LNPEAK:
            this->BSIM3v32lnpeak = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lnpeakGiven = true;
            if (this->BSIM3v32lnpeak > 1.0e20)
                this->BSIM3v32lnpeak *= 1.0e-6;
            break;
        case BSIM3v32_MOD_LNGATE:
            this->BSIM3v32lngate = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lngateGiven = true;
            if (this->BSIM3v32lngate > 1.0e23)
                this->BSIM3v32lngate *= 1.0e-6;
            break;
        case BSIM3v32_MOD_LGAMMA1:
            this->BSIM3v32lgamma1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lgamma1Given = true;
            break;
        case BSIM3v32_MOD_LGAMMA2:
            this->BSIM3v32lgamma2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lgamma2Given = true;
            break;
        case BSIM3v32_MOD_LVBX:
            this->BSIM3v32lvbx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvbxGiven = true;
            break;
        case BSIM3v32_MOD_LVBM:
            this->BSIM3v32lvbm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvbmGiven = true;
            break;
        case BSIM3v32_MOD_LXT:
            this->BSIM3v32lxt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lxtGiven = true;
            break;
        case BSIM3v32_MOD_LK1:
            this->BSIM3v32lk1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lk1Given = true;
            break;
        case BSIM3v32_MOD_LKT1:
            this->BSIM3v32lkt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lkt1Given = true;
            break;
        case BSIM3v32_MOD_LKT1L:
            this->BSIM3v32lkt1l = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lkt1lGiven = true;
            break;
        case BSIM3v32_MOD_LKT2:
            this->BSIM3v32lkt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lkt2Given = true;
            break;
        case BSIM3v32_MOD_LK2:
            this->BSIM3v32lk2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lk2Given = true;
            break;
        case BSIM3v32_MOD_LK3:
            this->BSIM3v32lk3 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lk3Given = true;
            break;
        case BSIM3v32_MOD_LK3B:
            this->BSIM3v32lk3b = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lk3bGiven = true;
            break;
        case BSIM3v32_MOD_LNLX:
            this->BSIM3v32lnlx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lnlxGiven = true;
            break;
        case BSIM3v32_MOD_LW0:
            this->BSIM3v32lw0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lw0Given = true;
            break;
        case BSIM3v32_MOD_LDVT0:
            this->BSIM3v32ldvt0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt0Given = true;
            break;
        case BSIM3v32_MOD_LDVT1:
            this->BSIM3v32ldvt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt1Given = true;
            break;
        case BSIM3v32_MOD_LDVT2:
            this->BSIM3v32ldvt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt2Given = true;
            break;
        case BSIM3v32_MOD_LDVT0W:
            this->BSIM3v32ldvt0w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt0wGiven = true;
            break;
        case BSIM3v32_MOD_LDVT1W:
            this->BSIM3v32ldvt1w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt1wGiven = true;
            break;
        case BSIM3v32_MOD_LDVT2W:
            this->BSIM3v32ldvt2w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldvt2wGiven = true;
            break;
        case BSIM3v32_MOD_LDROUT:
            this->BSIM3v32ldrout = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldroutGiven = true;
            break;
        case BSIM3v32_MOD_LDSUB:
            this->BSIM3v32ldsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldsubGiven = true;
            break;
        case BSIM3v32_MOD_LVTH0:
            this->BSIM3v32lvth0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvth0Given = true;
            break;
        case BSIM3v32_MOD_LUA:
            this->BSIM3v32lua = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32luaGiven = true;
            break;
        case BSIM3v32_MOD_LUA1:
            this->BSIM3v32lua1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lua1Given = true;
            break;
        case BSIM3v32_MOD_LUB:
            this->BSIM3v32lub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lubGiven = true;
            break;
        case BSIM3v32_MOD_LUB1:
            this->BSIM3v32lub1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lub1Given = true;
            break;
        case BSIM3v32_MOD_LUC:
            this->BSIM3v32luc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lucGiven = true;
            break;
        case BSIM3v32_MOD_LUC1:
            this->BSIM3v32luc1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32luc1Given = true;
            break;
        case BSIM3v32_MOD_LU0:
            this->BSIM3v32lu0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lu0Given = true;
            break;
        case BSIM3v32_MOD_LUTE:
            this->BSIM3v32lute = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32luteGiven = true;
            break;
        case BSIM3v32_MOD_LVOFF:
            this->BSIM3v32lvoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvoffGiven = true;
            break;
        case BSIM3v32_MOD_LDELTA:
            this->BSIM3v32ldelta = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldeltaGiven = true;
            break;
        case BSIM3v32_MOD_LRDSW:
            this->BSIM3v32lrdsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lrdswGiven = true;
            break;
        case BSIM3v32_MOD_LPRWB:
            this->BSIM3v32lprwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lprwbGiven = true;
            break;
        case BSIM3v32_MOD_LPRWG:
            this->BSIM3v32lprwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lprwgGiven = true;
            break;
        case BSIM3v32_MOD_LPRT:
            this->BSIM3v32lprt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lprtGiven = true;
            break;
        case BSIM3v32_MOD_LETA0:
            this->BSIM3v32leta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32leta0Given = true;
            break;
        case BSIM3v32_MOD_LETAB:
            this->BSIM3v32letab = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32letabGiven = true;
            break;
        case BSIM3v32_MOD_LPCLM:
            this->BSIM3v32lpclm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpclmGiven = true;
            break;
        case BSIM3v32_MOD_LPDIBL1:
            this->BSIM3v32lpdibl1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpdibl1Given = true;
            break;
        case BSIM3v32_MOD_LPDIBL2:
            this->BSIM3v32lpdibl2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpdibl2Given = true;
            break;
        case BSIM3v32_MOD_LPDIBLB:
            this->BSIM3v32lpdiblb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpdiblbGiven = true;
            break;
        case BSIM3v32_MOD_LPSCBE1:
            this->BSIM3v32lpscbe1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpscbe1Given = true;
            break;
        case BSIM3v32_MOD_LPSCBE2:
            this->BSIM3v32lpscbe2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpscbe2Given = true;
            break;
        case BSIM3v32_MOD_LPVAG:
            this->BSIM3v32lpvag = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lpvagGiven = true;
            break;
        case BSIM3v32_MOD_LWR:
            this->BSIM3v32lwr = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lwrGiven = true;
            break;
        case BSIM3v32_MOD_LDWG:
            this->BSIM3v32ldwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldwgGiven = true;
            break;
        case BSIM3v32_MOD_LDWB:
            this->BSIM3v32ldwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ldwbGiven = true;
            break;
        case BSIM3v32_MOD_LB0:
            this->BSIM3v32lb0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lb0Given = true;
            break;
        case BSIM3v32_MOD_LB1:
            this->BSIM3v32lb1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lb1Given = true;
            break;
        case BSIM3v32_MOD_LALPHA0:
            this->BSIM3v32lalpha0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lalpha0Given = true;
            break;
        case BSIM3v32_MOD_LALPHA1:
            this->BSIM3v32lalpha1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lalpha1Given = true;
            break;
        case BSIM3v32_MOD_LBETA0:
            this->BSIM3v32lbeta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lbeta0Given = true;
            break;
        case BSIM3v32_MOD_LVFB:
            this->BSIM3v32lvfb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvfbGiven = true;
            break;

        case BSIM3v32_MOD_LELM:
            this->BSIM3v32lelm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lelmGiven = true;
            break;
        case BSIM3v32_MOD_LCGSL:
            this->BSIM3v32lcgsl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcgslGiven = true;
            break;
        case BSIM3v32_MOD_LCGDL:
            this->BSIM3v32lcgdl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcgdlGiven = true;
            break;
        case BSIM3v32_MOD_LCKAPPA:
            this->BSIM3v32lckappa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lckappaGiven = true;
            break;
        case BSIM3v32_MOD_LCF:
            this->BSIM3v32lcf = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcfGiven = true;
            break;
        case BSIM3v32_MOD_LCLC:
            this->BSIM3v32lclc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lclcGiven = true;
            break;
        case BSIM3v32_MOD_LCLE:
            this->BSIM3v32lcle = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lcleGiven = true;
            break;
        case BSIM3v32_MOD_LVFBCV:
            this->BSIM3v32lvfbcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvfbcvGiven = true;
            break;
        case BSIM3v32_MOD_LACDE:
            this->BSIM3v32lacde = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lacdeGiven = true;
            break;
        case BSIM3v32_MOD_LMOIN:
            this->BSIM3v32lmoin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lmoinGiven = true;
            break;
        case BSIM3v32_MOD_LNOFF:
            this->BSIM3v32lnoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lnoffGiven = true;
            break;
        case BSIM3v32_MOD_LVOFFCV:
            this->BSIM3v32lvoffcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32lvoffcvGiven = true;
            break;

        /* Width dependence */
        case BSIM3v32_MOD_WCDSC:
            this->BSIM3v32wcdsc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcdscGiven = true;
            break;

        case BSIM3v32_MOD_WCDSCB:
            this->BSIM3v32wcdscb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcdscbGiven = true;
            break;
        case BSIM3v32_MOD_WCDSCD:
            this->BSIM3v32wcdscd = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcdscdGiven = true;
            break;
        case BSIM3v32_MOD_WCIT:
            this->BSIM3v32wcit = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcitGiven = true;
            break;
        case BSIM3v32_MOD_WNFACTOR:
            this->BSIM3v32wnfactor = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wnfactorGiven = true;
            break;
        case BSIM3v32_MOD_WXJ:
            this->BSIM3v32wxj = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wxjGiven = true;
            break;
        case BSIM3v32_MOD_WVSAT:
            this->BSIM3v32wvsat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvsatGiven = true;
            break;

        case BSIM3v32_MOD_WA0:
            this->BSIM3v32wa0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wa0Given = true;
            break;
        case BSIM3v32_MOD_WAGS:
            this->BSIM3v32wags = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wagsGiven = true;
            break;
        case BSIM3v32_MOD_WA1:
            this->BSIM3v32wa1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wa1Given = true;
            break;
        case BSIM3v32_MOD_WA2:
            this->BSIM3v32wa2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wa2Given = true;
            break;
        case BSIM3v32_MOD_WAT:
            this->BSIM3v32wat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32watGiven = true;
            break;
        case BSIM3v32_MOD_WKETA:
            this->BSIM3v32wketa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wketaGiven = true;
            break;
        case BSIM3v32_MOD_WNSUB:
            this->BSIM3v32wnsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wnsubGiven = true;
            break;
        case BSIM3v32_MOD_WNPEAK:
            this->BSIM3v32wnpeak = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wnpeakGiven = true;
            if (this->BSIM3v32wnpeak > 1.0e20)
                this->BSIM3v32wnpeak *= 1.0e-6;
            break;
        case BSIM3v32_MOD_WNGATE:
            this->BSIM3v32wngate = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wngateGiven = true;
            if (this->BSIM3v32wngate > 1.0e23)
                this->BSIM3v32wngate *= 1.0e-6;
            break;
        case BSIM3v32_MOD_WGAMMA1:
            this->BSIM3v32wgamma1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wgamma1Given = true;
            break;
        case BSIM3v32_MOD_WGAMMA2:
            this->BSIM3v32wgamma2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wgamma2Given = true;
            break;
        case BSIM3v32_MOD_WVBX:
            this->BSIM3v32wvbx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvbxGiven = true;
            break;
        case BSIM3v32_MOD_WVBM:
            this->BSIM3v32wvbm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvbmGiven = true;
            break;
        case BSIM3v32_MOD_WXT:
            this->BSIM3v32wxt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wxtGiven = true;
            break;
        case BSIM3v32_MOD_WK1:
            this->BSIM3v32wk1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wk1Given = true;
            break;
        case BSIM3v32_MOD_WKT1:
            this->BSIM3v32wkt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wkt1Given = true;
            break;
        case BSIM3v32_MOD_WKT1L:
            this->BSIM3v32wkt1l = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wkt1lGiven = true;
            break;
        case BSIM3v32_MOD_WKT2:
            this->BSIM3v32wkt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wkt2Given = true;
            break;
        case BSIM3v32_MOD_WK2:
            this->BSIM3v32wk2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wk2Given = true;
            break;
        case BSIM3v32_MOD_WK3:
            this->BSIM3v32wk3 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wk3Given = true;
            break;
        case BSIM3v32_MOD_WK3B:
            this->BSIM3v32wk3b = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wk3bGiven = true;
            break;
        case BSIM3v32_MOD_WNLX:
            this->BSIM3v32wnlx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wnlxGiven = true;
            break;
        case BSIM3v32_MOD_WW0:
            this->BSIM3v32ww0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ww0Given = true;
            break;
        case BSIM3v32_MOD_WDVT0:
            this->BSIM3v32wdvt0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt0Given = true;
            break;
        case BSIM3v32_MOD_WDVT1:
            this->BSIM3v32wdvt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt1Given = true;
            break;
        case BSIM3v32_MOD_WDVT2:
            this->BSIM3v32wdvt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt2Given = true;
            break;
        case BSIM3v32_MOD_WDVT0W:
            this->BSIM3v32wdvt0w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt0wGiven = true;
            break;
        case BSIM3v32_MOD_WDVT1W:
            this->BSIM3v32wdvt1w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt1wGiven = true;
            break;
        case BSIM3v32_MOD_WDVT2W:
            this->BSIM3v32wdvt2w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdvt2wGiven = true;
            break;
        case BSIM3v32_MOD_WDROUT:
            this->BSIM3v32wdrout = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdroutGiven = true;
            break;
        case BSIM3v32_MOD_WDSUB:
            this->BSIM3v32wdsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdsubGiven = true;
            break;
        case BSIM3v32_MOD_WVTH0:
            this->BSIM3v32wvth0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvth0Given = true;
            break;
        case BSIM3v32_MOD_WUA:
            this->BSIM3v32wua = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wuaGiven = true;
            break;
        case BSIM3v32_MOD_WUA1:
            this->BSIM3v32wua1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wua1Given = true;
            break;
        case BSIM3v32_MOD_WUB:
            this->BSIM3v32wub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wubGiven = true;
            break;
        case BSIM3v32_MOD_WUB1:
            this->BSIM3v32wub1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wub1Given = true;
            break;
        case BSIM3v32_MOD_WUC:
            this->BSIM3v32wuc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wucGiven = true;
            break;
        case BSIM3v32_MOD_WUC1:
            this->BSIM3v32wuc1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wuc1Given = true;
            break;
        case BSIM3v32_MOD_WU0:
            this->BSIM3v32wu0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wu0Given = true;
            break;
        case BSIM3v32_MOD_WUTE:
            this->BSIM3v32wute = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wuteGiven = true;
            break;
        case BSIM3v32_MOD_WVOFF:
            this->BSIM3v32wvoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvoffGiven = true;
            break;
        case BSIM3v32_MOD_WDELTA:
            this->BSIM3v32wdelta = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdeltaGiven = true;
            break;
        case BSIM3v32_MOD_WRDSW:
            this->BSIM3v32wrdsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wrdswGiven = true;
            break;
        case BSIM3v32_MOD_WPRWB:
            this->BSIM3v32wprwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wprwbGiven = true;
            break;
        case BSIM3v32_MOD_WPRWG:
            this->BSIM3v32wprwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wprwgGiven = true;
            break;
        case BSIM3v32_MOD_WPRT:
            this->BSIM3v32wprt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wprtGiven = true;
            break;
        case BSIM3v32_MOD_WETA0:
            this->BSIM3v32weta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32weta0Given = true;
            break;
        case BSIM3v32_MOD_WETAB:
            this->BSIM3v32wetab = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wetabGiven = true;
            break;
        case BSIM3v32_MOD_WPCLM:
            this->BSIM3v32wpclm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpclmGiven = true;
            break;
        case BSIM3v32_MOD_WPDIBL1:
            this->BSIM3v32wpdibl1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpdibl1Given = true;
            break;
        case BSIM3v32_MOD_WPDIBL2:
            this->BSIM3v32wpdibl2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpdibl2Given = true;
            break;
        case BSIM3v32_MOD_WPDIBLB:
            this->BSIM3v32wpdiblb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpdiblbGiven = true;
            break;
        case BSIM3v32_MOD_WPSCBE1:
            this->BSIM3v32wpscbe1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpscbe1Given = true;
            break;
        case BSIM3v32_MOD_WPSCBE2:
            this->BSIM3v32wpscbe2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpscbe2Given = true;
            break;
        case BSIM3v32_MOD_WPVAG:
            this->BSIM3v32wpvag = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wpvagGiven = true;
            break;
        case BSIM3v32_MOD_WWR:
            this->BSIM3v32wwr = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wwrGiven = true;
            break;
        case BSIM3v32_MOD_WDWG:
            this->BSIM3v32wdwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdwgGiven = true;
            break;
        case BSIM3v32_MOD_WDWB:
            this->BSIM3v32wdwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wdwbGiven = true;
            break;
        case BSIM3v32_MOD_WB0:
            this->BSIM3v32wb0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wb0Given = true;
            break;
        case BSIM3v32_MOD_WB1:
            this->BSIM3v32wb1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wb1Given = true;
            break;
        case BSIM3v32_MOD_WALPHA0:
            this->BSIM3v32walpha0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32walpha0Given = true;
            break;
        case BSIM3v32_MOD_WALPHA1:
            this->BSIM3v32walpha1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32walpha1Given = true;
            break;
        case BSIM3v32_MOD_WBETA0:
            this->BSIM3v32wbeta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wbeta0Given = true;
            break;
        case BSIM3v32_MOD_WVFB:
            this->BSIM3v32wvfb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvfbGiven = true;
            break;

        case BSIM3v32_MOD_WELM:
            this->BSIM3v32welm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32welmGiven = true;
            break;
        case BSIM3v32_MOD_WCGSL:
            this->BSIM3v32wcgsl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcgslGiven = true;
            break;
        case BSIM3v32_MOD_WCGDL:
            this->BSIM3v32wcgdl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcgdlGiven = true;
            break;
        case BSIM3v32_MOD_WCKAPPA:
            this->BSIM3v32wckappa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wckappaGiven = true;
            break;
        case BSIM3v32_MOD_WCF:
            this->BSIM3v32wcf = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcfGiven = true;
            break;
        case BSIM3v32_MOD_WCLC:
            this->BSIM3v32wclc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wclcGiven = true;
            break;
        case BSIM3v32_MOD_WCLE:
            this->BSIM3v32wcle = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wcleGiven = true;
            break;
        case BSIM3v32_MOD_WVFBCV:
            this->BSIM3v32wvfbcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvfbcvGiven = true;
            break;
        case BSIM3v32_MOD_WACDE:
            this->BSIM3v32wacde = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wacdeGiven = true;
            break;
        case BSIM3v32_MOD_WMOIN:
            this->BSIM3v32wmoin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wmoinGiven = true;
            break;
        case BSIM3v32_MOD_WNOFF:
            this->BSIM3v32wnoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wnoffGiven = true;
            break;
        case BSIM3v32_MOD_WVOFFCV:
            this->BSIM3v32wvoffcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32wvoffcvGiven = true;
            break;

        /* Cross-term dependence */
        case BSIM3v32_MOD_PCDSC:
            this->BSIM3v32pcdsc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcdscGiven = true;
            break;

        case BSIM3v32_MOD_PCDSCB:
            this->BSIM3v32pcdscb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcdscbGiven = true;
            break;
        case BSIM3v32_MOD_PCDSCD:
            this->BSIM3v32pcdscd = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcdscdGiven = true;
            break;
        case BSIM3v32_MOD_PCIT:
            this->BSIM3v32pcit = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcitGiven = true;
            break;
        case BSIM3v32_MOD_PNFACTOR:
            this->BSIM3v32pnfactor = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pnfactorGiven = true;
            break;
        case BSIM3v32_MOD_PXJ:
            this->BSIM3v32pxj = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pxjGiven = true;
            break;
        case BSIM3v32_MOD_PVSAT:
            this->BSIM3v32pvsat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvsatGiven = true;
            break;

        case BSIM3v32_MOD_PA0:
            this->BSIM3v32pa0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pa0Given = true;
            break;
        case BSIM3v32_MOD_PAGS:
            this->BSIM3v32pags = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pagsGiven = true;
            break;
        case BSIM3v32_MOD_PA1:
            this->BSIM3v32pa1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pa1Given = true;
            break;
        case BSIM3v32_MOD_PA2:
            this->BSIM3v32pa2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pa2Given = true;
            break;
        case BSIM3v32_MOD_PAT:
            this->BSIM3v32pat = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32patGiven = true;
            break;
        case BSIM3v32_MOD_PKETA:
            this->BSIM3v32pketa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pketaGiven = true;
            break;
        case BSIM3v32_MOD_PNSUB:
            this->BSIM3v32pnsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pnsubGiven = true;
            break;
        case BSIM3v32_MOD_PNPEAK:
            this->BSIM3v32pnpeak = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pnpeakGiven = true;
            if (this->BSIM3v32pnpeak > 1.0e20)
                this->BSIM3v32pnpeak *= 1.0e-6;
            break;
        case BSIM3v32_MOD_PNGATE:
            this->BSIM3v32pngate = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pngateGiven = true;
            if (this->BSIM3v32pngate > 1.0e23)
                this->BSIM3v32pngate *= 1.0e-6;
            break;
        case BSIM3v32_MOD_PGAMMA1:
            this->BSIM3v32pgamma1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pgamma1Given = true;
            break;
        case BSIM3v32_MOD_PGAMMA2:
            this->BSIM3v32pgamma2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pgamma2Given = true;
            break;
        case BSIM3v32_MOD_PVBX:
            this->BSIM3v32pvbx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvbxGiven = true;
            break;
        case BSIM3v32_MOD_PVBM:
            this->BSIM3v32pvbm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvbmGiven = true;
            break;
        case BSIM3v32_MOD_PXT:
            this->BSIM3v32pxt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pxtGiven = true;
            break;
        case BSIM3v32_MOD_PK1:
            this->BSIM3v32pk1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pk1Given = true;
            break;
        case BSIM3v32_MOD_PKT1:
            this->BSIM3v32pkt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pkt1Given = true;
            break;
        case BSIM3v32_MOD_PKT1L:
            this->BSIM3v32pkt1l = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pkt1lGiven = true;
            break;
        case BSIM3v32_MOD_PKT2:
            this->BSIM3v32pkt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pkt2Given = true;
            break;
        case BSIM3v32_MOD_PK2:
            this->BSIM3v32pk2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pk2Given = true;
            break;
        case BSIM3v32_MOD_PK3:
            this->BSIM3v32pk3 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pk3Given = true;
            break;
        case BSIM3v32_MOD_PK3B:
            this->BSIM3v32pk3b = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pk3bGiven = true;
            break;
        case BSIM3v32_MOD_PNLX:
            this->BSIM3v32pnlx = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pnlxGiven = true;
            break;
        case BSIM3v32_MOD_PW0:
            this->BSIM3v32pw0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pw0Given = true;
            break;
        case BSIM3v32_MOD_PDVT0:
            this->BSIM3v32pdvt0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt0Given = true;
            break;
        case BSIM3v32_MOD_PDVT1:
            this->BSIM3v32pdvt1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt1Given = true;
            break;
        case BSIM3v32_MOD_PDVT2:
            this->BSIM3v32pdvt2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt2Given = true;
            break;
        case BSIM3v32_MOD_PDVT0W:
            this->BSIM3v32pdvt0w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt0wGiven = true;
            break;
        case BSIM3v32_MOD_PDVT1W:
            this->BSIM3v32pdvt1w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt1wGiven = true;
            break;
        case BSIM3v32_MOD_PDVT2W:
            this->BSIM3v32pdvt2w = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdvt2wGiven = true;
            break;
        case BSIM3v32_MOD_PDROUT:
            this->BSIM3v32pdrout = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdroutGiven = true;
            break;
        case BSIM3v32_MOD_PDSUB:
            this->BSIM3v32pdsub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdsubGiven = true;
            break;
        case BSIM3v32_MOD_PVTH0:
            this->BSIM3v32pvth0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvth0Given = true;
            break;
        case BSIM3v32_MOD_PUA:
            this->BSIM3v32pua = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32puaGiven = true;
            break;
        case BSIM3v32_MOD_PUA1:
            this->BSIM3v32pua1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pua1Given = true;
            break;
        case BSIM3v32_MOD_PUB:
            this->BSIM3v32pub = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pubGiven = true;
            break;
        case BSIM3v32_MOD_PUB1:
            this->BSIM3v32pub1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pub1Given = true;
            break;
        case BSIM3v32_MOD_PUC:
            this->BSIM3v32puc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pucGiven = true;
            break;
        case BSIM3v32_MOD_PUC1:
            this->BSIM3v32puc1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32puc1Given = true;
            break;
        case BSIM3v32_MOD_PU0:
            this->BSIM3v32pu0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pu0Given = true;
            break;
        case BSIM3v32_MOD_PUTE:
            this->BSIM3v32pute = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32puteGiven = true;
            break;
        case BSIM3v32_MOD_PVOFF:
            this->BSIM3v32pvoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvoffGiven = true;
            break;
        case BSIM3v32_MOD_PDELTA:
            this->BSIM3v32pdelta = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdeltaGiven = true;
            break;
        case BSIM3v32_MOD_PRDSW:
            this->BSIM3v32prdsw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32prdswGiven = true;
            break;
        case BSIM3v32_MOD_PPRWB:
            this->BSIM3v32pprwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pprwbGiven = true;
            break;
        case BSIM3v32_MOD_PPRWG:
            this->BSIM3v32pprwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pprwgGiven = true;
            break;
        case BSIM3v32_MOD_PPRT:
            this->BSIM3v32pprt = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pprtGiven = true;
            break;
        case BSIM3v32_MOD_PETA0:
            this->BSIM3v32peta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32peta0Given = true;
            break;
        case BSIM3v32_MOD_PETAB:
            this->BSIM3v32petab = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32petabGiven = true;
            break;
        case BSIM3v32_MOD_PPCLM:
            this->BSIM3v32ppclm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppclmGiven = true;
            break;
        case BSIM3v32_MOD_PPDIBL1:
            this->BSIM3v32ppdibl1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppdibl1Given = true;
            break;
        case BSIM3v32_MOD_PPDIBL2:
            this->BSIM3v32ppdibl2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppdibl2Given = true;
            break;
        case BSIM3v32_MOD_PPDIBLB:
            this->BSIM3v32ppdiblb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppdiblbGiven = true;
            break;
        case BSIM3v32_MOD_PPSCBE1:
            this->BSIM3v32ppscbe1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppscbe1Given = true;
            break;
        case BSIM3v32_MOD_PPSCBE2:
            this->BSIM3v32ppscbe2 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppscbe2Given = true;
            break;
        case BSIM3v32_MOD_PPVAG:
            this->BSIM3v32ppvag = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32ppvagGiven = true;
            break;
        case BSIM3v32_MOD_PWR:
            this->BSIM3v32pwr = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pwrGiven = true;
            break;
        case BSIM3v32_MOD_PDWG:
            this->BSIM3v32pdwg = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdwgGiven = true;
            break;
        case BSIM3v32_MOD_PDWB:
            this->BSIM3v32pdwb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pdwbGiven = true;
            break;
        case BSIM3v32_MOD_PB0:
            this->BSIM3v32pb0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pb0Given = true;
            break;
        case BSIM3v32_MOD_PB1:
            this->BSIM3v32pb1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pb1Given = true;
            break;
        case BSIM3v32_MOD_PALPHA0:
            this->BSIM3v32palpha0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32palpha0Given = true;
            break;
        case BSIM3v32_MOD_PALPHA1:
            this->BSIM3v32palpha1 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32palpha1Given = true;
            break;
        case BSIM3v32_MOD_PBETA0:
            this->BSIM3v32pbeta0 = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pbeta0Given = true;
            break;
        case BSIM3v32_MOD_PVFB:
            this->BSIM3v32pvfb = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvfbGiven = true;
            break;

        case BSIM3v32_MOD_PELM:
            this->BSIM3v32pelm = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pelmGiven = true;
            break;
        case BSIM3v32_MOD_PCGSL:
            this->BSIM3v32pcgsl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcgslGiven = true;
            break;
        case BSIM3v32_MOD_PCGDL:
            this->BSIM3v32pcgdl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcgdlGiven = true;
            break;
        case BSIM3v32_MOD_PCKAPPA:
            this->BSIM3v32pckappa = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pckappaGiven = true;
            break;
        case BSIM3v32_MOD_PCF:
            this->BSIM3v32pcf = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcfGiven = true;
            break;
        case BSIM3v32_MOD_PCLC:
            this->BSIM3v32pclc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pclcGiven = true;
            break;
        case BSIM3v32_MOD_PCLE:
            this->BSIM3v32pcle = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pcleGiven = true;
            break;
        case BSIM3v32_MOD_PVFBCV:
            this->BSIM3v32pvfbcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvfbcvGiven = true;
            break;
        case BSIM3v32_MOD_PACDE:
            this->BSIM3v32pacde = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pacdeGiven = true;
            break;
        case BSIM3v32_MOD_PMOIN:
            this->BSIM3v32pmoin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pmoinGiven = true;
            break;
        case BSIM3v32_MOD_PNOFF:
            this->BSIM3v32pnoff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pnoffGiven = true;
            break;
        case BSIM3v32_MOD_PVOFFCV:
            this->BSIM3v32pvoffcv = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32pvoffcvGiven = true;
            break;

        case BSIM3v32_MOD_TNOM:
            this->BSIM3v32tnom = value.getDoubleCast(&rc) + CONSTCtoK;
            UPDATE_RC(rc);
            this->BSIM3v32tnomGiven = true;
            break;
        case BSIM3v32_MOD_CGSO:
            this->BSIM3v32cgso = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cgsoGiven = true;
            break;
        case BSIM3v32_MOD_CGDO:
            this->BSIM3v32cgdo = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cgdoGiven = true;
            break;
        case BSIM3v32_MOD_CGBO:
            this->BSIM3v32cgbo = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32cgboGiven = true;
            break;
        case BSIM3v32_MOD_XPART:
            this->BSIM3v32xpart = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32xpartGiven = true;
            break;
        case BSIM3v32_MOD_RSH:
            this->BSIM3v32sheetResistance = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32sheetResistanceGiven = true;
            break;
        case BSIM3v32_MOD_JS:
            this->BSIM3v32jctSatCurDensity = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32jctSatCurDensityGiven = true;
            break;
        case BSIM3v32_MOD_JSW:
            this->BSIM3v32jctSidewallSatCurDensity = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32jctSidewallSatCurDensityGiven = true;
            break;
        case BSIM3v32_MOD_PB:
            this->BSIM3v32bulkJctPotential = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32bulkJctPotentialGiven = true;
            break;
        case BSIM3v32_MOD_MJ:
            this->BSIM3v32bulkJctBotGradingCoeff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32bulkJctBotGradingCoeffGiven = true;
            break;
        case BSIM3v32_MOD_PBSW:
            this->BSIM3v32sidewallJctPotential = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32sidewallJctPotentialGiven = true;
            break;
        case BSIM3v32_MOD_MJSW:
            this->BSIM3v32bulkJctSideGradingCoeff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32bulkJctSideGradingCoeffGiven = true;
            break;
        case BSIM3v32_MOD_CJ:
            this->BSIM3v32unitAreaJctCap = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32unitAreaJctCapGiven = true;
            break;
        case BSIM3v32_MOD_CJSW:
            this->BSIM3v32unitLengthSidewallJctCap = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32unitLengthSidewallJctCapGiven = true;
            break;
        case BSIM3v32_MOD_NJ:
            this->BSIM3v32jctEmissionCoeff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32jctEmissionCoeffGiven = true;
            break;
        case BSIM3v32_MOD_PBSWG:
            this->BSIM3v32GatesidewallJctPotential = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32GatesidewallJctPotentialGiven = true;
            break;
        case BSIM3v32_MOD_MJSWG:
            this->BSIM3v32bulkJctGateSideGradingCoeff = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32bulkJctGateSideGradingCoeffGiven = true;
            break;
        case BSIM3v32_MOD_CJSWG:
            this->BSIM3v32unitLengthGateSidewallJctCap = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32unitLengthGateSidewallJctCapGiven = true;
            break;
        case BSIM3v32_MOD_XTI:
            this->BSIM3v32jctTempExponent = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32jctTempExponentGiven = true;
            break;
        case BSIM3v32_MOD_LINT:
            this->BSIM3v32Lint = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LintGiven = true;
            break;
        case BSIM3v32_MOD_LL:
            this->BSIM3v32Ll = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LlGiven = true;
            break;
        case BSIM3v32_MOD_LLC:
            this->BSIM3v32Llc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LlcGiven = true;
            break;
        case BSIM3v32_MOD_LLN:
            this->BSIM3v32Lln = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LlnGiven = true;
            break;
        case BSIM3v32_MOD_LW:
            this->BSIM3v32Lw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LwGiven = true;
            break;
        case BSIM3v32_MOD_LWC:
            this->BSIM3v32Lwc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LwcGiven = true;
            break;
        case BSIM3v32_MOD_LWN:
            this->BSIM3v32Lwn = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LwnGiven = true;
            break;
        case BSIM3v32_MOD_LWL:
            this->BSIM3v32Lwl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LwlGiven = true;
            break;
        case BSIM3v32_MOD_LWLC:
            this->BSIM3v32Lwlc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LwlcGiven = true;
            break;
        case BSIM3v32_MOD_LMIN:
            this->BSIM3v32Lmin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LminGiven = true;
            break;
        case BSIM3v32_MOD_LMAX:
            this->BSIM3v32Lmax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32LmaxGiven = true;
            break;
        case BSIM3v32_MOD_WINT:
            this->BSIM3v32Wint = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WintGiven = true;
            break;
        case BSIM3v32_MOD_WL:
            this->BSIM3v32Wl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WlGiven = true;
            break;
        case BSIM3v32_MOD_WLC:
            this->BSIM3v32Wlc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WlcGiven = true;
            break;
        case BSIM3v32_MOD_WLN:
            this->BSIM3v32Wln = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WlnGiven = true;
            break;
        case BSIM3v32_MOD_WW:
            this->BSIM3v32Ww = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WwGiven = true;
            break;
        case BSIM3v32_MOD_WWC:
            this->BSIM3v32Wwc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WwcGiven = true;
            break;
        case BSIM3v32_MOD_WWN:
            this->BSIM3v32Wwn = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WwnGiven = true;
            break;
        case BSIM3v32_MOD_WWL:
            this->BSIM3v32Wwl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WwlGiven = true;
            break;
        case BSIM3v32_MOD_WWLC:
            this->BSIM3v32Wwlc = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WwlcGiven = true;
            break;
        case BSIM3v32_MOD_WMIN:
            this->BSIM3v32Wmin = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WminGiven = true;
            break;
        case BSIM3v32_MOD_WMAX:
            this->BSIM3v32Wmax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32WmaxGiven = true;
            break;

        case BSIM3v32_MOD_XL:
            this->BSIM3v32xl = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32xlGiven = true;
            break;
        case BSIM3v32_MOD_XW:
            this->BSIM3v32xw = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32xwGiven = true;
            break;

        case BSIM3v32_MOD_NOIA:
            this->BSIM3v32oxideTrapDensityA = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32oxideTrapDensityAGiven = true;
            break;
        case BSIM3v32_MOD_NOIB:
            this->BSIM3v32oxideTrapDensityB = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32oxideTrapDensityBGiven = true;
            break;
        case BSIM3v32_MOD_NOIC:
            this->BSIM3v32oxideTrapDensityC = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32oxideTrapDensityCGiven = true;
            break;
        case BSIM3v32_MOD_EM:
            this->BSIM3v32em = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32emGiven = true;
            break;
        case BSIM3v32_MOD_EF:
            this->BSIM3v32ef = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32efGiven = true;
            break;
        case BSIM3v32_MOD_AF:
            this->BSIM3v32af = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32afGiven = true;
            break;
        case BSIM3v32_MOD_KF:
            this->BSIM3v32kf = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32kfGiven = true;
            break;

        case BSIM3v32_MOD_VGS_MAX:
            this->BSIM3v32vgsMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgsMaxGiven = true;
            break;
        case BSIM3v32_MOD_VGD_MAX:
            this->BSIM3v32vgdMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgdMaxGiven = true;
            break;
        case BSIM3v32_MOD_VGB_MAX:
            this->BSIM3v32vgbMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgbMaxGiven = true;
            break;
        case BSIM3v32_MOD_VDS_MAX:
            this->BSIM3v32vdsMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vdsMaxGiven = true;
            break;
        case BSIM3v32_MOD_VBS_MAX:
            this->BSIM3v32vbsMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbsMaxGiven = true;
            break;
        case BSIM3v32_MOD_VBD_MAX:
            this->BSIM3v32vbdMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbdMaxGiven = true;
            break;
        case BSIM3v32_MOD_VGSR_MAX:
            this->BSIM3v32vgsrMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgsrMaxGiven = true;
            break;
        case BSIM3v32_MOD_VGDR_MAX:
            this->BSIM3v32vgdrMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgdrMaxGiven = true;
            break;
        case BSIM3v32_MOD_VGBR_MAX:
            this->BSIM3v32vgbrMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vgbrMaxGiven = true;
            break;
        case BSIM3v32_MOD_VBSR_MAX:
            this->BSIM3v32vbsrMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbsrMaxGiven = true;
            break;
        case BSIM3v32_MOD_VBDR_MAX:
            this->BSIM3v32vbdrMax = value.getDoubleCast(&rc);
            UPDATE_RC(rc);
            this->BSIM3v32vbdrMaxGiven = true;
            break;

        case BSIM3v32_MOD_NMOS:
        {
            bool s = value.getBooleanCast(&rc);
            UPDATE_RC(rc);
            if (s)
            {
                this->BSIM3v32type = BSIM3V32::NMOS;
                this->BSIM3v32typeGiven = true;
            }
            break;
        }
        case BSIM3v32_MOD_PMOS:
        {
            bool s = value.getBooleanCast(&rc);
            UPDATE_RC(rc);
            if (s)
            {
                this->BSIM3v32type = BSIM3V32::PMOS;
                this->BSIM3v32typeGiven = true;
            }
            break;
        }
        default:
            return CERR_NO_SUCH_PROPERTY;
        }
        return 0;
    }

    int BSIM3v32model::setup(Environment *env)
    {
        /* Default value Processing for BSIM3v32 MOSFET Models */
        if (!this->BSIM3v32typeGiven)
            this->BSIM3v32type = BSIM3V32::NMOS;
        if (!this->BSIM3v32mobModGiven)
            this->BSIM3v32mobMod = 1;
        if (!this->BSIM3v32binUnitGiven)
            this->BSIM3v32binUnit = 1;
        if (!this->BSIM3v32paramChkGiven)
            this->BSIM3v32paramChk = 0;
        if (!this->BSIM3v32capModGiven)
            this->BSIM3v32capMod = 3;
        if (!this->BSIM3v32acmModGiven)
            this->BSIM3v32acmMod = 0;
        if (!this->BSIM3v32calcacmGiven)
            this->BSIM3v32calcacm = 0;
        if (!this->BSIM3v32noiModGiven)
            this->BSIM3v32noiMod = 1;
        if (!this->BSIM3v32nqsModGiven)
            this->BSIM3v32nqsMod = 0;
        else if ((this->BSIM3v32nqsMod != 0) && (this->BSIM3v32nqsMod != 1))
        {
            this->BSIM3v32nqsMod = 0;
            env->logging(Environment::WARN, "Warning: nqsMod has been set to its default value: 0.\n");
        }

        /* If the user does not provide the model revision,
             * we always choose the most recent.
             */
        if (!this->BSIM3v32versionGiven)
            this->BSIM3v32version = "3.2.4";

        /* I have added below the code that translate model string
             * into an integer. This trick is meant to speed up the
             * revision testing instruction, since comparing integer
             * is faster than comparing strings.
             * Paolo Nenzi 2002
             */
        if ((!strcmp(this->BSIM3v32version, "3.2.4")) || (!strncmp(this->BSIM3v32version, "3.24", 4)))
            this->BSIM3v32intVersion = BSIM3v32V324;
        else if ((!strcmp(this->BSIM3v32version, "3.2.3")) || (!strncmp(this->BSIM3v32version, "3.23", 4)))
            this->BSIM3v32intVersion = BSIM3v32V323;
        else if ((!strcmp(this->BSIM3v32version, "3.2.2")) || (!strncmp(this->BSIM3v32version, "3.22", 4)))
            this->BSIM3v32intVersion = BSIM3v32V322;
        else if ((!strncmp(this->BSIM3v32version, "3.2", 3)) || (!strncmp(this->BSIM3v32version, "3.20", 4)))
            this->BSIM3v32intVersion = BSIM3v32V32;
        else
            this->BSIM3v32intVersion = BSIM3v32V3OLD;
        /* BSIM3v32V3OLD is a placeholder for pre 3.2 revision
            * This model should not be used for pre 3.2 models.
            */

        if (!this->BSIM3v32toxGiven)
            this->BSIM3v32tox = 150.0e-10;
        this->BSIM3v32cox = 3.453133e-11 / this->BSIM3v32tox;
        if (!this->BSIM3v32toxmGiven)
            this->BSIM3v32toxm = this->BSIM3v32tox;

        if (!this->BSIM3v32cdscGiven)
            this->BSIM3v32cdsc = 2.4e-4; /* unit Q/V/m^2  */
        if (!this->BSIM3v32cdscbGiven)
            this->BSIM3v32cdscb = 0.0; /* unit Q/V/m^2  */
        if (!this->BSIM3v32cdscdGiven)
            this->BSIM3v32cdscd = 0.0; /* unit Q/V/m^2  */
        if (!this->BSIM3v32citGiven)
            this->BSIM3v32cit = 0.0; /* unit Q/V/m^2  */
        if (!this->BSIM3v32nfactorGiven)
            this->BSIM3v32nfactor = 1;
        if (!this->BSIM3v32xjGiven)
            this->BSIM3v32xj = .15e-6;
        if (!this->BSIM3v32vsatGiven)
            this->BSIM3v32vsat = 8.0e4; /* unit m/s */
        if (!this->BSIM3v32atGiven)
            this->BSIM3v32at = 3.3e4; /* unit m/s */
        if (!this->BSIM3v32a0Given)
            this->BSIM3v32a0 = 1.0;
        if (!this->BSIM3v32agsGiven)
            this->BSIM3v32ags = 0.0;
        if (!this->BSIM3v32a1Given)
            this->BSIM3v32a1 = 0.0;
        if (!this->BSIM3v32a2Given)
            this->BSIM3v32a2 = 1.0;
        if (!this->BSIM3v32ketaGiven)
            this->BSIM3v32keta = -0.047; /* unit  / V */
        if (!this->BSIM3v32nsubGiven)
            this->BSIM3v32nsub = 6.0e16; /* unit 1/cm3 */
        if (!this->BSIM3v32npeakGiven)
            this->BSIM3v32npeak = 1.7e17; /* unit 1/cm3 */
        if (!this->BSIM3v32ngateGiven)
            this->BSIM3v32ngate = 0; /* unit 1/cm3 */
        if (!this->BSIM3v32vbmGiven)
            this->BSIM3v32vbm = -3.0;
        if (!this->BSIM3v32xtGiven)
            this->BSIM3v32xt = 1.55e-7;
        if (!this->BSIM3v32kt1Given)
            this->BSIM3v32kt1 = -0.11; /* unit V */
        if (!this->BSIM3v32kt1lGiven)
            this->BSIM3v32kt1l = 0.0; /* unit V*m */
        if (!this->BSIM3v32kt2Given)
            this->BSIM3v32kt2 = 0.022; /* No unit */
        if (!this->BSIM3v32k3Given)
            this->BSIM3v32k3 = 80.0;
        if (!this->BSIM3v32k3bGiven)
            this->BSIM3v32k3b = 0.0;
        if (!this->BSIM3v32w0Given)
            this->BSIM3v32w0 = 2.5e-6;
        if (!this->BSIM3v32nlxGiven)
            this->BSIM3v32nlx = 1.74e-7;
        if (!this->BSIM3v32dvt0Given)
            this->BSIM3v32dvt0 = 2.2;
        if (!this->BSIM3v32dvt1Given)
            this->BSIM3v32dvt1 = 0.53;
        if (!this->BSIM3v32dvt2Given)
            this->BSIM3v32dvt2 = -0.032; /* unit 1 / V */

        if (!this->BSIM3v32dvt0wGiven)
            this->BSIM3v32dvt0w = 0.0;
        if (!this->BSIM3v32dvt1wGiven)
            this->BSIM3v32dvt1w = 5.3e6;
        if (!this->BSIM3v32dvt2wGiven)
            this->BSIM3v32dvt2w = -0.032;

        if (!this->BSIM3v32droutGiven)
            this->BSIM3v32drout = 0.56;
        if (!this->BSIM3v32dsubGiven)
            this->BSIM3v32dsub = this->BSIM3v32drout;
        if (!this->BSIM3v32vth0Given)
            this->BSIM3v32vth0 = (this->BSIM3v32type == BSIM3V32::NMOS) ? 0.7 : -0.7;
        if (!this->BSIM3v32uaGiven)
            this->BSIM3v32ua = 2.25e-9; /* unit m/V */
        if (!this->BSIM3v32ua1Given)
            this->BSIM3v32ua1 = 4.31e-9; /* unit m/V */
        if (!this->BSIM3v32ubGiven)
            this->BSIM3v32ub = 5.87e-19; /* unit (m/V)**2 */
        if (!this->BSIM3v32ub1Given)
            this->BSIM3v32ub1 = -7.61e-18; /* unit (m/V)**2 */
        if (!this->BSIM3v32ucGiven)
            this->BSIM3v32uc = (this->BSIM3v32mobMod == 3) ? -0.0465 : -0.0465e-9;
        if (!this->BSIM3v32uc1Given)
            this->BSIM3v32uc1 = (this->BSIM3v32mobMod == 3) ? -0.056 : -0.056e-9;
        if (!this->BSIM3v32u0Given)
            this->BSIM3v32u0 = (this->BSIM3v32type == BSIM3V32::NMOS) ? 0.067 : 0.025;
        if (!this->BSIM3v32uteGiven)
            this->BSIM3v32ute = -1.5;
        if (!this->BSIM3v32voffGiven)
            this->BSIM3v32voff = -0.08;
        if (!this->BSIM3v32deltaGiven)
            this->BSIM3v32delta = 0.01;
        if (!this->BSIM3v32rdswGiven)
            this->BSIM3v32rdsw = 0;
        if (!this->BSIM3v32prwgGiven)
            this->BSIM3v32prwg = 0.0; /* unit 1/V */
        if (!this->BSIM3v32prwbGiven)
            this->BSIM3v32prwb = 0.0;
        if (!this->BSIM3v32prtGiven)
            this->BSIM3v32prt = 0.0;
        if (!this->BSIM3v32eta0Given)
            this->BSIM3v32eta0 = 0.08; /* no unit  */
        if (!this->BSIM3v32etabGiven)
            this->BSIM3v32etab = -0.07; /* unit  1/V */
        if (!this->BSIM3v32pclmGiven)
            this->BSIM3v32pclm = 1.3; /* no unit  */
        if (!this->BSIM3v32pdibl1Given)
            this->BSIM3v32pdibl1 = .39; /* no unit  */
        if (!this->BSIM3v32pdibl2Given)
            this->BSIM3v32pdibl2 = 0.0086; /* no unit  */
        if (!this->BSIM3v32pdiblbGiven)
            this->BSIM3v32pdiblb = 0.0; /* 1/V  */
        if (!this->BSIM3v32pscbe1Given)
            this->BSIM3v32pscbe1 = 4.24e8;
        if (!this->BSIM3v32pscbe2Given)
            this->BSIM3v32pscbe2 = 1.0e-5;
        if (!this->BSIM3v32pvagGiven)
            this->BSIM3v32pvag = 0.0;
        if (!this->BSIM3v32wrGiven)
            this->BSIM3v32wr = 1.0;
        if (!this->BSIM3v32dwgGiven)
            this->BSIM3v32dwg = 0.0;
        if (!this->BSIM3v32dwbGiven)
            this->BSIM3v32dwb = 0.0;
        if (!this->BSIM3v32b0Given)
            this->BSIM3v32b0 = 0.0;
        if (!this->BSIM3v32b1Given)
            this->BSIM3v32b1 = 0.0;
        if (!this->BSIM3v32alpha0Given)
            this->BSIM3v32alpha0 = 0.0;
        if (!this->BSIM3v32alpha1Given)
            this->BSIM3v32alpha1 = 0.0;
        if (!this->BSIM3v32beta0Given)
            this->BSIM3v32beta0 = 30.0;
        if (!this->BSIM3v32ijthGiven)
            this->BSIM3v32ijth = 0.1; /* unit A */

        if (!this->BSIM3v32elmGiven)
            this->BSIM3v32elm = 5.0;
        if (!this->BSIM3v32cgslGiven)
            this->BSIM3v32cgsl = 0.0;
        if (!this->BSIM3v32cgdlGiven)
            this->BSIM3v32cgdl = 0.0;
        if (!this->BSIM3v32ckappaGiven)
            this->BSIM3v32ckappa = 0.6;
        if (!this->BSIM3v32clcGiven)
            this->BSIM3v32clc = 0.1e-6;
        if (!this->BSIM3v32cleGiven)
            this->BSIM3v32cle = 0.6;
        if (!this->BSIM3v32vfbcvGiven)
            this->BSIM3v32vfbcv = -1.0;
        if (!this->BSIM3v32acdeGiven)
            this->BSIM3v32acde = 1.0;
        if (!this->BSIM3v32moinGiven)
            this->BSIM3v32moin = 15.0;
        if (!this->BSIM3v32noffGiven)
            this->BSIM3v32noff = 1.0;
        if (!this->BSIM3v32voffcvGiven)
            this->BSIM3v32voffcv = 0.0;
        if (!this->BSIM3v32tcjGiven)
            this->BSIM3v32tcj = 0.0;
        if (!this->BSIM3v32tpbGiven)
            this->BSIM3v32tpb = 0.0;
        if (!this->BSIM3v32tcjswGiven)
            this->BSIM3v32tcjsw = 0.0;
        if (!this->BSIM3v32tpbswGiven)
            this->BSIM3v32tpbsw = 0.0;
        if (!this->BSIM3v32tcjswgGiven)
            this->BSIM3v32tcjswg = 0.0;
        if (!this->BSIM3v32tpbswgGiven)
            this->BSIM3v32tpbswg = 0.0;

        /* ACM model */
        if (!this->BSIM3v32hdifGiven)
            this->BSIM3v32hdif = 0.0;
        if (!this->BSIM3v32ldifGiven)
            this->BSIM3v32ldif = 0.0;
        if (!this->BSIM3v32ldGiven)
            this->BSIM3v32ld = 0.0;
        if (!this->BSIM3v32rdGiven)
            this->BSIM3v32rd = 0.0;
        if (!this->BSIM3v32rsGiven)
            this->BSIM3v32rs = 0.0;
        if (!this->BSIM3v32rdcGiven)
            this->BSIM3v32rdc = 0.0;
        if (!this->BSIM3v32rscGiven)
            this->BSIM3v32rsc = 0.0;
        if (!this->BSIM3v32wmltGiven)
            this->BSIM3v32wmlt = 1.0;

        if (!this->BSIM3v32lmltGiven)
            this->BSIM3v32lmlt = 1.0;

        /* Length dependence */
        if (!this->BSIM3v32lcdscGiven)
            this->BSIM3v32lcdsc = 0.0;
        if (!this->BSIM3v32lcdscbGiven)
            this->BSIM3v32lcdscb = 0.0;
        if (!this->BSIM3v32lcdscdGiven)
            this->BSIM3v32lcdscd = 0.0;
        if (!this->BSIM3v32lcitGiven)
            this->BSIM3v32lcit = 0.0;
        if (!this->BSIM3v32lnfactorGiven)
            this->BSIM3v32lnfactor = 0.0;
        if (!this->BSIM3v32lxjGiven)
            this->BSIM3v32lxj = 0.0;
        if (!this->BSIM3v32lvsatGiven)
            this->BSIM3v32lvsat = 0.0;
        if (!this->BSIM3v32latGiven)
            this->BSIM3v32lat = 0.0;
        if (!this->BSIM3v32la0Given)
            this->BSIM3v32la0 = 0.0;
        if (!this->BSIM3v32lagsGiven)
            this->BSIM3v32lags = 0.0;
        if (!this->BSIM3v32la1Given)
            this->BSIM3v32la1 = 0.0;
        if (!this->BSIM3v32la2Given)
            this->BSIM3v32la2 = 0.0;
        if (!this->BSIM3v32lketaGiven)
            this->BSIM3v32lketa = 0.0;
        if (!this->BSIM3v32lnsubGiven)
            this->BSIM3v32lnsub = 0.0;
        if (!this->BSIM3v32lnpeakGiven)
            this->BSIM3v32lnpeak = 0.0;
        if (!this->BSIM3v32lngateGiven)
            this->BSIM3v32lngate = 0.0;
        if (!this->BSIM3v32lvbmGiven)
            this->BSIM3v32lvbm = 0.0;
        if (!this->BSIM3v32lxtGiven)
            this->BSIM3v32lxt = 0.0;
        if (!this->BSIM3v32lkt1Given)
            this->BSIM3v32lkt1 = 0.0;
        if (!this->BSIM3v32lkt1lGiven)
            this->BSIM3v32lkt1l = 0.0;
        if (!this->BSIM3v32lkt2Given)
            this->BSIM3v32lkt2 = 0.0;
        if (!this->BSIM3v32lk3Given)
            this->BSIM3v32lk3 = 0.0;
        if (!this->BSIM3v32lk3bGiven)
            this->BSIM3v32lk3b = 0.0;
        if (!this->BSIM3v32lw0Given)
            this->BSIM3v32lw0 = 0.0;
        if (!this->BSIM3v32lnlxGiven)
            this->BSIM3v32lnlx = 0.0;
        if (!this->BSIM3v32ldvt0Given)
            this->BSIM3v32ldvt0 = 0.0;
        if (!this->BSIM3v32ldvt1Given)
            this->BSIM3v32ldvt1 = 0.0;
        if (!this->BSIM3v32ldvt2Given)
            this->BSIM3v32ldvt2 = 0.0;
        if (!this->BSIM3v32ldvt0wGiven)
            this->BSIM3v32ldvt0w = 0.0;
        if (!this->BSIM3v32ldvt1wGiven)
            this->BSIM3v32ldvt1w = 0.0;
        if (!this->BSIM3v32ldvt2wGiven)
            this->BSIM3v32ldvt2w = 0.0;
        if (!this->BSIM3v32ldroutGiven)
            this->BSIM3v32ldrout = 0.0;
        if (!this->BSIM3v32ldsubGiven)
            this->BSIM3v32ldsub = 0.0;
        if (!this->BSIM3v32lvth0Given)
            this->BSIM3v32lvth0 = 0.0;
        if (!this->BSIM3v32luaGiven)
            this->BSIM3v32lua = 0.0;
        if (!this->BSIM3v32lua1Given)
            this->BSIM3v32lua1 = 0.0;
        if (!this->BSIM3v32lubGiven)
            this->BSIM3v32lub = 0.0;
        if (!this->BSIM3v32lub1Given)
            this->BSIM3v32lub1 = 0.0;
        if (!this->BSIM3v32lucGiven)
            this->BSIM3v32luc = 0.0;
        if (!this->BSIM3v32luc1Given)
            this->BSIM3v32luc1 = 0.0;
        if (!this->BSIM3v32lu0Given)
            this->BSIM3v32lu0 = 0.0;
        if (!this->BSIM3v32luteGiven)
            this->BSIM3v32lute = 0.0;
        if (!this->BSIM3v32lvoffGiven)
            this->BSIM3v32lvoff = 0.0;
        if (!this->BSIM3v32ldeltaGiven)
            this->BSIM3v32ldelta = 0.0;
        if (!this->BSIM3v32lrdswGiven)
            this->BSIM3v32lrdsw = 0.0;
        if (!this->BSIM3v32lprwbGiven)
            this->BSIM3v32lprwb = 0.0;
        if (!this->BSIM3v32lprwgGiven)
            this->BSIM3v32lprwg = 0.0;
        if (!this->BSIM3v32lprtGiven)
            this->BSIM3v32lprt = 0.0;
        if (!this->BSIM3v32leta0Given)
            this->BSIM3v32leta0 = 0.0;
        if (!this->BSIM3v32letabGiven)
            this->BSIM3v32letab = -0.0;
        if (!this->BSIM3v32lpclmGiven)
            this->BSIM3v32lpclm = 0.0;
        if (!this->BSIM3v32lpdibl1Given)
            this->BSIM3v32lpdibl1 = 0.0;
        if (!this->BSIM3v32lpdibl2Given)
            this->BSIM3v32lpdibl2 = 0.0;
        if (!this->BSIM3v32lpdiblbGiven)
            this->BSIM3v32lpdiblb = 0.0;
        if (!this->BSIM3v32lpscbe1Given)
            this->BSIM3v32lpscbe1 = 0.0;
        if (!this->BSIM3v32lpscbe2Given)
            this->BSIM3v32lpscbe2 = 0.0;
        if (!this->BSIM3v32lpvagGiven)
            this->BSIM3v32lpvag = 0.0;
        if (!this->BSIM3v32lwrGiven)
            this->BSIM3v32lwr = 0.0;
        if (!this->BSIM3v32ldwgGiven)
            this->BSIM3v32ldwg = 0.0;
        if (!this->BSIM3v32ldwbGiven)
            this->BSIM3v32ldwb = 0.0;
        if (!this->BSIM3v32lb0Given)
            this->BSIM3v32lb0 = 0.0;
        if (!this->BSIM3v32lb1Given)
            this->BSIM3v32lb1 = 0.0;
        if (!this->BSIM3v32lalpha0Given)
            this->BSIM3v32lalpha0 = 0.0;
        if (!this->BSIM3v32lalpha1Given)
            this->BSIM3v32lalpha1 = 0.0;
        if (!this->BSIM3v32lbeta0Given)
            this->BSIM3v32lbeta0 = 0.0;
        if (!this->BSIM3v32lvfbGiven)
            this->BSIM3v32lvfb = 0.0;

        if (!this->BSIM3v32lelmGiven)
            this->BSIM3v32lelm = 0.0;
        if (!this->BSIM3v32lcgslGiven)
            this->BSIM3v32lcgsl = 0.0;
        if (!this->BSIM3v32lcgdlGiven)
            this->BSIM3v32lcgdl = 0.0;
        if (!this->BSIM3v32lckappaGiven)
            this->BSIM3v32lckappa = 0.0;
        if (!this->BSIM3v32lclcGiven)
            this->BSIM3v32lclc = 0.0;
        if (!this->BSIM3v32lcleGiven)
            this->BSIM3v32lcle = 0.0;
        if (!this->BSIM3v32lcfGiven)
            this->BSIM3v32lcf = 0.0;
        if (!this->BSIM3v32lvfbcvGiven)
            this->BSIM3v32lvfbcv = 0.0;
        if (!this->BSIM3v32lacdeGiven)
            this->BSIM3v32lacde = 0.0;
        if (!this->BSIM3v32lmoinGiven)
            this->BSIM3v32lmoin = 0.0;
        if (!this->BSIM3v32lnoffGiven)
            this->BSIM3v32lnoff = 0.0;
        if (!this->BSIM3v32lvoffcvGiven)
            this->BSIM3v32lvoffcv = 0.0;

        /* Width dependence */
        if (!this->BSIM3v32wcdscGiven)
            this->BSIM3v32wcdsc = 0.0;
        if (!this->BSIM3v32wcdscbGiven)
            this->BSIM3v32wcdscb = 0.0;
        if (!this->BSIM3v32wcdscdGiven)
            this->BSIM3v32wcdscd = 0.0;
        if (!this->BSIM3v32wcitGiven)
            this->BSIM3v32wcit = 0.0;
        if (!this->BSIM3v32wnfactorGiven)
            this->BSIM3v32wnfactor = 0.0;
        if (!this->BSIM3v32wxjGiven)
            this->BSIM3v32wxj = 0.0;
        if (!this->BSIM3v32wvsatGiven)
            this->BSIM3v32wvsat = 0.0;
        if (!this->BSIM3v32watGiven)
            this->BSIM3v32wat = 0.0;
        if (!this->BSIM3v32wa0Given)
            this->BSIM3v32wa0 = 0.0;
        if (!this->BSIM3v32wagsGiven)
            this->BSIM3v32wags = 0.0;
        if (!this->BSIM3v32wa1Given)
            this->BSIM3v32wa1 = 0.0;
        if (!this->BSIM3v32wa2Given)
            this->BSIM3v32wa2 = 0.0;
        if (!this->BSIM3v32wketaGiven)
            this->BSIM3v32wketa = 0.0;
        if (!this->BSIM3v32wnsubGiven)
            this->BSIM3v32wnsub = 0.0;
        if (!this->BSIM3v32wnpeakGiven)
            this->BSIM3v32wnpeak = 0.0;
        if (!this->BSIM3v32wngateGiven)
            this->BSIM3v32wngate = 0.0;
        if (!this->BSIM3v32wvbmGiven)
            this->BSIM3v32wvbm = 0.0;
        if (!this->BSIM3v32wxtGiven)
            this->BSIM3v32wxt = 0.0;
        if (!this->BSIM3v32wkt1Given)
            this->BSIM3v32wkt1 = 0.0;
        if (!this->BSIM3v32wkt1lGiven)
            this->BSIM3v32wkt1l = 0.0;
        if (!this->BSIM3v32wkt2Given)
            this->BSIM3v32wkt2 = 0.0;
        if (!this->BSIM3v32wk3Given)
            this->BSIM3v32wk3 = 0.0;
        if (!this->BSIM3v32wk3bGiven)
            this->BSIM3v32wk3b = 0.0;
        if (!this->BSIM3v32ww0Given)
            this->BSIM3v32ww0 = 0.0;
        if (!this->BSIM3v32wnlxGiven)
            this->BSIM3v32wnlx = 0.0;
        if (!this->BSIM3v32wdvt0Given)
            this->BSIM3v32wdvt0 = 0.0;
        if (!this->BSIM3v32wdvt1Given)
            this->BSIM3v32wdvt1 = 0.0;
        if (!this->BSIM3v32wdvt2Given)
            this->BSIM3v32wdvt2 = 0.0;
        if (!this->BSIM3v32wdvt0wGiven)
            this->BSIM3v32wdvt0w = 0.0;
        if (!this->BSIM3v32wdvt1wGiven)
            this->BSIM3v32wdvt1w = 0.0;
        if (!this->BSIM3v32wdvt2wGiven)
            this->BSIM3v32wdvt2w = 0.0;
        if (!this->BSIM3v32wdroutGiven)
            this->BSIM3v32wdrout = 0.0;
        if (!this->BSIM3v32wdsubGiven)
            this->BSIM3v32wdsub = 0.0;
        if (!this->BSIM3v32wvth0Given)
            this->BSIM3v32wvth0 = 0.0;
        if (!this->BSIM3v32wuaGiven)
            this->BSIM3v32wua = 0.0;
        if (!this->BSIM3v32wua1Given)
            this->BSIM3v32wua1 = 0.0;
        if (!this->BSIM3v32wubGiven)
            this->BSIM3v32wub = 0.0;
        if (!this->BSIM3v32wub1Given)
            this->BSIM3v32wub1 = 0.0;
        if (!this->BSIM3v32wucGiven)
            this->BSIM3v32wuc = 0.0;
        if (!this->BSIM3v32wuc1Given)
            this->BSIM3v32wuc1 = 0.0;
        if (!this->BSIM3v32wu0Given)
            this->BSIM3v32wu0 = 0.0;
        if (!this->BSIM3v32wuteGiven)
            this->BSIM3v32wute = 0.0;
        if (!this->BSIM3v32wvoffGiven)
            this->BSIM3v32wvoff = 0.0;
        if (!this->BSIM3v32wdeltaGiven)
            this->BSIM3v32wdelta = 0.0;
        if (!this->BSIM3v32wrdswGiven)
            this->BSIM3v32wrdsw = 0.0;
        if (!this->BSIM3v32wprwbGiven)
            this->BSIM3v32wprwb = 0.0;
        if (!this->BSIM3v32wprwgGiven)
            this->BSIM3v32wprwg = 0.0;
        if (!this->BSIM3v32wprtGiven)
            this->BSIM3v32wprt = 0.0;
        if (!this->BSIM3v32weta0Given)
            this->BSIM3v32weta0 = 0.0;
        if (!this->BSIM3v32wetabGiven)
            this->BSIM3v32wetab = 0.0;
        if (!this->BSIM3v32wpclmGiven)
            this->BSIM3v32wpclm = 0.0;
        if (!this->BSIM3v32wpdibl1Given)
            this->BSIM3v32wpdibl1 = 0.0;
        if (!this->BSIM3v32wpdibl2Given)
            this->BSIM3v32wpdibl2 = 0.0;
        if (!this->BSIM3v32wpdiblbGiven)
            this->BSIM3v32wpdiblb = 0.0;
        if (!this->BSIM3v32wpscbe1Given)
            this->BSIM3v32wpscbe1 = 0.0;
        if (!this->BSIM3v32wpscbe2Given)
            this->BSIM3v32wpscbe2 = 0.0;
        if (!this->BSIM3v32wpvagGiven)
            this->BSIM3v32wpvag = 0.0;
        if (!this->BSIM3v32wwrGiven)
            this->BSIM3v32wwr = 0.0;
        if (!this->BSIM3v32wdwgGiven)
            this->BSIM3v32wdwg = 0.0;
        if (!this->BSIM3v32wdwbGiven)
            this->BSIM3v32wdwb = 0.0;
        if (!this->BSIM3v32wb0Given)
            this->BSIM3v32wb0 = 0.0;
        if (!this->BSIM3v32wb1Given)
            this->BSIM3v32wb1 = 0.0;
        if (!this->BSIM3v32walpha0Given)
            this->BSIM3v32walpha0 = 0.0;
        if (!this->BSIM3v32walpha1Given)
            this->BSIM3v32walpha1 = 0.0;
        if (!this->BSIM3v32wbeta0Given)
            this->BSIM3v32wbeta0 = 0.0;
        if (!this->BSIM3v32wvfbGiven)
            this->BSIM3v32wvfb = 0.0;

        if (!this->BSIM3v32welmGiven)
            this->BSIM3v32welm = 0.0;
        if (!this->BSIM3v32wcgslGiven)
            this->BSIM3v32wcgsl = 0.0;
        if (!this->BSIM3v32wcgdlGiven)
            this->BSIM3v32wcgdl = 0.0;
        if (!this->BSIM3v32wckappaGiven)
            this->BSIM3v32wckappa = 0.0;
        if (!this->BSIM3v32wcfGiven)
            this->BSIM3v32wcf = 0.0;
        if (!this->BSIM3v32wclcGiven)
            this->BSIM3v32wclc = 0.0;
        if (!this->BSIM3v32wcleGiven)
            this->BSIM3v32wcle = 0.0;
        if (!this->BSIM3v32wvfbcvGiven)
            this->BSIM3v32wvfbcv = 0.0;
        if (!this->BSIM3v32wacdeGiven)
            this->BSIM3v32wacde = 0.0;
        if (!this->BSIM3v32wmoinGiven)
            this->BSIM3v32wmoin = 0.0;
        if (!this->BSIM3v32wnoffGiven)
            this->BSIM3v32wnoff = 0.0;
        if (!this->BSIM3v32wvoffcvGiven)
            this->BSIM3v32wvoffcv = 0.0;

        /* Cross-term dependence */
        if (!this->BSIM3v32pcdscGiven)
            this->BSIM3v32pcdsc = 0.0;
        if (!this->BSIM3v32pcdscbGiven)
            this->BSIM3v32pcdscb = 0.0;
        if (!this->BSIM3v32pcdscdGiven)
            this->BSIM3v32pcdscd = 0.0;
        if (!this->BSIM3v32pcitGiven)
            this->BSIM3v32pcit = 0.0;
        if (!this->BSIM3v32pnfactorGiven)
            this->BSIM3v32pnfactor = 0.0;
        if (!this->BSIM3v32pxjGiven)
            this->BSIM3v32pxj = 0.0;
        if (!this->BSIM3v32pvsatGiven)
            this->BSIM3v32pvsat = 0.0;
        if (!this->BSIM3v32patGiven)
            this->BSIM3v32pat = 0.0;
        if (!this->BSIM3v32pa0Given)
            this->BSIM3v32pa0 = 0.0;

        if (!this->BSIM3v32pagsGiven)
            this->BSIM3v32pags = 0.0;
        if (!this->BSIM3v32pa1Given)
            this->BSIM3v32pa1 = 0.0;
        if (!this->BSIM3v32pa2Given)
            this->BSIM3v32pa2 = 0.0;
        if (!this->BSIM3v32pketaGiven)
            this->BSIM3v32pketa = 0.0;
        if (!this->BSIM3v32pnsubGiven)
            this->BSIM3v32pnsub = 0.0;
        if (!this->BSIM3v32pnpeakGiven)
            this->BSIM3v32pnpeak = 0.0;
        if (!this->BSIM3v32pngateGiven)
            this->BSIM3v32pngate = 0.0;
        if (!this->BSIM3v32pvbmGiven)
            this->BSIM3v32pvbm = 0.0;
        if (!this->BSIM3v32pxtGiven)
            this->BSIM3v32pxt = 0.0;
        if (!this->BSIM3v32pkt1Given)
            this->BSIM3v32pkt1 = 0.0;
        if (!this->BSIM3v32pkt1lGiven)
            this->BSIM3v32pkt1l = 0.0;
        if (!this->BSIM3v32pkt2Given)
            this->BSIM3v32pkt2 = 0.0;
        if (!this->BSIM3v32pk3Given)
            this->BSIM3v32pk3 = 0.0;
        if (!this->BSIM3v32pk3bGiven)
            this->BSIM3v32pk3b = 0.0;
        if (!this->BSIM3v32pw0Given)
            this->BSIM3v32pw0 = 0.0;
        if (!this->BSIM3v32pnlxGiven)
            this->BSIM3v32pnlx = 0.0;
        if (!this->BSIM3v32pdvt0Given)
            this->BSIM3v32pdvt0 = 0.0;
        if (!this->BSIM3v32pdvt1Given)
            this->BSIM3v32pdvt1 = 0.0;
        if (!this->BSIM3v32pdvt2Given)
            this->BSIM3v32pdvt2 = 0.0;
        if (!this->BSIM3v32pdvt0wGiven)
            this->BSIM3v32pdvt0w = 0.0;
        if (!this->BSIM3v32pdvt1wGiven)
            this->BSIM3v32pdvt1w = 0.0;
        if (!this->BSIM3v32pdvt2wGiven)
            this->BSIM3v32pdvt2w = 0.0;
        if (!this->BSIM3v32pdroutGiven)
            this->BSIM3v32pdrout = 0.0;
        if (!this->BSIM3v32pdsubGiven)
            this->BSIM3v32pdsub = 0.0;
        if (!this->BSIM3v32pvth0Given)
            this->BSIM3v32pvth0 = 0.0;
        if (!this->BSIM3v32puaGiven)
            this->BSIM3v32pua = 0.0;
        if (!this->BSIM3v32pua1Given)
            this->BSIM3v32pua1 = 0.0;
        if (!this->BSIM3v32pubGiven)
            this->BSIM3v32pub = 0.0;
        if (!this->BSIM3v32pub1Given)
            this->BSIM3v32pub1 = 0.0;
        if (!this->BSIM3v32pucGiven)
            this->BSIM3v32puc = 0.0;
        if (!this->BSIM3v32puc1Given)
            this->BSIM3v32puc1 = 0.0;
        if (!this->BSIM3v32pu0Given)
            this->BSIM3v32pu0 = 0.0;
        if (!this->BSIM3v32puteGiven)
            this->BSIM3v32pute = 0.0;
        if (!this->BSIM3v32pvoffGiven)
            this->BSIM3v32pvoff = 0.0;
        if (!this->BSIM3v32pdeltaGiven)
            this->BSIM3v32pdelta = 0.0;
        if (!this->BSIM3v32prdswGiven)
            this->BSIM3v32prdsw = 0.0;
        if (!this->BSIM3v32pprwbGiven)
            this->BSIM3v32pprwb = 0.0;
        if (!this->BSIM3v32pprwgGiven)
            this->BSIM3v32pprwg = 0.0;
        if (!this->BSIM3v32pprtGiven)
            this->BSIM3v32pprt = 0.0;
        if (!this->BSIM3v32peta0Given)
            this->BSIM3v32peta0 = 0.0;
        if (!this->BSIM3v32petabGiven)
            this->BSIM3v32petab = 0.0;
        if (!this->BSIM3v32ppclmGiven)
            this->BSIM3v32ppclm = 0.0;
        if (!this->BSIM3v32ppdibl1Given)
            this->BSIM3v32ppdibl1 = 0.0;
        if (!this->BSIM3v32ppdibl2Given)
            this->BSIM3v32ppdibl2 = 0.0;
        if (!this->BSIM3v32ppdiblbGiven)
            this->BSIM3v32ppdiblb = 0.0;
        if (!this->BSIM3v32ppscbe1Given)
            this->BSIM3v32ppscbe1 = 0.0;
        if (!this->BSIM3v32ppscbe2Given)
            this->BSIM3v32ppscbe2 = 0.0;
        if (!this->BSIM3v32ppvagGiven)
            this->BSIM3v32ppvag = 0.0;
        if (!this->BSIM3v32pwrGiven)
            this->BSIM3v32pwr = 0.0;
        if (!this->BSIM3v32pdwgGiven)
            this->BSIM3v32pdwg = 0.0;
        if (!this->BSIM3v32pdwbGiven)
            this->BSIM3v32pdwb = 0.0;
        if (!this->BSIM3v32pb0Given)
            this->BSIM3v32pb0 = 0.0;
        if (!this->BSIM3v32pb1Given)
            this->BSIM3v32pb1 = 0.0;
        if (!this->BSIM3v32palpha0Given)
            this->BSIM3v32palpha0 = 0.0;
        if (!this->BSIM3v32palpha1Given)
            this->BSIM3v32palpha1 = 0.0;
        if (!this->BSIM3v32pbeta0Given)
            this->BSIM3v32pbeta0 = 0.0;
        if (!this->BSIM3v32pvfbGiven)
            this->BSIM3v32pvfb = 0.0;

        if (!this->BSIM3v32pelmGiven)
            this->BSIM3v32pelm = 0.0;
        if (!this->BSIM3v32pcgslGiven)
            this->BSIM3v32pcgsl = 0.0;
        if (!this->BSIM3v32pcgdlGiven)
            this->BSIM3v32pcgdl = 0.0;
        if (!this->BSIM3v32pckappaGiven)
            this->BSIM3v32pckappa = 0.0;
        if (!this->BSIM3v32pcfGiven)
            this->BSIM3v32pcf = 0.0;
        if (!this->BSIM3v32pclcGiven)
            this->BSIM3v32pclc = 0.0;
        if (!this->BSIM3v32pcleGiven)
            this->BSIM3v32pcle = 0.0;
        if (!this->BSIM3v32pvfbcvGiven)
            this->BSIM3v32pvfbcv = 0.0;
        if (!this->BSIM3v32pacdeGiven)
            this->BSIM3v32pacde = 0.0;
        if (!this->BSIM3v32pmoinGiven)
            this->BSIM3v32pmoin = 0.0;
        if (!this->BSIM3v32pnoffGiven)
            this->BSIM3v32pnoff = 0.0;
        if (!this->BSIM3v32pvoffcvGiven)
            this->BSIM3v32pvoffcv = 0.0;

        /* unit degree celcius */
        if (!this->BSIM3v32tnomGiven)
            this->BSIM3v32tnom = env->getNormTemp();
        /*        else
            this->BSIM3v32tnom = this->BSIM3v32tnom + 273.15; we make this transform in b3v32mpar.c in the first run */
        if (!this->BSIM3v32LintGiven)
            this->BSIM3v32Lint = 0.0;
        if (!this->BSIM3v32LlGiven)
            this->BSIM3v32Ll = 0.0;
        if (!this->BSIM3v32LlcGiven)
            this->BSIM3v32Llc = this->BSIM3v32Ll;
        if (!this->BSIM3v32LlnGiven)
            this->BSIM3v32Lln = 1.0;
        if (!this->BSIM3v32LwGiven)
            this->BSIM3v32Lw = 0.0;
        if (!this->BSIM3v32LwcGiven)
            this->BSIM3v32Lwc = this->BSIM3v32Lw;
        if (!this->BSIM3v32LwnGiven)
            this->BSIM3v32Lwn = 1.0;
        if (!this->BSIM3v32LwlGiven)
            this->BSIM3v32Lwl = 0.0;
        if (!this->BSIM3v32LwlcGiven)
            this->BSIM3v32Lwlc = this->BSIM3v32Lwl;
        if (!this->BSIM3v32LminGiven)
            this->BSIM3v32Lmin = 0.0;
        if (!this->BSIM3v32LmaxGiven)
            this->BSIM3v32Lmax = 1.0;
        if (!this->BSIM3v32WintGiven)
            this->BSIM3v32Wint = 0.0;
        if (!this->BSIM3v32WlGiven)
            this->BSIM3v32Wl = 0.0;
        if (!this->BSIM3v32WlcGiven)
            this->BSIM3v32Wlc = this->BSIM3v32Wl;
        if (!this->BSIM3v32WlnGiven)
            this->BSIM3v32Wln = 1.0;
        if (!this->BSIM3v32WwGiven)
            this->BSIM3v32Ww = 0.0;
        if (!this->BSIM3v32WwcGiven)
            this->BSIM3v32Wwc = this->BSIM3v32Ww;
        if (!this->BSIM3v32WwnGiven)
            this->BSIM3v32Wwn = 1.0;
        if (!this->BSIM3v32WwlGiven)
            this->BSIM3v32Wwl = 0.0;
        if (!this->BSIM3v32WwlcGiven)
            this->BSIM3v32Wwlc = this->BSIM3v32Wwl;
        if (!this->BSIM3v32WminGiven)
            this->BSIM3v32Wmin = 0.0;
        if (!this->BSIM3v32WmaxGiven)
            this->BSIM3v32Wmax = 1.0;
        if (!this->BSIM3v32dwcGiven)
            this->BSIM3v32dwc = this->BSIM3v32Wint;
        if (!this->BSIM3v32dlcGiven)
            this->BSIM3v32dlc = this->BSIM3v32Lint;

        if (!this->BSIM3v32xlGiven)
            this->BSIM3v32xl = 0.0;
        if (!this->BSIM3v32xwGiven)
            this->BSIM3v32xw = 0.0;

        if (!this->BSIM3v32cfGiven)
            this->BSIM3v32cf = 2.0 * EPSOX / PI * log(1.0 + 0.4e-6 / this->BSIM3v32tox);
        if (!this->BSIM3v32cgdoGiven)
        {
            if (this->BSIM3v32dlcGiven && (this->BSIM3v32dlc > 0.0))
            {
                this->BSIM3v32cgdo = this->BSIM3v32dlc * this->BSIM3v32cox - this->BSIM3v32cgdl;
            }
            else
                this->BSIM3v32cgdo = 0.6 * this->BSIM3v32xj * this->BSIM3v32cox;
        }
        if (!this->BSIM3v32cgsoGiven)
        {
            if (this->BSIM3v32dlcGiven && (this->BSIM3v32dlc > 0.0))
            {
                this->BSIM3v32cgso = this->BSIM3v32dlc * this->BSIM3v32cox - this->BSIM3v32cgsl;
            }
            else
                this->BSIM3v32cgso = 0.6 * this->BSIM3v32xj * this->BSIM3v32cox;
        }

        if (!this->BSIM3v32cgboGiven)
        {
            this->BSIM3v32cgbo = 2.0 * this->BSIM3v32dwc * this->BSIM3v32cox;
        }
        if (!this->BSIM3v32xpartGiven)
            this->BSIM3v32xpart = 0.0;
        if (!this->BSIM3v32sheetResistanceGiven)
            this->BSIM3v32sheetResistance = 0.0;
        if (!this->BSIM3v32unitAreaJctCapGiven)
            this->BSIM3v32unitAreaJctCap = 5.0E-4;
        if (!this->BSIM3v32unitLengthSidewallJctCapGiven)
            this->BSIM3v32unitLengthSidewallJctCap = 5.0E-10;
        if (!this->BSIM3v32unitLengthGateSidewallJctCapGiven)
            this->BSIM3v32unitLengthGateSidewallJctCap = this->BSIM3v32unitLengthSidewallJctCap;
        if (!this->BSIM3v32jctSatCurDensityGiven)
            this->BSIM3v32jctSatCurDensity = 1.0E-4;
        if (!this->BSIM3v32jctSidewallSatCurDensityGiven)
            this->BSIM3v32jctSidewallSatCurDensity = 0.0;
        if (!this->BSIM3v32bulkJctPotentialGiven)
            this->BSIM3v32bulkJctPotential = 1.0;
        if (!this->BSIM3v32sidewallJctPotentialGiven)
            this->BSIM3v32sidewallJctPotential = 1.0;
        if (!this->BSIM3v32GatesidewallJctPotentialGiven)
            this->BSIM3v32GatesidewallJctPotential = this->BSIM3v32sidewallJctPotential;
        if (!this->BSIM3v32bulkJctBotGradingCoeffGiven)
            this->BSIM3v32bulkJctBotGradingCoeff = 0.5;
        if (!this->BSIM3v32bulkJctSideGradingCoeffGiven)
            this->BSIM3v32bulkJctSideGradingCoeff = 0.33;
        if (!this->BSIM3v32bulkJctGateSideGradingCoeffGiven)
            this->BSIM3v32bulkJctGateSideGradingCoeff = this->BSIM3v32bulkJctSideGradingCoeff;
        if (!this->BSIM3v32jctEmissionCoeffGiven)
            this->BSIM3v32jctEmissionCoeff = 1.0;
        if (!this->BSIM3v32jctTempExponentGiven)
            this->BSIM3v32jctTempExponent = 3.0;
        if (!this->BSIM3v32oxideTrapDensityAGiven)
        {
            if (this->BSIM3v32type == BSIM3V32::NMOS)
                this->BSIM3v32oxideTrapDensityA = 1e20;
            else
                this->BSIM3v32oxideTrapDensityA = 9.9e18;
        }
        if (!this->BSIM3v32oxideTrapDensityBGiven)
        {
            if (this->BSIM3v32type == BSIM3V32::NMOS)
                this->BSIM3v32oxideTrapDensityB = 5e4;
            else
                this->BSIM3v32oxideTrapDensityB = 2.4e3;
        }
        if (!this->BSIM3v32oxideTrapDensityCGiven)
        {
            if (this->BSIM3v32type == BSIM3V32::NMOS)
                this->BSIM3v32oxideTrapDensityC = -1.4e-12;
            else
                this->BSIM3v32oxideTrapDensityC = 1.4e-12;
        }
        if (!this->BSIM3v32emGiven)
            this->BSIM3v32em = 4.1e7; /* V/m */
        if (!this->BSIM3v32efGiven)
            this->BSIM3v32ef = 1.0;
        if (!this->BSIM3v32afGiven)
            this->BSIM3v32af = 1.0;
        if (!this->BSIM3v32kfGiven)
            this->BSIM3v32kf = 0.0;

        if (!this->BSIM3v32vgsMaxGiven)
            this->BSIM3v32vgsMax = 1e99;
        if (!this->BSIM3v32vgdMaxGiven)
            this->BSIM3v32vgdMax = 1e99;
        if (!this->BSIM3v32vgbMaxGiven)
            this->BSIM3v32vgbMax = 1e99;
        if (!this->BSIM3v32vdsMaxGiven)
            this->BSIM3v32vdsMax = 1e99;
        if (!this->BSIM3v32vbsMaxGiven)
            this->BSIM3v32vbsMax = 1e99;
        if (!this->BSIM3v32vbdMaxGiven)
            this->BSIM3v32vbdMax = 1e99;
        if (!this->BSIM3v32vgsrMaxGiven)
            this->BSIM3v32vgsrMax = 1e99;
        if (!this->BSIM3v32vgdrMaxGiven)
            this->BSIM3v32vgdrMax = 1e99;
        if (!this->BSIM3v32vgbrMaxGiven)
            this->BSIM3v32vgbrMax = 1e99;
        if (!this->BSIM3v32vbsrMaxGiven)
            this->BSIM3v32vbsrMax = 1e99;
        if (!this->BSIM3v32vbdrMaxGiven)
            this->BSIM3v32vbdrMax = 1e99;

        return 0;
    }

    int BSIM3v32model::setTemp(ModelBase *device, Environment *env)
    {
        struct bsim3v32SizeDependParam *pSizeDependParamKnot, *pLastKnot, *pParam = NULL;
        double tmp, tmp1, tmp2, tmp3, Eg, Eg0, ni, T0, T1, T2, T3, T4, T5, Ldrn, Wdrn;
        double delTemp, Temp, TRatio, Inv_L, Inv_W, Inv_LW, Vtm0, Tnom;
        double Nvtm, SourceSatCurrent, DrainSatCurrent;
        int Size_Not_Found, error;
        BSIM3V32 *dev = dynamic_cast<BSIM3V32 *>(device);

        Temp = env->getTemp();
        if (this->BSIM3v32bulkJctPotential < 0.1)
        {
            this->BSIM3v32bulkJctPotential = 0.1;
            env->logging(Environment::ERR, "Given pb is less than 0.1. Pb is set to 0.1.\n");
        }
        if (this->BSIM3v32sidewallJctPotential < 0.1)
        {
            this->BSIM3v32sidewallJctPotential = 0.1;
            env->logging(Environment::ERR, "Given pbsw is less than 0.1. Pbsw is set to 0.1.\n");
        }
        if (this->BSIM3v32GatesidewallJctPotential < 0.1)
        {
            this->BSIM3v32GatesidewallJctPotential = 0.1;
            env->logging(Environment::ERR, "Given pbswg is less than 0.1. Pbswg is set to 0.1.\n");
        }

        struct bsim3v32SizeDependParam *p = this->pSizeDependParamKnot;
        while (p)
        {
            struct bsim3v32SizeDependParam *next_p = p->pNext;
            delete p;
            p = next_p;
        }
        this->pSizeDependParamKnot = NULL;
        pLastKnot = NULL;

        Tnom = this->BSIM3v32tnom;
        TRatio = Temp / Tnom;

        this->BSIM3v32vcrit = CONSTvt0 * log(CONSTvt0 / (CONSTroot2 * 1.0e-14));
        this->BSIM3v32factor1 = sqrt(EPSSI / EPSOX * this->BSIM3v32tox);

        Vtm0 = KboQ * Tnom;
        Eg0 = 1.16 - 7.02e-4 * Tnom * Tnom / (Tnom + 1108.0);
        ni = 1.45e10 * (Tnom / 300.15) * sqrt(Tnom / 300.15) * exp(21.5565981 - Eg0 / (2.0 * Vtm0));

        this->BSIM3v32vtm = KboQ * Temp;
        Eg = 1.16 - 7.02e-4 * Temp * Temp / (Temp + 1108.0);
        if (Temp != Tnom)
        {
            T0 = Eg0 / Vtm0 - Eg / this->BSIM3v32vtm + this->BSIM3v32jctTempExponent * log(Temp / Tnom);
            T1 = exp(T0 / this->BSIM3v32jctEmissionCoeff);
            this->BSIM3v32jctTempSatCurDensity = this->BSIM3v32jctSatCurDensity * T1;
            this->BSIM3v32jctSidewallTempSatCurDensity = this->BSIM3v32jctSidewallSatCurDensity * T1;
        }
        else
        {
            this->BSIM3v32jctTempSatCurDensity = this->BSIM3v32jctSatCurDensity;
            this->BSIM3v32jctSidewallTempSatCurDensity = this->BSIM3v32jctSidewallSatCurDensity;
        }

        if (this->BSIM3v32jctTempSatCurDensity < 0.0)
            this->BSIM3v32jctTempSatCurDensity = 0.0;
        if (this->BSIM3v32jctSidewallTempSatCurDensity < 0.0)
            this->BSIM3v32jctSidewallTempSatCurDensity = 0.0;

        /* Temperature dependence of D/B and S/B diode capacitance begins */
        delTemp = Temp - this->BSIM3v32tnom;
        T0 = this->BSIM3v32tcj * delTemp;
        if (T0 >= -1.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitAreaTempJctCap =
                    this->BSIM3v32unitAreaJctCap * (1.0 + T0);
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitAreaJctCap *= 1.0 + T0;
            }
        }
        else if (this->BSIM3v32unitAreaJctCap > 0.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitAreaTempJctCap = 0.0;
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitAreaJctCap = 0.0;
            }
            env->logging(Environment::ERR, "Temperature effect has caused cj to be negative. Cj is clamped to zero.\n");
        }
        T0 = this->BSIM3v32tcjsw * delTemp;
        if (T0 >= -1.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitLengthSidewallTempJctCap =
                    this->BSIM3v32unitLengthSidewallJctCap * (1.0 + T0);
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitLengthSidewallJctCap *= 1.0 + T0;
            }
        }
        else if (this->BSIM3v32unitLengthSidewallJctCap > 0.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitLengthSidewallTempJctCap = 0.0;
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitLengthSidewallJctCap = 0.0;
            }
            env->logging(Environment::ERR, "Temperature effect has caused cjsw to be negative. Cjsw is clamped to zero.\n");
        }
        T0 = this->BSIM3v32tcjswg * delTemp;
        if (T0 >= -1.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitLengthGateSidewallTempJctCap =
                    this->BSIM3v32unitLengthGateSidewallJctCap * (1.0 + T0);
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitLengthGateSidewallJctCap *= 1.0 + T0;
            }
        }
        else if (this->BSIM3v32unitLengthGateSidewallJctCap > 0.0)
        {
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
                this->BSIM3v32unitLengthGateSidewallTempJctCap = 0.0;
                break;
            case BSIM3v32V322:
            case BSIM3v32V32:
            default:
                this->BSIM3v32unitLengthGateSidewallJctCap = 0.0;
            }
            env->logging(Environment::ERR, "Temperature effect has caused cjswg to be negative. Cjswg is clamped to zero.\n");
        }

        this->BSIM3v32PhiB = this->BSIM3v32bulkJctPotential - this->BSIM3v32tpb * delTemp;
        if (this->BSIM3v32PhiB < 0.01)
        {
            this->BSIM3v32PhiB = 0.01;
            env->logging(Environment::ERR, "Temperature effect has caused pb to be less than 0.01. Pb is clamped to 0.01.\n");
        }
        this->BSIM3v32PhiBSW = this->BSIM3v32sidewallJctPotential - this->BSIM3v32tpbsw * delTemp;
        if (this->BSIM3v32PhiBSW <= 0.01)
        {
            this->BSIM3v32PhiBSW = 0.01;
            env->logging(Environment::ERR, "Temperature effect has caused pbsw to be less than 0.01. Pbsw is clamped to 0.01.\n");
        }
        this->BSIM3v32PhiBSWG = this->BSIM3v32GatesidewallJctPotential - this->BSIM3v32tpbswg * delTemp;
        if (this->BSIM3v32PhiBSWG <= 0.01)
        {
            this->BSIM3v32PhiBSWG = 0.01;
            env->logging(Environment::ERR, "Temperature effect has caused pbswg to be less than 0.01. Pbswg is clamped to 0.01.\n");
        }
        /* End of junction capacitance */

        /* Device dependent configurations */

        /* MCJ: Length and Width not initialized */

        pSizeDependParamKnot = this->pSizeDependParamKnot;
        Size_Not_Found = 1;
        while ((pSizeDependParamKnot != NULL) && Size_Not_Found)
        {
            if ((dev->BSIM3v32l == pSizeDependParamKnot->Length) && (dev->BSIM3v32w == pSizeDependParamKnot->Width))
            {
                Size_Not_Found = 0;
                dev->pParam = pSizeDependParamKnot;
                if (this->BSIM3v32intVersion > BSIM3v32V322)
                {
                    pParam = dev->pParam; /*bug-fix  */
                }
            }
            else
            {
                pLastKnot = pSizeDependParamKnot;
                pSizeDependParamKnot = pSizeDependParamKnot->pNext;
            }
        }

        if (Size_Not_Found)
        {
            pParam = new bsim3v32SizeDependParam();
            if (pLastKnot == NULL)
                this->pSizeDependParamKnot = pParam;
            else
                pLastKnot->pNext = pParam;
            pParam->pNext = NULL;
            dev->pParam = pParam;

            Ldrn = dev->BSIM3v32l;
            Wdrn = dev->BSIM3v32w;
            pParam->Length = Ldrn;
            pParam->Width = Wdrn;

            T0 = pow(Ldrn, this->BSIM3v32Lln);
            T1 = pow(Wdrn, this->BSIM3v32Lwn);
            tmp1 = this->BSIM3v32Ll / T0 + this->BSIM3v32Lw / T1 + this->BSIM3v32Lwl / (T0 * T1);
            pParam->BSIM3v32dl = this->BSIM3v32Lint + tmp1;
            tmp2 = this->BSIM3v32Llc / T0 + this->BSIM3v32Lwc / T1 + this->BSIM3v32Lwlc / (T0 * T1);
            pParam->BSIM3v32dlc = this->BSIM3v32dlc + tmp2;

            T2 = pow(Ldrn, this->BSIM3v32Wln);
            T3 = pow(Wdrn, this->BSIM3v32Wwn);
            tmp1 = this->BSIM3v32Wl / T2 + this->BSIM3v32Ww / T3 + this->BSIM3v32Wwl / (T2 * T3);
            pParam->BSIM3v32dw = this->BSIM3v32Wint + tmp1;
            tmp2 = this->BSIM3v32Wlc / T2 + this->BSIM3v32Wwc / T3 + this->BSIM3v32Wwlc / (T2 * T3);
            pParam->BSIM3v32dwc = this->BSIM3v32dwc + tmp2;

            pParam->BSIM3v32leff = dev->BSIM3v32l + this->BSIM3v32xl - 2.0 * pParam->BSIM3v32dl;
            if (pParam->BSIM3v32leff <= 0.0)
            {
                env->logging(Environment::FATAL,
                             "BSIM3v32: mosfet %s, model %s: Effective channel length <= 0",
                             dev->name(), this->name());
                return CERR_INVALID_PARAMETER;
            }

            pParam->BSIM3v32weff = dev->BSIM3v32w + this->BSIM3v32xw - 2.0 * pParam->BSIM3v32dw;
            if (pParam->BSIM3v32weff <= 0.0)
            {
                env->logging(Environment::FATAL,
                             "BSIM3v32: mosfet %s, model %s: Effective channel width <= 0",
                             dev->name(), this->name());
                return CERR_INVALID_PARAMETER;
            }

            pParam->BSIM3v32leffCV = dev->BSIM3v32l + this->BSIM3v32xl - 2.0 * pParam->BSIM3v32dlc;
            if (pParam->BSIM3v32leffCV <= 0.0)
            {
                env->logging(Environment::FATAL,
                             "BSIM3v32: mosfet %s, model %s: Effective channel length for C-V <= 0",
                             dev->name(), this->name());
                return CERR_INVALID_PARAMETER;
            }

            pParam->BSIM3v32weffCV = dev->BSIM3v32w + this->BSIM3v32xw - 2.0 * pParam->BSIM3v32dwc;
            if (pParam->BSIM3v32weffCV <= 0.0)
            {
                env->logging(Environment::FATAL,
                             "BSIM3v32: mosfet %s, model %s: Effective channel width for C-V <= 0",
                             dev->name(), this->name());
                return CERR_INVALID_PARAMETER;
            }

            if (this->BSIM3v32binUnit == 1)
            {
                Inv_L = 1.0e-6 / pParam->BSIM3v32leff;
                Inv_W = 1.0e-6 / pParam->BSIM3v32weff;
                Inv_LW = 1.0e-12 / (pParam->BSIM3v32leff * pParam->BSIM3v32weff);
            }
            else
            {
                Inv_L = 1.0 / pParam->BSIM3v32leff;
                Inv_W = 1.0 / pParam->BSIM3v32weff;
                Inv_LW = 1.0 / (pParam->BSIM3v32leff * pParam->BSIM3v32weff);
            }
            pParam->BSIM3v32cdsc = this->BSIM3v32cdsc + this->BSIM3v32lcdsc * Inv_L + this->BSIM3v32wcdsc * Inv_W + this->BSIM3v32pcdsc * Inv_LW;
            pParam->BSIM3v32cdscb = this->BSIM3v32cdscb + this->BSIM3v32lcdscb * Inv_L + this->BSIM3v32wcdscb * Inv_W + this->BSIM3v32pcdscb * Inv_LW;

            pParam->BSIM3v32cdscd = this->BSIM3v32cdscd + this->BSIM3v32lcdscd * Inv_L + this->BSIM3v32wcdscd * Inv_W + this->BSIM3v32pcdscd * Inv_LW;

            pParam->BSIM3v32cit = this->BSIM3v32cit + this->BSIM3v32lcit * Inv_L + this->BSIM3v32wcit * Inv_W + this->BSIM3v32pcit * Inv_LW;
            pParam->BSIM3v32nfactor = this->BSIM3v32nfactor + this->BSIM3v32lnfactor * Inv_L + this->BSIM3v32wnfactor * Inv_W + this->BSIM3v32pnfactor * Inv_LW;
            pParam->BSIM3v32xj = this->BSIM3v32xj + this->BSIM3v32lxj * Inv_L + this->BSIM3v32wxj * Inv_W + this->BSIM3v32pxj * Inv_LW;
            pParam->BSIM3v32vsat = this->BSIM3v32vsat + this->BSIM3v32lvsat * Inv_L + this->BSIM3v32wvsat * Inv_W + this->BSIM3v32pvsat * Inv_LW;
            pParam->BSIM3v32at = this->BSIM3v32at + this->BSIM3v32lat * Inv_L + this->BSIM3v32wat * Inv_W + this->BSIM3v32pat * Inv_LW;
            pParam->BSIM3v32a0 = this->BSIM3v32a0 + this->BSIM3v32la0 * Inv_L + this->BSIM3v32wa0 * Inv_W + this->BSIM3v32pa0 * Inv_LW;

            pParam->BSIM3v32ags = this->BSIM3v32ags + this->BSIM3v32lags * Inv_L + this->BSIM3v32wags * Inv_W + this->BSIM3v32pags * Inv_LW;

            pParam->BSIM3v32a1 = this->BSIM3v32a1 + this->BSIM3v32la1 * Inv_L + this->BSIM3v32wa1 * Inv_W + this->BSIM3v32pa1 * Inv_LW;
            pParam->BSIM3v32a2 = this->BSIM3v32a2 + this->BSIM3v32la2 * Inv_L + this->BSIM3v32wa2 * Inv_W + this->BSIM3v32pa2 * Inv_LW;
            pParam->BSIM3v32keta = this->BSIM3v32keta + this->BSIM3v32lketa * Inv_L + this->BSIM3v32wketa * Inv_W + this->BSIM3v32pketa * Inv_LW;
            pParam->BSIM3v32nsub = this->BSIM3v32nsub + this->BSIM3v32lnsub * Inv_L + this->BSIM3v32wnsub * Inv_W + this->BSIM3v32pnsub * Inv_LW;
            pParam->BSIM3v32npeak = this->BSIM3v32npeak + this->BSIM3v32lnpeak * Inv_L + this->BSIM3v32wnpeak * Inv_W + this->BSIM3v32pnpeak * Inv_LW;
            pParam->BSIM3v32ngate = this->BSIM3v32ngate + this->BSIM3v32lngate * Inv_L + this->BSIM3v32wngate * Inv_W + this->BSIM3v32pngate * Inv_LW;
            pParam->BSIM3v32gamma1 = this->BSIM3v32gamma1 + this->BSIM3v32lgamma1 * Inv_L + this->BSIM3v32wgamma1 * Inv_W + this->BSIM3v32pgamma1 * Inv_LW;
            pParam->BSIM3v32gamma2 = this->BSIM3v32gamma2 + this->BSIM3v32lgamma2 * Inv_L + this->BSIM3v32wgamma2 * Inv_W + this->BSIM3v32pgamma2 * Inv_LW;
            pParam->BSIM3v32vbx = this->BSIM3v32vbx + this->BSIM3v32lvbx * Inv_L + this->BSIM3v32wvbx * Inv_W + this->BSIM3v32pvbx * Inv_LW;
            pParam->BSIM3v32vbm = this->BSIM3v32vbm + this->BSIM3v32lvbm * Inv_L + this->BSIM3v32wvbm * Inv_W + this->BSIM3v32pvbm * Inv_LW;
            pParam->BSIM3v32xt = this->BSIM3v32xt + this->BSIM3v32lxt * Inv_L + this->BSIM3v32wxt * Inv_W + this->BSIM3v32pxt * Inv_LW;
            pParam->BSIM3v32vfb = this->BSIM3v32vfb + this->BSIM3v32lvfb * Inv_L + this->BSIM3v32wvfb * Inv_W + this->BSIM3v32pvfb * Inv_LW;
            pParam->BSIM3v32k1 = this->BSIM3v32k1 + this->BSIM3v32lk1 * Inv_L + this->BSIM3v32wk1 * Inv_W + this->BSIM3v32pk1 * Inv_LW;
            pParam->BSIM3v32kt1 = this->BSIM3v32kt1 + this->BSIM3v32lkt1 * Inv_L + this->BSIM3v32wkt1 * Inv_W + this->BSIM3v32pkt1 * Inv_LW;
            pParam->BSIM3v32kt1l = this->BSIM3v32kt1l + this->BSIM3v32lkt1l * Inv_L + this->BSIM3v32wkt1l * Inv_W + this->BSIM3v32pkt1l * Inv_LW;
            pParam->BSIM3v32k2 = this->BSIM3v32k2 + this->BSIM3v32lk2 * Inv_L + this->BSIM3v32wk2 * Inv_W + this->BSIM3v32pk2 * Inv_LW;
            pParam->BSIM3v32kt2 = this->BSIM3v32kt2 + this->BSIM3v32lkt2 * Inv_L + this->BSIM3v32wkt2 * Inv_W + this->BSIM3v32pkt2 * Inv_LW;
            pParam->BSIM3v32k3 = this->BSIM3v32k3 + this->BSIM3v32lk3 * Inv_L + this->BSIM3v32wk3 * Inv_W + this->BSIM3v32pk3 * Inv_LW;
            pParam->BSIM3v32k3b = this->BSIM3v32k3b + this->BSIM3v32lk3b * Inv_L + this->BSIM3v32wk3b * Inv_W + this->BSIM3v32pk3b * Inv_LW;
            pParam->BSIM3v32w0 = this->BSIM3v32w0 + this->BSIM3v32lw0 * Inv_L + this->BSIM3v32ww0 * Inv_W + this->BSIM3v32pw0 * Inv_LW;
            pParam->BSIM3v32nlx = this->BSIM3v32nlx + this->BSIM3v32lnlx * Inv_L + this->BSIM3v32wnlx * Inv_W + this->BSIM3v32pnlx * Inv_LW;
            pParam->BSIM3v32dvt0 = this->BSIM3v32dvt0 + this->BSIM3v32ldvt0 * Inv_L + this->BSIM3v32wdvt0 * Inv_W + this->BSIM3v32pdvt0 * Inv_LW;
            pParam->BSIM3v32dvt1 = this->BSIM3v32dvt1 + this->BSIM3v32ldvt1 * Inv_L + this->BSIM3v32wdvt1 * Inv_W + this->BSIM3v32pdvt1 * Inv_LW;
            pParam->BSIM3v32dvt2 = this->BSIM3v32dvt2 + this->BSIM3v32ldvt2 * Inv_L + this->BSIM3v32wdvt2 * Inv_W + this->BSIM3v32pdvt2 * Inv_LW;
            pParam->BSIM3v32dvt0w = this->BSIM3v32dvt0w + this->BSIM3v32ldvt0w * Inv_L + this->BSIM3v32wdvt0w * Inv_W + this->BSIM3v32pdvt0w * Inv_LW;
            pParam->BSIM3v32dvt1w = this->BSIM3v32dvt1w + this->BSIM3v32ldvt1w * Inv_L + this->BSIM3v32wdvt1w * Inv_W + this->BSIM3v32pdvt1w * Inv_LW;
            pParam->BSIM3v32dvt2w = this->BSIM3v32dvt2w + this->BSIM3v32ldvt2w * Inv_L + this->BSIM3v32wdvt2w * Inv_W + this->BSIM3v32pdvt2w * Inv_LW;
            pParam->BSIM3v32drout = this->BSIM3v32drout + this->BSIM3v32ldrout * Inv_L + this->BSIM3v32wdrout * Inv_W + this->BSIM3v32pdrout * Inv_LW;
            pParam->BSIM3v32dsub = this->BSIM3v32dsub + this->BSIM3v32ldsub * Inv_L + this->BSIM3v32wdsub * Inv_W + this->BSIM3v32pdsub * Inv_LW;
            pParam->BSIM3v32vth0 = this->BSIM3v32vth0 + this->BSIM3v32lvth0 * Inv_L + this->BSIM3v32wvth0 * Inv_W + this->BSIM3v32pvth0 * Inv_LW;
            pParam->BSIM3v32ua = this->BSIM3v32ua + this->BSIM3v32lua * Inv_L + this->BSIM3v32wua * Inv_W + this->BSIM3v32pua * Inv_LW;
            pParam->BSIM3v32ua1 = this->BSIM3v32ua1 + this->BSIM3v32lua1 * Inv_L + this->BSIM3v32wua1 * Inv_W + this->BSIM3v32pua1 * Inv_LW;
            pParam->BSIM3v32ub = this->BSIM3v32ub + this->BSIM3v32lub * Inv_L + this->BSIM3v32wub * Inv_W + this->BSIM3v32pub * Inv_LW;
            pParam->BSIM3v32ub1 = this->BSIM3v32ub1 + this->BSIM3v32lub1 * Inv_L + this->BSIM3v32wub1 * Inv_W + this->BSIM3v32pub1 * Inv_LW;
            pParam->BSIM3v32uc = this->BSIM3v32uc + this->BSIM3v32luc * Inv_L + this->BSIM3v32wuc * Inv_W + this->BSIM3v32puc * Inv_LW;
            pParam->BSIM3v32uc1 = this->BSIM3v32uc1 + this->BSIM3v32luc1 * Inv_L + this->BSIM3v32wuc1 * Inv_W + this->BSIM3v32puc1 * Inv_LW;
            pParam->BSIM3v32u0 = this->BSIM3v32u0 + this->BSIM3v32lu0 * Inv_L + this->BSIM3v32wu0 * Inv_W + this->BSIM3v32pu0 * Inv_LW;
            pParam->BSIM3v32ute = this->BSIM3v32ute + this->BSIM3v32lute * Inv_L + this->BSIM3v32wute * Inv_W + this->BSIM3v32pute * Inv_LW;
            pParam->BSIM3v32voff = this->BSIM3v32voff + this->BSIM3v32lvoff * Inv_L + this->BSIM3v32wvoff * Inv_W + this->BSIM3v32pvoff * Inv_LW;
            pParam->BSIM3v32delta = this->BSIM3v32delta + this->BSIM3v32ldelta * Inv_L + this->BSIM3v32wdelta * Inv_W + this->BSIM3v32pdelta * Inv_LW;
            pParam->BSIM3v32rdsw = this->BSIM3v32rdsw + this->BSIM3v32lrdsw * Inv_L + this->BSIM3v32wrdsw * Inv_W + this->BSIM3v32prdsw * Inv_LW;
            pParam->BSIM3v32prwg = this->BSIM3v32prwg + this->BSIM3v32lprwg * Inv_L + this->BSIM3v32wprwg * Inv_W + this->BSIM3v32pprwg * Inv_LW;
            pParam->BSIM3v32prwb = this->BSIM3v32prwb + this->BSIM3v32lprwb * Inv_L + this->BSIM3v32wprwb * Inv_W + this->BSIM3v32pprwb * Inv_LW;
            pParam->BSIM3v32prt = this->BSIM3v32prt + this->BSIM3v32lprt * Inv_L + this->BSIM3v32wprt * Inv_W + this->BSIM3v32pprt * Inv_LW;
            pParam->BSIM3v32eta0 = this->BSIM3v32eta0 + this->BSIM3v32leta0 * Inv_L + this->BSIM3v32weta0 * Inv_W + this->BSIM3v32peta0 * Inv_LW;
            pParam->BSIM3v32etab = this->BSIM3v32etab + this->BSIM3v32letab * Inv_L + this->BSIM3v32wetab * Inv_W + this->BSIM3v32petab * Inv_LW;
            pParam->BSIM3v32pclm = this->BSIM3v32pclm + this->BSIM3v32lpclm * Inv_L + this->BSIM3v32wpclm * Inv_W + this->BSIM3v32ppclm * Inv_LW;
            pParam->BSIM3v32pdibl1 = this->BSIM3v32pdibl1 + this->BSIM3v32lpdibl1 * Inv_L + this->BSIM3v32wpdibl1 * Inv_W + this->BSIM3v32ppdibl1 * Inv_LW;
            pParam->BSIM3v32pdibl2 = this->BSIM3v32pdibl2 + this->BSIM3v32lpdibl2 * Inv_L + this->BSIM3v32wpdibl2 * Inv_W + this->BSIM3v32ppdibl2 * Inv_LW;
            pParam->BSIM3v32pdiblb = this->BSIM3v32pdiblb + this->BSIM3v32lpdiblb * Inv_L + this->BSIM3v32wpdiblb * Inv_W + this->BSIM3v32ppdiblb * Inv_LW;
            pParam->BSIM3v32pscbe1 = this->BSIM3v32pscbe1 + this->BSIM3v32lpscbe1 * Inv_L + this->BSIM3v32wpscbe1 * Inv_W + this->BSIM3v32ppscbe1 * Inv_LW;
            pParam->BSIM3v32pscbe2 = this->BSIM3v32pscbe2 + this->BSIM3v32lpscbe2 * Inv_L + this->BSIM3v32wpscbe2 * Inv_W + this->BSIM3v32ppscbe2 * Inv_LW;
            pParam->BSIM3v32pvag = this->BSIM3v32pvag + this->BSIM3v32lpvag * Inv_L + this->BSIM3v32wpvag * Inv_W + this->BSIM3v32ppvag * Inv_LW;
            pParam->BSIM3v32wr = this->BSIM3v32wr + this->BSIM3v32lwr * Inv_L + this->BSIM3v32wwr * Inv_W + this->BSIM3v32pwr * Inv_LW;
            pParam->BSIM3v32dwg = this->BSIM3v32dwg + this->BSIM3v32ldwg * Inv_L + this->BSIM3v32wdwg * Inv_W + this->BSIM3v32pdwg * Inv_LW;
            pParam->BSIM3v32dwb = this->BSIM3v32dwb + this->BSIM3v32ldwb * Inv_L + this->BSIM3v32wdwb * Inv_W + this->BSIM3v32pdwb * Inv_LW;
            pParam->BSIM3v32b0 = this->BSIM3v32b0 + this->BSIM3v32lb0 * Inv_L + this->BSIM3v32wb0 * Inv_W + this->BSIM3v32pb0 * Inv_LW;
            pParam->BSIM3v32b1 = this->BSIM3v32b1 + this->BSIM3v32lb1 * Inv_L + this->BSIM3v32wb1 * Inv_W + this->BSIM3v32pb1 * Inv_LW;
            pParam->BSIM3v32alpha0 = this->BSIM3v32alpha0 + this->BSIM3v32lalpha0 * Inv_L + this->BSIM3v32walpha0 * Inv_W + this->BSIM3v32palpha0 * Inv_LW;
            pParam->BSIM3v32alpha1 = this->BSIM3v32alpha1 + this->BSIM3v32lalpha1 * Inv_L + this->BSIM3v32walpha1 * Inv_W + this->BSIM3v32palpha1 * Inv_LW;
            pParam->BSIM3v32beta0 = this->BSIM3v32beta0 + this->BSIM3v32lbeta0 * Inv_L + this->BSIM3v32wbeta0 * Inv_W + this->BSIM3v32pbeta0 * Inv_LW;
            /* CV model */
            pParam->BSIM3v32elm = this->BSIM3v32elm + this->BSIM3v32lelm * Inv_L + this->BSIM3v32welm * Inv_W + this->BSIM3v32pelm * Inv_LW;
            pParam->BSIM3v32cgsl = this->BSIM3v32cgsl + this->BSIM3v32lcgsl * Inv_L + this->BSIM3v32wcgsl * Inv_W + this->BSIM3v32pcgsl * Inv_LW;
            pParam->BSIM3v32cgdl = this->BSIM3v32cgdl + this->BSIM3v32lcgdl * Inv_L + this->BSIM3v32wcgdl * Inv_W + this->BSIM3v32pcgdl * Inv_LW;
            pParam->BSIM3v32ckappa = this->BSIM3v32ckappa + this->BSIM3v32lckappa * Inv_L + this->BSIM3v32wckappa * Inv_W + this->BSIM3v32pckappa * Inv_LW;
            pParam->BSIM3v32cf = this->BSIM3v32cf + this->BSIM3v32lcf * Inv_L + this->BSIM3v32wcf * Inv_W + this->BSIM3v32pcf * Inv_LW;
            pParam->BSIM3v32clc = this->BSIM3v32clc + this->BSIM3v32lclc * Inv_L + this->BSIM3v32wclc * Inv_W + this->BSIM3v32pclc * Inv_LW;
            pParam->BSIM3v32cle = this->BSIM3v32cle + this->BSIM3v32lcle * Inv_L + this->BSIM3v32wcle * Inv_W + this->BSIM3v32pcle * Inv_LW;
            pParam->BSIM3v32vfbcv = this->BSIM3v32vfbcv + this->BSIM3v32lvfbcv * Inv_L + this->BSIM3v32wvfbcv * Inv_W + this->BSIM3v32pvfbcv * Inv_LW;
            pParam->BSIM3v32acde = this->BSIM3v32acde + this->BSIM3v32lacde * Inv_L + this->BSIM3v32wacde * Inv_W + this->BSIM3v32pacde * Inv_LW;
            pParam->BSIM3v32moin = this->BSIM3v32moin + this->BSIM3v32lmoin * Inv_L + this->BSIM3v32wmoin * Inv_W + this->BSIM3v32pmoin * Inv_LW;
            pParam->BSIM3v32noff = this->BSIM3v32noff + this->BSIM3v32lnoff * Inv_L + this->BSIM3v32wnoff * Inv_W + this->BSIM3v32pnoff * Inv_LW;
            pParam->BSIM3v32voffcv = this->BSIM3v32voffcv + this->BSIM3v32lvoffcv * Inv_L + this->BSIM3v32wvoffcv * Inv_W + this->BSIM3v32pvoffcv * Inv_LW;

            pParam->BSIM3v32abulkCVfactor = 1.0 + pow((pParam->BSIM3v32clc / pParam->BSIM3v32leffCV),
                                                      pParam->BSIM3v32cle);

            T0 = (TRatio - 1.0);
            pParam->BSIM3v32ua = pParam->BSIM3v32ua + pParam->BSIM3v32ua1 * T0;
            pParam->BSIM3v32ub = pParam->BSIM3v32ub + pParam->BSIM3v32ub1 * T0;
            pParam->BSIM3v32uc = pParam->BSIM3v32uc + pParam->BSIM3v32uc1 * T0;
            if (pParam->BSIM3v32u0 > 1.0)
                pParam->BSIM3v32u0 = pParam->BSIM3v32u0 / 1.0e4;

            pParam->BSIM3v32u0temp = pParam->BSIM3v32u0 * pow(TRatio, pParam->BSIM3v32ute);
            pParam->BSIM3v32vsattemp = pParam->BSIM3v32vsat - pParam->BSIM3v32at * T0;
            pParam->BSIM3v32rds0 = (pParam->BSIM3v32rdsw + pParam->BSIM3v32prt * T0) / pow(pParam->BSIM3v32weff * 1E6, pParam->BSIM3v32wr);

            if (check(dev, env))
            {
                env->logging(Environment::FATAL, "Fatal error(s) detected during BSIM3v32V3.2 parameter checking for %s in model %s", dev->name(), this->name());
                return CERR_INVALID_PARAMETER;
            }

            pParam->BSIM3v32cgdo = (this->BSIM3v32cgdo + pParam->BSIM3v32cf) * pParam->BSIM3v32weffCV;
            pParam->BSIM3v32cgso = (this->BSIM3v32cgso + pParam->BSIM3v32cf) * pParam->BSIM3v32weffCV;
            pParam->BSIM3v32cgbo = this->BSIM3v32cgbo * pParam->BSIM3v32leffCV;

            T0 = pParam->BSIM3v32leffCV * pParam->BSIM3v32leffCV;
            pParam->BSIM3v32tconst = pParam->BSIM3v32u0temp * pParam->BSIM3v32elm / (this->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV * T0);

            if (!this->BSIM3v32npeakGiven && this->BSIM3v32gamma1Given)
            {
                T0 = pParam->BSIM3v32gamma1 * this->BSIM3v32cox;
                pParam->BSIM3v32npeak = 3.021E22 * T0 * T0;
            }

            pParam->BSIM3v32phi = 2.0 * Vtm0 * log(pParam->BSIM3v32npeak / ni);

            pParam->BSIM3v32sqrtPhi = sqrt(pParam->BSIM3v32phi);
            pParam->BSIM3v32phis3 = pParam->BSIM3v32sqrtPhi * pParam->BSIM3v32phi;

            pParam->BSIM3v32Xdep0 = sqrt(2.0 * EPSSI / (Charge_q * pParam->BSIM3v32npeak * 1.0e6)) * pParam->BSIM3v32sqrtPhi;
            pParam->BSIM3v32sqrtXdep0 = sqrt(pParam->BSIM3v32Xdep0);
            pParam->BSIM3v32litl = sqrt(3.0 * pParam->BSIM3v32xj * this->BSIM3v32tox);
            pParam->BSIM3v32vbi = Vtm0 * log(1.0e20 * pParam->BSIM3v32npeak / (ni * ni));
            pParam->BSIM3v32cdep0 = sqrt(Charge_q * EPSSI * pParam->BSIM3v32npeak * 1.0e6 / 2.0 / pParam->BSIM3v32phi);

            pParam->BSIM3v32ldeb = sqrt(EPSSI * Vtm0 / (Charge_q * pParam->BSIM3v32npeak * 1.0e6)) / 3.0;
            pParam->BSIM3v32acde *= pow((pParam->BSIM3v32npeak / 2.0e16), -0.25);

            if (this->BSIM3v32k1Given || this->BSIM3v32k2Given)
            {
                if (!this->BSIM3v32k1Given)
                {
#if 0
                    if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9)) /* don't print in sensitivity */
#endif
                    env->logging(Environment::WARN, "Warning: k1 should be specified with k2.\n");
                    pParam->BSIM3v32k1 = 0.53;
                }
                if (!this->BSIM3v32k2Given)
                {
#if 0
                    if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9)) /* don't print in sensitivity */
#endif
                    env->logging(Environment::WARN, "Warning: k2 should be specified with k1.\n");
                    pParam->BSIM3v32k2 = -0.0186;
                }
#if 0
                if ((!ckt->CKTcurJob) || (ckt->CKTcurJob->JOBtype < 9))
#endif
                { /* don't print in sensitivity */
                    if (this->BSIM3v32nsubGiven)
                        env->logging(Environment::WARN, "Warning: nsub is ignored because k1 or k2 is given.\n");
                    if (this->BSIM3v32xtGiven)
                        env->logging(Environment::WARN, "Warning: xt is ignored because k1 or k2 is given.\n");
                    if (this->BSIM3v32vbxGiven)
                        env->logging(Environment::WARN, "Warning: vbx is ignored because k1 or k2 is given.\n");
                    if (this->BSIM3v32gamma1Given)
                        env->logging(Environment::WARN, "Warning: gamma1 is ignored because k1 or k2 is given.\n");
                    if (this->BSIM3v32gamma2Given)
                        env->logging(Environment::WARN, "Warning: gamma2 is ignored because k1 or k2 is given.\n");
                }
            }
            else
            {
                if (!this->BSIM3v32vbxGiven)
                    pParam->BSIM3v32vbx = pParam->BSIM3v32phi - 7.7348e-4 * pParam->BSIM3v32npeak * pParam->BSIM3v32xt * pParam->BSIM3v32xt;
                if (pParam->BSIM3v32vbx > 0.0)
                    pParam->BSIM3v32vbx = -pParam->BSIM3v32vbx;
                if (pParam->BSIM3v32vbm > 0.0)
                    pParam->BSIM3v32vbm = -pParam->BSIM3v32vbm;

                if (!this->BSIM3v32gamma1Given)
                    pParam->BSIM3v32gamma1 = 5.753e-12 * sqrt(pParam->BSIM3v32npeak) / this->BSIM3v32cox;
                if (!this->BSIM3v32gamma2Given)
                    pParam->BSIM3v32gamma2 = 5.753e-12 * sqrt(pParam->BSIM3v32nsub) / this->BSIM3v32cox;

                T0 = pParam->BSIM3v32gamma1 - pParam->BSIM3v32gamma2;
                T1 = sqrt(pParam->BSIM3v32phi - pParam->BSIM3v32vbx) - pParam->BSIM3v32sqrtPhi;
                T2 = sqrt(pParam->BSIM3v32phi * (pParam->BSIM3v32phi - pParam->BSIM3v32vbm)) - pParam->BSIM3v32phi;
                pParam->BSIM3v32k2 = T0 * T1 / (2.0 * T2 + pParam->BSIM3v32vbm);
                pParam->BSIM3v32k1 = pParam->BSIM3v32gamma2 - 2.0 * pParam->BSIM3v32k2 * sqrt(pParam->BSIM3v32phi - pParam->BSIM3v32vbm);
            }

            if (pParam->BSIM3v32k2 < 0.0)
            {
                T0 = 0.5 * pParam->BSIM3v32k1 / pParam->BSIM3v32k2;
                pParam->BSIM3v32vbsc = 0.9 * (pParam->BSIM3v32phi - T0 * T0);
                if (pParam->BSIM3v32vbsc > -3.0)
                    pParam->BSIM3v32vbsc = -3.0;
                else if (pParam->BSIM3v32vbsc < -30.0)
                    pParam->BSIM3v32vbsc = -30.0;
            }
            else
            {
                pParam->BSIM3v32vbsc = -30.0;
            }
            if (pParam->BSIM3v32vbsc > pParam->BSIM3v32vbm)
                pParam->BSIM3v32vbsc = pParam->BSIM3v32vbm;

            if (!this->BSIM3v32vfbGiven)
            {
                if (this->BSIM3v32vth0Given)
                {
                    pParam->BSIM3v32vfb = this->BSIM3v32type * pParam->BSIM3v32vth0 - pParam->BSIM3v32phi - pParam->BSIM3v32k1 * pParam->BSIM3v32sqrtPhi;
                }
                else
                {
                    pParam->BSIM3v32vfb = -1.0;
                }
            }
            if (!this->BSIM3v32vth0Given)
            {
                pParam->BSIM3v32vth0 = this->BSIM3v32type * (pParam->BSIM3v32vfb + pParam->BSIM3v32phi + pParam->BSIM3v32k1 * pParam->BSIM3v32sqrtPhi);
            }

            pParam->BSIM3v32k1ox = pParam->BSIM3v32k1 * this->BSIM3v32tox / this->BSIM3v32toxm;
            pParam->BSIM3v32k2ox = pParam->BSIM3v32k2 * this->BSIM3v32tox / this->BSIM3v32toxm;

            T1 = sqrt(EPSSI / EPSOX * this->BSIM3v32tox * pParam->BSIM3v32Xdep0);
            T0 = exp(-0.5 * pParam->BSIM3v32dsub * pParam->BSIM3v32leff / T1);
            pParam->BSIM3v32theta0vb0 = (T0 + 2.0 * T0 * T0);

            T0 = exp(-0.5 * pParam->BSIM3v32drout * pParam->BSIM3v32leff / T1);
            T2 = (T0 + 2.0 * T0 * T0);
            pParam->BSIM3v32thetaRout = pParam->BSIM3v32pdibl1 * T2 + pParam->BSIM3v32pdibl2;

            tmp = sqrt(pParam->BSIM3v32Xdep0);
            tmp1 = pParam->BSIM3v32vbi - pParam->BSIM3v32phi;
            tmp2 = this->BSIM3v32factor1 * tmp;

            T0 = -0.5 * pParam->BSIM3v32dvt1w * pParam->BSIM3v32weff * pParam->BSIM3v32leff / tmp2;
            if (T0 > -EXP_THRESHOLD)
            {
                T1 = exp(T0);
                T2 = T1 * (1.0 + 2.0 * T1);
            }
            else
            {
                T1 = MIN_EXP;
                T2 = T1 * (1.0 + 2.0 * T1);
            }
            T0 = pParam->BSIM3v32dvt0w * T2;
            T2 = T0 * tmp1;

            T0 = -0.5 * pParam->BSIM3v32dvt1 * pParam->BSIM3v32leff / tmp2;
            if (T0 > -EXP_THRESHOLD)
            {
                T1 = exp(T0);
                T3 = T1 * (1.0 + 2.0 * T1);
            }
            else
            {
                T1 = MIN_EXP;
                T3 = T1 * (1.0 + 2.0 * T1);
            }
            T3 = pParam->BSIM3v32dvt0 * T3 * tmp1;

            T4 = this->BSIM3v32tox * pParam->BSIM3v32phi / (pParam->BSIM3v32weff + pParam->BSIM3v32w0);

            T0 = sqrt(1.0 + pParam->BSIM3v32nlx / pParam->BSIM3v32leff);
            T5 = pParam->BSIM3v32k1ox * (T0 - 1.0) * pParam->BSIM3v32sqrtPhi + (pParam->BSIM3v32kt1 + pParam->BSIM3v32kt1l / pParam->BSIM3v32leff) * (TRatio - 1.0);

            tmp3 = this->BSIM3v32type * pParam->BSIM3v32vth0 - T2 - T3 + pParam->BSIM3v32k3 * T4 + T5;
            pParam->BSIM3v32vfbzb = tmp3 - pParam->BSIM3v32phi - pParam->BSIM3v32k1 * pParam->BSIM3v32sqrtPhi;
            /* End of vfbzb */
        }

        /* adding delvto  */
        dev->BSIM3v32vth0 = pParam->BSIM3v32vth0 + dev->BSIM3v32delvto;
        dev->BSIM3v32vfb = pParam->BSIM3v32vfb + this->BSIM3v32type * dev->BSIM3v32delvto;
        dev->BSIM3v32vfbzb = pParam->BSIM3v32vfbzb + this->BSIM3v32type * dev->BSIM3v32delvto;

        /* low field mobility multiplier */
        dev->BSIM3v32u0temp = pParam->BSIM3v32u0temp * dev->BSIM3v32mulu0;

        dev->BSIM3v32tconst = dev->BSIM3v32u0temp * pParam->BSIM3v32elm / (this->BSIM3v32cox * pParam->BSIM3v32weffCV * pParam->BSIM3v32leffCV * T0);

        /* process source/drain series resistance */
        /* ACM model */

        double DrainResistance, SourceResistance;

        if (this->BSIM3v32acmMod == 0)
        {
            DrainResistance = this->BSIM3v32sheetResistance * dev->BSIM3v32drainSquares;
            SourceResistance = this->BSIM3v32sheetResistance * dev->BSIM3v32sourceSquares;
        }
        else /* ACM > 0 */
        {
            error = ACM_SourceDrainResistances(
                this->BSIM3v32acmMod,
                this->BSIM3v32ld,
                this->BSIM3v32ldif,
                this->BSIM3v32hdif,
                this->BSIM3v32wmlt,
                dev->BSIM3v32w,
                this->BSIM3v32xw,
                this->BSIM3v32sheetResistance,
                dev->BSIM3v32drainSquaresGiven,
                this->BSIM3v32rd,
                this->BSIM3v32rdc,
                dev->BSIM3v32drainSquares,
                dev->BSIM3v32sourceSquaresGiven,
                this->BSIM3v32rs,
                this->BSIM3v32rsc,
                dev->BSIM3v32sourceSquares,
                &DrainResistance,
                &SourceResistance);
            if (error)
                return (error);
        }
        if (DrainResistance > 0.0)
            dev->BSIM3v32drainConductance = 1.0 / DrainResistance;
        else
            dev->BSIM3v32drainConductance = 0.0;

        if (SourceResistance > 0.0)
            dev->BSIM3v32sourceConductance = 1.0 / SourceResistance;
        else
            dev->BSIM3v32sourceConductance = 0.0;

        dev->BSIM3v32cgso = pParam->BSIM3v32cgso;
        dev->BSIM3v32cgdo = pParam->BSIM3v32cgdo;

        Nvtm = this->BSIM3v32vtm * this->BSIM3v32jctEmissionCoeff;
        if (this->BSIM3v32acmMod == 0)
        {
            if ((dev->BSIM3v32sourceArea <= 0.0) &&
                (dev->BSIM3v32sourcePerimeter <= 0.0))
            {
                SourceSatCurrent = 1.0e-14;
            }
            else
            {
                SourceSatCurrent = dev->BSIM3v32sourceArea * this->BSIM3v32jctTempSatCurDensity + dev->BSIM3v32sourcePerimeter * this->BSIM3v32jctSidewallTempSatCurDensity;
            }

            if ((dev->BSIM3v32drainArea <= 0.0) &&
                (dev->BSIM3v32drainPerimeter <= 0.0))
            {
                DrainSatCurrent = 1.0e-14;
            }
            else
            {
                DrainSatCurrent = dev->BSIM3v32drainArea * this->BSIM3v32jctTempSatCurDensity + dev->BSIM3v32drainPerimeter * this->BSIM3v32jctSidewallTempSatCurDensity;
            }
        }
        else /* ACM > 0 */
        {
            error = ACM_saturationCurrents(
                this->BSIM3v32acmMod,
                this->BSIM3v32calcacm,
                dev->BSIM3v32geo,
                this->BSIM3v32hdif,
                this->BSIM3v32wmlt,
                dev->BSIM3v32w,
                this->BSIM3v32xw,
                this->BSIM3v32jctTempSatCurDensity,
                this->BSIM3v32jctSidewallTempSatCurDensity,
                dev->BSIM3v32drainAreaGiven,
                dev->BSIM3v32drainArea,
                dev->BSIM3v32drainPerimeterGiven,
                dev->BSIM3v32drainPerimeter,
                dev->BSIM3v32sourceAreaGiven,
                dev->BSIM3v32sourceArea,
                dev->BSIM3v32sourcePerimeterGiven,
                dev->BSIM3v32sourcePerimeter,
                &DrainSatCurrent,
                &SourceSatCurrent);
            if (error)
                return (error);
        }

        if ((SourceSatCurrent > 0.0) && (this->BSIM3v32ijth > 0.0))
        {
            dev->BSIM3v32vjsm = Nvtm * log(this->BSIM3v32ijth / SourceSatCurrent + 1.0);
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
            case BSIM3v32V322:
                dev->BSIM3v32IsEvjsm =
                    SourceSatCurrent * exp(dev->BSIM3v32vjsm / Nvtm);
                break;
            case BSIM3v32V32:
            default:
                /* Do nothing */
                break;
            }
        }

        if ((DrainSatCurrent > 0.0) && (this->BSIM3v32ijth > 0.0))
        {
            dev->BSIM3v32vjdm = Nvtm * log(this->BSIM3v32ijth / DrainSatCurrent + 1.0);
            /* Added revision dependent code */
            switch (this->BSIM3v32intVersion)
            {
            case BSIM3v32V324:
            case BSIM3v32V323:
            case BSIM3v32V322:
                dev->BSIM3v32IsEvjdm =
                    DrainSatCurrent * exp(dev->BSIM3v32vjdm / Nvtm);
                break;
            case BSIM3v32V32:
            default:
                /* Do nothing */
                break;
            }
        }

        return 0;
    }

    int BSIM3v32model::check(BSIM3V32 *here, Environment *env)
    {
        struct bsim3v32SizeDependParam *pParam;
        int Fatal_Flag = 0;
        pParam = here->pParam;

        env->logging(Environment::INFO, "\nChecking parameters for BSIM 3.2 model %s\n", this->name());

        if ((strcmp(this->BSIM3v32version, "3.2.4")) && (strncmp(this->BSIM3v32version, "3.24", 4)) && (strcmp(this->BSIM3v32version, "3.2.3")) && (strncmp(this->BSIM3v32version, "3.23", 4)) && (strcmp(this->BSIM3v32version, "3.2.2")) && (strncmp(this->BSIM3v32version, "3.22", 4)) && (strncmp(this->BSIM3v32version, "3.2", 3)) && (strncmp(this->BSIM3v32version, "3.20", 4)))
        {
            env->logging(Environment::WARN, "This model supports BSIM3v3.2, BSIM3v3.2.2, BSIM3v3.2.3, BSIM3v3.2.4\n"
                                            "You specified a wrong version number. Working now with BSIM3v3.2.4.\n");
        }

        if (pParam->BSIM3v32nlx < -pParam->BSIM3v32leff)
        {
            env->logging(Environment::ERR, "Nlx = %g is less than -Leff.\n", pParam->BSIM3v32nlx);
            Fatal_Flag = 1;
        }

        if (this->BSIM3v32tox <= 0.0)
        {
            env->logging(Environment::ERR, "Tox = %g is not positive.\n", this->BSIM3v32tox);
            Fatal_Flag = 1;
        }

        if (this->BSIM3v32toxm <= 0.0)
        {
            env->logging(Environment::ERR, "Toxm = %g is not positive.\n", this->BSIM3v32toxm);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32npeak <= 0.0)
        {
            env->logging(Environment::ERR, "Nch = %g is not positive.\n", pParam->BSIM3v32npeak);
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32nsub <= 0.0)
        {
            env->logging(Environment::ERR, "Nsub = %g is not positive.\n", pParam->BSIM3v32nsub);
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32ngate < 0.0)
        {
            env->logging(Environment::ERR, "Ngate = %g is not positive.\n", pParam->BSIM3v32ngate);
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32ngate > 1.e25)
        {
            env->logging(Environment::ERR, "Ngate = %g is too high.\n", pParam->BSIM3v32ngate);
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32xj <= 0.0)
        {
            env->logging(Environment::ERR, "Xj = %g is not positive.\n", pParam->BSIM3v32xj);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32dvt1 < 0.0)
        {
            env->logging(Environment::ERR, "Dvt1 = %g is negative.\n", pParam->BSIM3v32dvt1);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32dvt1w < 0.0)
        {
            env->logging(Environment::ERR, "Dvt1w = %g is negative.\n", pParam->BSIM3v32dvt1w);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32w0 == -pParam->BSIM3v32weff)
        {
            env->logging(Environment::ERR, "(W0 + Weff) = 0 causing divided-by-zero.\n");
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32dsub < 0.0)
        {
            env->logging(Environment::ERR, "Dsub = %g is negative.\n", pParam->BSIM3v32dsub);
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32b1 == -pParam->BSIM3v32weff)
        {
            env->logging(Environment::ERR, "(B1 + Weff) = 0 causing divided-by-zero.\n");
            Fatal_Flag = 1;
        }
        if (pParam->BSIM3v32u0temp <= 0.0)
        {
            env->logging(Environment::ERR, "u0 at current temperature = %g is not positive.\n", pParam->BSIM3v32u0temp);
            Fatal_Flag = 1;
        }

        /* Check delta parameter */
        if (pParam->BSIM3v32delta < 0.0)
        {
            env->logging(Environment::ERR, "Delta = %g is less than zero.\n", pParam->BSIM3v32delta);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32vsattemp <= 0.0)
        {
            env->logging(Environment::ERR, "Vsat at current temperature = %g is not positive.\n", pParam->BSIM3v32vsattemp);
            Fatal_Flag = 1;
        }
        /* Check Rout parameters */
        if (pParam->BSIM3v32pclm <= 0.0)
        {
            env->logging(Environment::ERR, "Pclm = %g is not positive.\n", pParam->BSIM3v32pclm);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32drout < 0.0)
        {
            env->logging(Environment::ERR, "Drout = %g is negative.\n", pParam->BSIM3v32drout);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32pscbe2 <= 0.0)
        {
            env->logging(Environment::WARN, "Pscbe2 = %g is not positive.\n", pParam->BSIM3v32pscbe2);
        }

        /* ACM model */
        if (this->BSIM3v32acmMod == 0)
        {
            if (this->BSIM3v32unitLengthSidewallJctCap > 0.0 ||
                this->BSIM3v32unitLengthGateSidewallJctCap > 0.0)
            {
                if (here->BSIM3v32drainPerimeter < pParam->BSIM3v32weff)
                {
                    env->logging(Environment::WARN, "Pd = %g is less than W.\n", here->BSIM3v32drainPerimeter);
                }
                if (here->BSIM3v32sourcePerimeter < pParam->BSIM3v32weff)
                {
                    env->logging(Environment::WARN, "Ps = %g is less than W.\n", here->BSIM3v32sourcePerimeter);
                }
            }
        }

        if ((this->BSIM3v32calcacm > 0) && (this->BSIM3v32acmMod != 12))
        {
            env->logging(Environment::WARN, "CALCACM = %d is wrong. Set back to 0.\n", this->BSIM3v32calcacm);
            this->BSIM3v32calcacm = 0;
        }

        if (pParam->BSIM3v32noff < 0.1)
        {
            env->logging(Environment::WARN, "Noff = %g is too small.\n", pParam->BSIM3v32noff);
        }
        if (pParam->BSIM3v32noff > 4.0)
        {
            env->logging(Environment::WARN, "Noff = %g is too large.\n", pParam->BSIM3v32noff);
        }

        if (pParam->BSIM3v32voffcv < -0.5)
        {
            env->logging(Environment::WARN, "Voffcv = %g is too small.\n", pParam->BSIM3v32voffcv);
        }
        if (pParam->BSIM3v32voffcv > 0.5)
        {
            env->logging(Environment::WARN, "Voffcv = %g is too large.\n", pParam->BSIM3v32voffcv);
        }

        if (this->BSIM3v32ijth < 0.0)
        {
            env->logging(Environment::ERR, "Ijth = %g cannot be negative.\n", this->BSIM3v32ijth);
            Fatal_Flag = 1;
        }

        /* Check capacitance parameters */
        if (pParam->BSIM3v32clc < 0.0)
        {
            env->logging(Environment::ERR, "Clc = %g is negative.\n", pParam->BSIM3v32clc);
            Fatal_Flag = 1;
        }

        if (pParam->BSIM3v32moin < 5.0)
        {
            env->logging(Environment::WARN, "Moin = %g is too small.\n", pParam->BSIM3v32moin);
        }
        if (pParam->BSIM3v32moin > 25.0)
        {
            env->logging(Environment::WARN, "Moin = %g is too large.\n", pParam->BSIM3v32moin);
        }

        if (this->BSIM3v32capMod == 3)
        {
            if (pParam->BSIM3v32acde < 0.4)
            {
                env->logging(Environment::WARN, " Acde = %g is too small.\n", pParam->BSIM3v32acde);
            }
            if (pParam->BSIM3v32acde > 1.6)
            {
                env->logging(Environment::WARN, " Acde = %g is too large.\n", pParam->BSIM3v32acde);
            }
        }

        if (this->BSIM3v32paramChk == 1)
        {
            /* Check L and W parameters */
            if (pParam->BSIM3v32leff <= 5.0e-8)
            {
                env->logging(Environment::WARN, "Leff = %g may be too small.\n", pParam->BSIM3v32leff);
            }

            if (pParam->BSIM3v32leffCV <= 5.0e-8)
            {
                env->logging(Environment::WARN, "Leff for CV = %g may be too small.\n", pParam->BSIM3v32leffCV);
            }

            if (pParam->BSIM3v32weff <= 1.0e-7)
            {
                env->logging(Environment::WARN, "Weff = %g may be too small.\n", pParam->BSIM3v32weff);
            }

            if (pParam->BSIM3v32weffCV <= 1.0e-7)
            {
                env->logging(Environment::WARN, "Weff for CV = %g may be too small.\n", pParam->BSIM3v32weffCV);
            }

            /* Check threshold voltage parameters */
            if (pParam->BSIM3v32nlx < 0.0)
            {
                env->logging(Environment::WARN, "Nlx = %g is negative.\n", pParam->BSIM3v32nlx);
            }
            if (this->BSIM3v32tox < 1.0e-9)
            {
                env->logging(Environment::WARN, "Tox = %g is less than 10A.\n", this->BSIM3v32tox);
            }

            if (pParam->BSIM3v32npeak <= 1.0e15)
            {
                env->logging(Environment::WARN, "Nch = %g may be too small.\n", pParam->BSIM3v32npeak);
            }
            else if (pParam->BSIM3v32npeak >= 1.0e21)
            {
                env->logging(Environment::WARN, "Nch = %g may be too large.\n", pParam->BSIM3v32npeak);
            }

            if (pParam->BSIM3v32nsub <= 1.0e14)
            {
                env->logging(Environment::WARN, "Nsub = %g may be too small.\n", pParam->BSIM3v32nsub);
            }
            else if (pParam->BSIM3v32nsub >= 1.0e21)
            {
                env->logging(Environment::WARN, "Nsub = %g may be too large.\n", pParam->BSIM3v32nsub);
            }

            if ((pParam->BSIM3v32ngate > 0.0) &&
                (pParam->BSIM3v32ngate <= 1.e18))
            {
                env->logging(Environment::WARN, "Ngate = %g is less than 1.E18cm^-3.\n", pParam->BSIM3v32ngate);
            }

            if (pParam->BSIM3v32dvt0 < 0.0)
            {
                env->logging(Environment::WARN, "Dvt0 = %g is negative.\n", pParam->BSIM3v32dvt0);
            }

            if (fabs(1.0e-6 / (pParam->BSIM3v32w0 + pParam->BSIM3v32weff)) > 10.0)
            {
                env->logging(Environment::WARN, "(W0 + Weff) may be too small.\n");
            }

            /* Check subthreshold parameters */
            if (pParam->BSIM3v32nfactor < 0.0)
            {
                env->logging(Environment::WARN, "Nfactor = %g is negative.\n", pParam->BSIM3v32nfactor);
            }
            if (pParam->BSIM3v32cdsc < 0.0)
            {
                env->logging(Environment::WARN, "Cdsc = %g is negative.\n", pParam->BSIM3v32cdsc);
            }
            if (pParam->BSIM3v32cdscd < 0.0)
            {
                env->logging(Environment::WARN, "Cdscd = %g is negative.\n", pParam->BSIM3v32cdscd);
            }
            /* Check DIBL parameters */
            if (pParam->BSIM3v32eta0 < 0.0)
            {
                env->logging(Environment::WARN, "Eta0 = %g is negative.\n", pParam->BSIM3v32eta0);
            }

            /* Check Abulk parameters */
            if (fabs(1.0e-6 / (pParam->BSIM3v32b1 + pParam->BSIM3v32weff)) > 10.0)
            {
                env->logging(Environment::WARN, "(B1 + Weff) may be too small.\n");
            }

            /* Check Saturation parameters */
            if (pParam->BSIM3v32a2 < 0.01)
            {
                env->logging(Environment::WARN, "A2 = %g is too small. Set to 0.01.\n", pParam->BSIM3v32a2);
                pParam->BSIM3v32a2 = 0.01;
            }
            else if (pParam->BSIM3v32a2 > 1.0)
            {
                env->logging(Environment::WARN, "A2 = %g is larger than 1. A2 is set to 1 and A1 is set to 0.\n", pParam->BSIM3v32a2);
                pParam->BSIM3v32a2 = 1.0;
                pParam->BSIM3v32a1 = 0.0;
            }

            if (pParam->BSIM3v32rdsw < 0.0)
            {
                env->logging(Environment::WARN, "Rdsw = %g is negative. Set to zero.\n", pParam->BSIM3v32rdsw);
                pParam->BSIM3v32rdsw = 0.0;
                pParam->BSIM3v32rds0 = 0.0;
            }
            else if ((pParam->BSIM3v32rds0 > 0.0) && (pParam->BSIM3v32rds0 < 0.001))
            {
                env->logging(Environment::WARN, "Rds at current temperature = %g is less than 0.001 ohm. Set to zero.\n", pParam->BSIM3v32rds0);
                pParam->BSIM3v32rds0 = 0.0;
            }
            if (pParam->BSIM3v32vsattemp < 1.0e3)
            {
                env->logging(Environment::WARN, "Vsat at current temperature = %g may be too small.\n", pParam->BSIM3v32vsattemp);
            }

            if (pParam->BSIM3v32pdibl1 < 0.0)
            {
                env->logging(Environment::WARN, "Pdibl1 = %g is negative.\n", pParam->BSIM3v32pdibl1);
            }
            if (pParam->BSIM3v32pdibl2 < 0.0)
            {
                env->logging(Environment::WARN, "Pdibl2 = %g is negative.\n", pParam->BSIM3v32pdibl2);
            }
            /* Check overlap capacitance parameters */
            if (this->BSIM3v32cgdo < 0.0)
            {
                env->logging(Environment::WARN, "cgdo = %g is negative. Set to zero.\n", this->BSIM3v32cgdo);
                this->BSIM3v32cgdo = 0.0;
            }
            if (this->BSIM3v32cgso < 0.0)
            {
                env->logging(Environment::WARN, "cgso = %g is negative. Set to zero.\n", this->BSIM3v32cgso);
                this->BSIM3v32cgso = 0.0;
            }
            if (this->BSIM3v32cgbo < 0.0)
            {
                env->logging(Environment::WARN, "cgbo = %g is negative. Set to zero.\n", this->BSIM3v32cgbo);
                this->BSIM3v32cgbo = 0.0;
            }
        }

        return Fatal_Flag;
    }

    static PropertyMdl *bsim3v32_createNmosMdl()
    {
        PropertyMdl *mdl = new BSIM3v32model();
        mdl->setProperty(BSIM3v32_MOD_NMOS, Variant(Variant::VariantBoolean).setBoolean(true));
        mdl->setProperty(BSIM3v32_MOD_PMOS, Variant(Variant::VariantBoolean).setBoolean(false));
        return mdl;
    }
    static PropertyMdl *bsim3v32_createPmosMdl()
    {
        PropertyMdl *mdl = new BSIM3v32model();
        mdl->setProperty(BSIM3v32_MOD_NMOS, Variant(Variant::VariantBoolean).setBoolean(false));
        mdl->setProperty(BSIM3v32_MOD_PMOS, Variant(Variant::VariantBoolean).setBoolean(true));
        return mdl;
    }

    static void bsim3v32_deleteMdl(PropertyMdl *mdl)
    {
        delete mdl;
    }

    static const PropertyMdlDescriptor mdlDescriptorsArray[] = {
        {"nmos", bsim3v32_mdl_props, ARRAY_COUNT(bsim3v32_mdl_props), &bsim3v32_createNmosMdl, &bsim3v32_deleteMdl},
        {"pmos", bsim3v32_mdl_props, ARRAY_COUNT(bsim3v32_mdl_props), &bsim3v32_createPmosMdl, &bsim3v32_deleteMdl},
    };
}

/* Exported symbols */
const PropertyMdlDescriptor *mdlDescriptors = csimModel::mdlDescriptorsArray;
const size_t numMdlDescriptors = ARRAY_COUNT(csimModel::mdlDescriptorsArray);
