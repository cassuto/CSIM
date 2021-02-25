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
#include <iostream>
#include <cstring>
#include "csim/utils/errors.h"
#include "csim/internal/LinearSolver.h"
#include "IntegralCorrectorGear.h"

namespace csim
{
    IntegralCorrectorGear::IntegralCorrectorGear()
        : m_linearSolver(LinearSolver::createInstance("gauss"))
    {
        m_step = 1e-9;
        setOrder(2);
    }

    IntegralCorrectorGear::~IntegralCorrectorGear()
    {
        delete m_linearSolver;
    }

    void IntegralCorrectorGear::setOrder(unsigned int order)
    {
        IntegralCorrector::setOrder(order);

        /*
         * Generate constant ai and bi of GEAR algorithm.
         * 
         * The principle is shown as follows:
         * Linear multistep interation:
         * \f[
         *      x_{n+1} = \sum_{i=0}^{p}a_i x_{n-i} + h_n\sum_{i=-1}^{p}b_i f(x_{n-i},t_{n-i})
         * \f]
         * 
         * Whereas \f$ p = order + 1 \f$; ai and bi meet:
         * \f[
         *      \begin{cases}
         *      \sum\limits_{i=0}^{p}a_i=1 \\
         *      \sum\limits_{i=1}^{p}{(-i)}^ja_i+ j\sum\limits_{i=-1}^{p} {(-1)}^{j-1}b_i = 1, & j=1,2, \cdots , k 
         *      \end{cases}
         * \f]
         * 
         * For Implicit GEAR:
         * \f[
         *      p=k-1, b_0=b_1=\cdots=b_{k-1}=0
         * \f]
         * 
         * So the coefficient ai and bi can be obtained by solving the above equation.
         */

        unsigned int rows = order + 1;

        memset(m_A, 0, sizeof(*m_A) * rows * rows);
        memset(m_b, 0, sizeof(*m_b) * rows);

        /* Generate A */
        for (unsigned int i = 1; i <= order; i++)
        {
            m_A[i * rows] = i;
            m_A[i] = 1;
        }
        for (unsigned int i = 1; i <= order - 1; i++)
        {
            double t = -(long)i;
            for (int j = 1; j <= order; j++)
            {
                m_A[j * rows + (i + 1)] = t;
                t *= -(long)i;
            }
        }
        /* Generate b */
        for (int i = 0; i <= order; i++)
            m_b[i] = 1;

        /* Solve x */
        int ret = m_linearSolver->solve(m_A, rows, m_x, m_b);
        assert(CSIM_OK(ret));
        (void)ret;

        genCoeffs();
    }

    void IntegralCorrectorGear::setStep(double step)
    {
        IntegralCorrector::setStep(step);
        genCoeffs();
    }

    void IntegralCorrectorGear::genCoeffs()
    {
        double b_neg_1 = m_x[0].real();
        m_coeffs[0] = 1.0 / getStep() / b_neg_1;
        for (unsigned i = 1; i <= getOrder(); i++)
        {
            m_coeffs[i] = -1.0 / getStep() / b_neg_1 * m_x[i].real();
        }
    }

    void IntegralCorrectorGear::integrate(const IntegralHistory *x, IntegralHistory *y, double k, double *c0, double *c1)
    {
        *c0 = k * m_coeffs[0];
        *c1 = 0.0;
        for (unsigned int i = 1; i <= getOrder(); i++)
            *c1 += k * m_coeffs[i] * x->get(i);
        y->set(0, x->get(0) * (*c0) + *c1);
    }

}
