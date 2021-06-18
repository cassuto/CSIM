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

#ifndef CSIM_LINEARSOLVEROPTIMIZER_H_
#define CSIM_LINEARSOLVEROPTIMIZER_H_

#include "csim/model/Types.h"

namespace csim
{
    class LinearSolverOptimizer
    {
    public:
        virtual ~LinearSolverOptimizer(){};
        virtual void reset() = 0;
        virtual int singularRow(csimModel::MComplex *A, unsigned curRow, unsigned int nRows) = 0;
    };
}

#endif // CSIM_LINEARSOLVEROPTIMIZER_H_