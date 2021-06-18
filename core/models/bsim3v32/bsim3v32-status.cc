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
#include "BSIM3V32.h"

namespace csimModel
{

    Variant BSIM3V32::queryStatus(unsigned int id, int *rc)
    {
        *rc = 0;
        if (!model)
        {
            *rc = CERR_NO_MDL;
            return Variant(Variant::VariantUnknown);
        }

        switch (id)
        {
        case BSIM3v32_STAT_L:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32l * model->BSIM3v32lmlt);
        case BSIM3v32_STAT_W:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32w);

        case BSIM3v32_STAT_M:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32m);

        case BSIM3v32_STAT_AS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32sourceArea);

        case BSIM3v32_STAT_AD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32drainArea);

        case BSIM3v32_STAT_PS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32sourcePerimeter);

        case BSIM3v32_STAT_PD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32drainPerimeter);

        case BSIM3v32_STAT_NRS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32sourceSquares);

        case BSIM3v32_STAT_NRD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32drainSquares);

        case BSIM3v32_STAT_OFF:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32off);

        case BSIM3v32_STAT_NQSMOD:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32nqsMod);

        case BSIM3v32_STAT_GEO:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32geo);

        case BSIM3v32_STAT_DELVTO:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32delvto);

        case BSIM3v32_STAT_MULU0:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32mulu0);

        case BSIM3v32_STAT_IC_VBS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32icVBS);

        case BSIM3v32_STAT_IC_VDS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32icVDS);

        case BSIM3v32_STAT_IC_VGS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32icVGS);

        case BSIM3v32_STAT_DNODE:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32dNode);

        case BSIM3v32_STAT_GNODE:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32gNode);

        case BSIM3v32_STAT_SNODE:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32sNode);

        case BSIM3v32_STAT_BNODE:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32bNode);

        case BSIM3v32_STAT_DNODEPRIME:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32dNodePrime);

        case BSIM3v32_STAT_SNODEPRIME:
            return Variant(Variant::VariantInt32).setInt32(this->BSIM3v32sNodePrime);

        case BSIM3v32_STAT_SOURCECONDUCT:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32sourceConductance * this->BSIM3v32m);

        case BSIM3v32_STAT_DRAINCONDUCT:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32drainConductance * this->BSIM3v32m);

        case BSIM3v32_STAT_VBD:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32vbd)->get(0));

        case BSIM3v32_STAT_VBS:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32vbs)->get(0));

        case BSIM3v32_STAT_VGS:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32vgs)->get(0));

        case BSIM3v32_STAT_VDS:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32vds)->get(0));

        case BSIM3v32_STAT_CD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cd * this->BSIM3v32m);

        case BSIM3v32_STAT_CBS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cbs * this->BSIM3v32m);

        case BSIM3v32_STAT_CBD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cbd * this->BSIM3v32m);

        case BSIM3v32_STAT_GM:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32gm * this->BSIM3v32m);

        case BSIM3v32_STAT_GDS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32gds * this->BSIM3v32m);

        case BSIM3v32_STAT_GMBS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32gmbs * this->BSIM3v32m);

        case BSIM3v32_STAT_GBD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32gbd * this->BSIM3v32m);

        case BSIM3v32_STAT_GBS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32gbs * this->BSIM3v32m);

        case BSIM3v32_STAT_QB:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32qb)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_CQB:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorY(BSIM3v32qb)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_QG:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32qg)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_CQG:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorY(BSIM3v32qg)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_QD:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32qd)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_CQD:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorY(BSIM3v32qd)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_CGG:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cggb * this->BSIM3v32m);

        case BSIM3v32_STAT_CGD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cgdb * this->BSIM3v32m);

        case BSIM3v32_STAT_CGS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cgsb * this->BSIM3v32m);

        case BSIM3v32_STAT_CDG:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cdgb * this->BSIM3v32m);

        case BSIM3v32_STAT_CDD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cddb * this->BSIM3v32m);

        case BSIM3v32_STAT_CDS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cdsb * this->BSIM3v32m);

        case BSIM3v32_STAT_CBG:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cbgb * this->BSIM3v32m);

        case BSIM3v32_STAT_CBDB:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cbdb * this->BSIM3v32m);

        case BSIM3v32_STAT_CBSB:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32cbsb * this->BSIM3v32m);

        case BSIM3v32_STAT_CAPBD:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32capbd * this->BSIM3v32m);

        case BSIM3v32_STAT_CAPBS:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32capbs * this->BSIM3v32m);

        case BSIM3v32_STAT_VON:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32von);

        case BSIM3v32_STAT_VDSAT:
            return Variant(Variant::VariantDouble).setDouble(this->BSIM3v32vdsat);

        case BSIM3v32_STAT_QBS:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32qbs)->get(0) * this->BSIM3v32m);

        case BSIM3v32_STAT_QBD:
            return Variant(Variant::VariantDouble).setDouble(getIntegratorX(BSIM3v32qbd)->get(0) * this->BSIM3v32m);

        default:
            *rc = CERR_INVALID_PARAMETER;
            return Variant(Variant::VariantUnknown);
        }
    }

}
