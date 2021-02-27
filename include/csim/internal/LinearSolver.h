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

#ifndef CSIM_LINEARSOLVER_H_
#define CSIM_LINEARSOLVER_H_

#include "csim/internal/Complex.h"

namespace csim
{
    class LinearSolver
    {
    public:
        LinearSolver() {}
        virtual ~LinearSolver() {}

        /**
         * Solve the linear equation Ax=B.
         * Note that the original matrix A or B may be modified after the solving process.
         * @param [in,out] A Pointer to the matrix A. NULL if you want to use the last passed value.
         *          For algorithms such as LU decomposition, using the last values can improve performance.
         * @param n Rows or columns of matrix A.
         * @param [in,out] x Where to store the solution vevtor.
         * @param [in,out] B Pointer to the matrix B.
         * @return Status code.
         */
        virtual int solve(Complex *A, int n, Complex *x, Complex *B) = 0;

    public:
        static LinearSolver *createInstance(const char *algorithm);
    };
}

#endif // CSIM_LINEARSOLVER_H_