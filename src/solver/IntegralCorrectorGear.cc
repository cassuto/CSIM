/**
 * @file Implicit GEAR Integral Corrector
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

#include <algorithm>
#include "csim/utils/errors.h"
#include "csim/internal/LinearSolver.h"
#include "IntegralCorrectorGear.h"

namespace csim
{
    IntegralCorrectorGear::IntegralCorrectorGear()
        : m_linearSolver(LinearSolver::createInstance("gauss"))
    {
    }

    IntegralCorrectorGear::~IntegralCorrectorGear()
    {
        delete m_linearSolver;
    }

    int IntegralCorrectorGear::setOrder(unsigned int order, const IntegralHistory *hsteps)
    {
        IntegralCorrector::setOrder(order, nullptr);
        UPDATE_RC(setStep(hsteps));

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

        std::fill(m_A, m_A + rows * rows, 0.0);
        std::fill(m_b, m_b + rows, 0.0);

        /* Generate A */
        for (unsigned int i = 1; i <= order; i++)
        {
            m_A[i * rows] = i;
            m_A[i] = 1;
        }
        for (unsigned int i = 1; i <= order - 1; i++)
        {
            double t = -(long)i;
            for (unsigned int j = 1; j <= order; j++)
            {
                m_A[j * rows + (i + 1)] = t;
                t *= -(long)i;
            }
        }
        /* Generate b */
        for (unsigned int i = 0; i <= order; i++)
            m_b[i] = 1;

        /* Solve x */
        if (m_linearSolver->solve(m_A, rows, m_x, m_b))
            return CERR_SET_INTEGRAL_ORDER;

        /*
         * Generate truncation error coefficient
         */
        m_trucnErrorCoeff = 0.0;
        double f = 1;
        for (int i = order + 1; i > 1; --i)
            f *= i;
        for (int i = -1; i < (int)order - 1; ++i)
        {
            double a = (i == -1 ? -1.0 : m_x[i + 1].real());
            m_trucnErrorCoeff -= a * std::pow((int)order - 1 - i, order + 1);
        }
        m_trucnErrorCoeff /= f;
        f /= order + 1;
        m_trucnErrorCoeff -= m_x[0].real() * std::pow(order, order) / f;

        return 0;
    }

    int IntegralCorrectorGear::setStep(const IntegralHistory *hsteps)
    {
        unsigned int order = getOrder();
        unsigned int rows = order + 1;

        std::fill(m_A, m_A + rows * rows, 0.0);
        std::fill(m_b, m_b + rows, 0.0);

        /* Generate A */
        for (unsigned int c = 0; c <= order; c++)
            m_A[c] = 1.0;
        double f = 0;
        for (unsigned int c = 0; c < order; c++)
        {
            f += hsteps->get(c);
            double a = 1.0;
            for (unsigned int r = 0; r < order; r++)
            {
                a *= f / hsteps->get(0);
                m_A[(r + 1) * rows + c + 1] = a;
            }
        }
        /* Generate B */
        m_b[1] = -1.0 / hsteps->get(0);

        /* Solve x */
        if (m_linearSolver->solve(m_A, rows, m_x, m_b))
            return CERR_SET_INTEGRAL_STEP;

        for (unsigned int r = 0; r <= order; r++)
        {
            m_coeffs[r] = m_x[r].real();
        }

        return 0;
    }

    void IntegralCorrectorGear::integrate(const IntegralHistory *x, IntegralHistory *y, double k, double *c0, double *c1)
    {
        *c0 = k * m_coeffs[0];
        *c1 = 0.0;
        for (unsigned int i = 1; i <= getOrder(); i++)
            *c1 += k * m_coeffs[i] * x->get(i);
        y->set(0, x->get(0) * (*c0) + *c1);
    }

    double IntegralCorrectorGear::getTruncErrorCoeff()
    {
        return m_trucnErrorCoeff;
    }

}
