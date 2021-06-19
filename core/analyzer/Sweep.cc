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

#include <cassert>
#include <cstring>
#include "csim/internal/SweepLinear.h"
#include "csim/internal/SweepLogarithm.h"
#include "csim/internal/SweepDecade.h"
#include "csim/internal/SweepOctave.h"

namespace csim
{
    Sweep *Sweep::createInstance(const char *name)
    {
        if (std::strcmp(name, "lin") == 0)
        {
            return new SweepLinear();
        }
        else if (std::strcmp(name, "log") == 0)
        {
            return new SweepLogarithm();
        }
        else if (std::strcmp(name, "dec") == 0)
        {
            return new SweepDecade();
        }
        else if (std::strcmp(name, "oct") == 0)
        {
            return new SweepOctave();
        }
        else
        {
            return 0l;
        }
    }
}
