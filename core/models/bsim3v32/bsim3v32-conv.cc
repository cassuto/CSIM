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

    bool BSIM3V32::checkConvergence()
    {
        Environment *env = getEnvironment();
        double delvbd, delvbs, delvds, delvgd, delvgs, vbd, vbs, vds;
        double cbd, cbhat, cbs, cd, cdhat, tol, vgd, vgdo, vgs;

        vbs = model->BSIM3v32type * (getPrevU(this->BSIM3v32bNode).real() - getPrevU(this->BSIM3v32sNodePrime).real());
        vgs = model->BSIM3v32type * (getPrevU(this->BSIM3v32gNode).real() - getPrevU(this->BSIM3v32sNodePrime).real());
        vds = model->BSIM3v32type * (getPrevU(this->BSIM3v32dNodePrime).real() - getPrevU(this->BSIM3v32sNodePrime).real());
        vbd = vbs - vds;
        vgd = vgs - vds;
        vgdo = getIntegratorX(BSIM3v32vgs)->get(0) - getIntegratorX(BSIM3v32vds)->get(0);
        delvbs = vbs - getIntegratorX(BSIM3v32vbs)->get(0);
        delvbd = vbd - getIntegratorX(BSIM3v32vbd)->get(0);
        delvgs = vgs - getIntegratorX(BSIM3v32vgs)->get(0);
        delvds = vds - getIntegratorX(BSIM3v32vds)->get(0);
        delvgd = vgd - vgdo;

        cd = this->BSIM3v32cd - this->BSIM3v32cbd;
        if (this->BSIM3v32mode >= 0)
        {
            cd += this->BSIM3v32csub;
            cdhat = cd - this->BSIM3v32gbd * delvbd + (this->BSIM3v32gmbs + this->BSIM3v32gbbs) * delvbs + (this->BSIM3v32gm + this->BSIM3v32gbgs) * delvgs + (this->BSIM3v32gds + this->BSIM3v32gbds) * delvds;
        }
        else
        {
            cdhat = cd + (this->BSIM3v32gmbs - this->BSIM3v32gbd) * delvbd + this->BSIM3v32gm * delvgd - this->BSIM3v32gds * delvds;
        }

        /*
         *  check convergence
         */
        if ((this->BSIM3v32off == 0) || (!(env->getCKTmode() & MODEINITFIX)))
        {
            tol = env->getRelTol() * std::max(fabs(cdhat), fabs(cd)) + env->getAbsTol();
            if (fabs(cdhat - cd) >= tol)
            {
                return false;
            }
            cbs = this->BSIM3v32cbs;
            cbd = this->BSIM3v32cbd;
            if (this->BSIM3v32mode >= 0)
            {
                cbhat = cbs + cbd - this->BSIM3v32csub + this->BSIM3v32gbd * delvbd + (this->BSIM3v32gbs - this->BSIM3v32gbbs) * delvbs - this->BSIM3v32gbgs * delvgs - this->BSIM3v32gbds * delvds;
            }
            else
            {
                cbhat = cbs + cbd - this->BSIM3v32csub + this->BSIM3v32gbs * delvbs + (this->BSIM3v32gbd - this->BSIM3v32gbbs) * delvbd - this->BSIM3v32gbgs * delvgd + this->BSIM3v32gbds * delvds;
            }
            tol = env->getRelTol() * std::max(fabs(cbhat),
                                              fabs(cbs + cbd - this->BSIM3v32csub)) +
                  env->getAbsTol();
            if (fabs(cbhat - (cbs + cbd - this->BSIM3v32csub)) > tol)
            {
                return false;
            }
        }

        return true;
    }

}
