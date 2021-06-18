/**
 * @file Routines to support SPICE models
 */

/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: 2001 AlansFixes
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

#include "csim/utils/errors.h"
#include "csim/model/compact/SPICE-Compatible.h"

namespace csimModel
{

    SPICE_Compatible::SPICE_Compatible()
        : m_useInitialCondition(false),
          m_spiceCompatibleMode(0U)
    {
    }

    int SPICE_Compatible::setFlagsOP()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODEDCOP | SPICE_MODEINITJCT);
        return 0;
    }
    int SPICE_Compatible::setFlagsTrOP()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODETRANOP | SPICE_MODEINITJCT);
        return 0;
    }
    int SPICE_Compatible::setFlagsDC()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODEDCTRANCURVE | SPICE_MODEINITJCT);
        return 0;
    }
    int SPICE_Compatible::setFlagsAC()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODEAC);
        return 0;
    }
    int SPICE_Compatible::setFlagsTR()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODETRAN | SPICE_MODEINITTRAN);
        return 0;
    }

    int SPICE_Compatible::setFlagsTRPred()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODETRAN | SPICE_MODEINITPRED);
        return 0;
    }

    int SPICE_Compatible::setFlagsSmallSig()
    {
        setMode((m_useInitialCondition ? SPICE_MODEUIC : 0) | SPICE_MODEDCOP | SPICE_MODEINITSMSIG);
        return 0;
    }

    int SPICE_Compatible::upateStateMachine(bool converged)
    {
        if (m_spiceCompatibleMode & SPICE_MODEINITFLOAT)
        {
            /*if ((m_spiceCompatibleMode & SPICE_MODEDC) && ckt->CKThadNodeset)
            {
                if (ipass)
                    ckt->CKTnoncon = ipass;
                ipass = 0;
            }*/
            if (converged)
            {
                return 0;
            }
        }
        else if (m_spiceCompatibleMode & SPICE_MODEINITJCT)
        {
            m_spiceCompatibleMode = (m_spiceCompatibleMode & ~SPICE_INITF) | SPICE_MODEINITFIX;
            /*ckt->CKTniState |= NISHOULDREORDER;*/
        }
        else if (m_spiceCompatibleMode & SPICE_MODEINITFIX)
        {
            if (converged)
                m_spiceCompatibleMode = (m_spiceCompatibleMode & ~SPICE_INITF) | SPICE_MODEINITFLOAT;
            /*ipass = 1;*/
        }
        else if (m_spiceCompatibleMode & SPICE_MODEINITSMSIG)
        {
            m_spiceCompatibleMode = (m_spiceCompatibleMode & ~SPICE_INITF) | SPICE_MODEINITFLOAT;
        }
        else if (m_spiceCompatibleMode & SPICE_MODEINITTRAN)
        {
            /*if (iterno <= 1)
                ckt->CKTniState |= NISHOULDREORDER;*/
            m_spiceCompatibleMode = (m_spiceCompatibleMode & ~SPICE_INITF) | SPICE_MODEINITFLOAT;
        }
        else if (m_spiceCompatibleMode & SPICE_MODEINITPRED)
        {
            m_spiceCompatibleMode = (m_spiceCompatibleMode & ~SPICE_INITF) | SPICE_MODEINITFLOAT;
        }
        return 0;
    }

}
