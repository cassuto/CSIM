/**
 * @file Numberic integral corrector
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
#include "IntegralPredictorEuler.h"
#include "IntegralPredictorGear.h"
#include "csim/internal/IntegralPredictor.h"

namespace csim
{

    /**
     * @brief Create instance of integral predictor by its algorithm name
     * @param algorithm Name of algorithm
     * @retval nullptr if failed
     * @retval Pointer to the integral predictor.
     */
    IntegralPredictor *IntegralPredictor::createInstance(const std::string &algorithm)
    {
        if (0 == algorithm.compare("euler"))
        {
            return new IntegralPredictorEuler();
        }
        else if (0 == algorithm.compare("gear"))
        {
            return new IntegralPredictorGear();
        }
        else
        {
            return nullptr;
        }
    }
}
