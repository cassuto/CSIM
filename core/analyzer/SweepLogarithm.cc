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
#include "csim/utils/errors.h"
#include "csim/internal/SweepLogarithm.h"

namespace csim
{
    int SweepLogarithm::init(double start, double stop, int points)
    {
        if (start * stop < 0)
        return CERR_INVALID_PARAMETER;

        double tmin = std::min(fabs(start), fabs(stop));
        double tmax = std::max(fabs(start), fabs(stop));
        if (fabs(start) > fabs(stop))
        {
            m_inverse = true;
            m_start = tmax;
        }
        else
        {
            m_inverse = false;
            m_start = tmin;
        }

        m_step = (std::log(tmax) - std::log(tmin)) / (points - 1);
        m_point = m_inverse ? points - 1 : 0;
        m_numPoints = points;
        return 0;
    }
    double SweepLogarithm::next()
    {
        assert(hasNext());
        if (m_inverse)
            return m_start * std::exp(m_step * (m_numPoints - 1 - m_point++));
        else
            return m_start * std::exp(m_step * (m_point++));
    }
    bool SweepLogarithm::hasNext()
    {
        return m_point <= m_numPoints;
    }
}
