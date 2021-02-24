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
#include "LinearSolverGauss.h"

namespace csim
{
    LinearSolverGauss::LinearSolverGauss()
        : m_bufRow(nullptr),
          m_bufA(nullptr), m_bufB(nullptr),
          m_bufRows(0)
    {
    }

    LinearSolverGauss::~LinearSolverGauss()
    {
        delete[] m_bufRow;
        delete[] m_bufA;
        delete[] m_bufB;
    }

    int LinearSolverGauss::solve(const Complex *A, int n, Complex *x, const Complex *B)
    {
        if (m_bufRows != n)
        {
            m_bufRows = n;
            m_lenRow = sizeof(Complex) * n;
            delete[] m_bufRow;
            delete[] m_bufA;
            delete[] m_bufB;
            /* Memory to temporarily store matrix */
            m_bufRow = new Complex[m_bufRows];
            m_bufA = new Complex[m_bufRows * m_bufRows];
            m_bufB = new Complex[m_bufRows];
        }

        memcpy(m_bufA, A, sizeof(*A) * n * n);
        memcpy(m_bufB, B, sizeof(*B) * n);

        for (int i = 0; i < n; i++)
        {
            int pivot = i;
            double maxcolumn = 0.0;
            for (int r = i; r < n; r++)
            {
                if (abs(m_bufA[r * n + i]) > maxcolumn)
                {
                    maxcolumn = abs(m_bufA[r * n + i]);
                    pivot = r;
                }
            }
            if (i != pivot)
            {
                /* A r[i] <-> r[prvot] */
                memcpy(m_bufRow, &m_bufA[i * n], m_lenRow);
                memcpy(&m_bufA[i * n], &m_bufA[pivot * n], m_lenRow);
                memcpy(&m_bufA[pivot * n], m_bufRow, m_lenRow);

                /* B r[i] <-> r[prvot] */
                Complex t = m_bufB[i];
                m_bufB[i] = m_bufB[pivot];
                m_bufB[pivot] = t;
            }

            /* Gaussian elimination */
            for (int r = i + 1; r < n; r++)
            {
                Complex f = m_bufA[r * n + i] / m_bufA[i * n + i];
                for (int c = i + 1; c < n; c++)
                {
                    m_bufA[r * n + c] -= f * m_bufA[i * n + c];
                }
                m_bufB[r] -= f * m_bufB[i];
            }
        }

        for (int i = n - 1; i >= 0; i--)
        {
            Complex f = m_bufB[i];
            for (int c = i + 1; c < n; c++)
            {
                f -= m_bufA[i * n + c] * x[c];
            }
            x[i] = f / m_bufA[i * n + i];
            if (!std::isfinite(x[i].real()) || !std::isfinite(x[i].imag()))
            {
                return CERR_SINGULAR_MATRIX;
            }
        }

        return 0;
    }

}
