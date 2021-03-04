/**
 * @file Forward Euler Integral Predictor
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
#include <algorithm>
#include "csim/utils/errors.h"
#include "csim/internal/LinearSolver.h"
#include "IntegralPredictorGear.h"

namespace csim
{
    IntegralPredictorGear::IntegralPredictorGear()
        : m_linearSolver(LinearSolver::createInstance("LU"))
    {
    }
    IntegralPredictorGear::~IntegralPredictorGear()
    {
        delete m_linearSolver;
    }

    void IntegralPredictorGear::setOrder(unsigned int order, const IntegralHistory *hsteps)
    {
        IntegralPredictor::setOrder(order, nullptr);
        setStep(hsteps);
    }

    void IntegralPredictorGear::setStep(const IntegralHistory *hsteps)
    {
        unsigned int rows = getOrder() + 1;

        std::fill(m_A, m_A + rows * rows, 0.0);
        std::fill(m_b, m_b + rows, 0.0);

        /* Generate A */
        for (unsigned int j = 0; j <= getOrder(); j++)
            m_A[j] = 1.0;
        double f = 0.0;
        for (unsigned int j = 0; j <= getOrder(); j++)
        {
            f += hsteps->get(j);
            double a = 1.0;
            for (unsigned int i = 0; i < getOrder(); i++)
            {
                a *= f / hsteps->get(0);
                m_A[(i + 1) * rows + j] = a;
            }
        }
        /* Generate b */
        m_b[0] = 1;

        /* Solve x */
        int ret = m_linearSolver->solve(m_A, rows, m_x, m_b);
        assert(CSIM_OK(ret));
        (void)ret;
    }

    double IntegralPredictorGear::predict(const IntegralHistory *x, const IntegralHistory *hsteps)
    {
        double p = 0.0;
        for (unsigned int i = 0; i <= getOrder(); i++)
            p += m_x[i].real() * x->get(i + 1);
        return p;
    }

    double IntegralPredictorGear::getTruncErrorCoeff()
    {
        return 1.0;
    }
}
