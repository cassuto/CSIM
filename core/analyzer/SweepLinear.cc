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
#include <cmath>
#include <algorithm>
#include "csim/internal/SweepLinear.h"

namespace csim
{
    int SweepLinear::init(double start, double stop, int points)
    {
        m_start = start;
        m_step = (stop - start) / (points - 1);
        return 0;
    }
    double SweepLinear::next()
    {
        assert(hasNext());
        return m_start + (m_point++ * m_step);
    }
    bool SweepLinear::hasNext()
    {
        return m_point <= m_numPoints;
    }
}
