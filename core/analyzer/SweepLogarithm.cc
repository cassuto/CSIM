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

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "csim/utils/errors.h"
#include "csim/internal/SweepLogarithm.h"

namespace csim
{
    int SweepLogarithm::init(double start, double stop, int points)
    {
        if (start * stop < 0 || points<2)
            return CERR_INVALID_PARAMETER;

        
        m_start = start;
        m_step = (std::log(stop) - std::log(start)) / (points - 1);
        m_point = 0;
        m_numPoints = points;
        return 0;
    }
    double SweepLogarithm::next()
    {
        assert(hasNext());
        return m_start * std::exp(m_step * (m_point++));
    }
    bool SweepLogarithm::hasNext()
    {
        return m_point <= m_numPoints;
    }
}
