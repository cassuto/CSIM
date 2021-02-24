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

        virtual int solve(const Complex *A, int n, Complex *x, const Complex *B) = 0;

    public:
        static LinearSolver *createInstance(const char *algorithm);
    };

    class LinearSolverGauss : public LinearSolver
    {
    public:
        LinearSolverGauss();
        virtual ~LinearSolverGauss();

        virtual int solve(const Complex *A, int n, Complex *x, const Complex *B);

    private:
        Complex *m_bufRow, *m_bufA, *m_bufB;
        int m_bufRows;
        size_t m_lenRow;
    };
}

#endif // CSIM_LINEARSOLVER_H_