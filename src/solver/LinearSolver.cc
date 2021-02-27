/**
 * @file Linear equations solver
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

#include <cstring>
#include "LinearSolverGauss.h"
#include "LinearSolverLU.h"
#include "csim/internal/LinearSolver.h"

namespace csim
{
    /**
     * @brief Create instance of Linear Solver by its algorithm name
     * @param algorithm Name of algorithm
     * @retval nullptr if failed
     * @retval Pointer to the solver.
     */
    LinearSolver *LinearSolver::createInstance(const char *algorithm)
    {
        if (0 == strcmp(algorithm, "gauss"))
        {
            return new LinearSolverGauss();
        }
        else if (0 == strcmp(algorithm, "LU"))
        {
            return new LinearSolverLU();
        }
        else
        {
            return nullptr;
        }
    }
}
