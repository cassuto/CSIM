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

#ifndef CSIM_COMPACT_SPICE_COMPATIBLE_H_
#define CSIM_COMPACT_SPICE_COMPATIBLE_H_

#include "csim/model/Types.h"

#define ENABLE_SPICE_COMPATIBLE

namespace csimModel
{
    class SPICE_Compatible
    {
    public:
        SPICE_Compatible();

        /* SPICE compatible CKTmode */
        enum SPICE_Mode
        {
            /* old 'mode' parameters */
            SPICE_MODE = 0x3U,
            SPICE_MODETRAN = 0x1U,
            SPICE_MODEAC = 0x2U,

            /* for noise analysis */
            SPICE_MODEACNOISE = 0x8U,

            /* old 'modedc' parameters */
            SPICE_MODEDC = 0x70U,
            SPICE_MODEDCOP = 0x10U,
            SPICE_MODETRANOP = 0x20U,
            SPICE_MODEDCTRANCURVE = 0x40U,

            /* old 'initf' parameters */
            SPICE_INITF = 0x3f00U,
            SPICE_MODEINITFLOAT = 0x100U,
            SPICE_MODEINITJCT = 0x200U,
            SPICE_MODEINITFIX = 0x400U,
            SPICE_MODEINITSMSIG = 0x800U,
            SPICE_MODEINITTRAN = 0x1000U,
            SPICE_MODEINITPRED = 0x2000U,

            /* old 'nosolv' paramater */
            SPICE_MODEUIC = 0x10000UL
        };

        inline void setMode(long spiceCompatibleMode)
        {
            m_spiceCompatibleMode = spiceCompatibleMode;
        }
        inline long getMode() const
        {
            return m_spiceCompatibleMode;
        }

        inline bool getUseInitialCondition() const
        {
            return m_useInitialCondition;
        }
        
        int setFlagsOP();
        int setFlagsTrOP();
        int setFlagsDC();
        int setFlagsAC();
        int setFlagsTR();
        int setFlagsTRPred();
        int setFlagsSmallSig();
        int upateStateMachine(bool converged);

    private:
        bool m_useInitialCondition;
        long m_spiceCompatibleMode;
    };

}

#endif // CSIM_COMPACT_SPICE_COMPATIBLE_H_
