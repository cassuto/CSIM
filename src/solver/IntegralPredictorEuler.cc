/**
 * @file Complex type restricted to model SDK
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition; you can redistribute it and/or           
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation; either      
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cassert>
#include <cstring>
#include <iostream>
#include "csim/utils/errors.h"
#include "IntegralPredictorEuler.h"

namespace csim
{
    void IntegralPredictorEuler::setOrder(unsigned int order)
    {
        assert(order == 1);
    }

    double IntegralPredictorEuler::predict(const IntegralHistory *x, IntegralHistory *steps)
    {
        /* The previous difference */
        double f = (x->getX(1) - x->getX(2)) / steps->getX(1);
        steps->pushX(getStep());
        return x->getX(1) + getStep() * f;
    }

}
