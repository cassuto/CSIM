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

#ifndef CSIM_SWEEPLOGARITHM_H_
#define CSIM_SWEEPLOGARITHM_H_

#include "csim/internal/Sweep.h"

namespace csim
{

    class SweepLogarithm : public Sweep
    {
    public:
        virtual int init(double start, double stop, int points);
        virtual double next();
        virtual bool hasNext();

    private:
        double m_step;
        double m_start;
        int m_inverse;
        int m_point, m_numPoints;
    };

}

#endif // CSIM_SWEEPLOGARITHM_H_
