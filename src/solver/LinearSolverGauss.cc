/**
 * @file Gauss elimination based solver
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
#include "LinearSolverGauss.h"

namespace csim
{
    LinearSolverGauss::LinearSolverGauss()
        : m_bufRow(nullptr),
          m_bufA(nullptr),
          m_bufRows(0),
          m_lenRow(0)
    {
    }

    LinearSolverGauss::~LinearSolverGauss()
    {
        delete[] m_bufRow;
        delete[] m_bufA;
    }

    int LinearSolverGauss::solve(Complex *A, unsigned int n, Complex *x, Complex *B)
    {
        if (m_bufRows != n)
        {
            m_bufRows = n;
            m_lenRow = sizeof(Complex) * n;
            delete[] m_bufRow;
            delete[] m_bufA;
            /* Memory to temporarily store matrix */
            m_bufRow = new Complex[m_bufRows];
            m_bufA = new Complex[m_bufRows * m_bufRows];
        }

        /* Copy the matrix only if it changes (!= nullptr) */
        if (A)
            memcpy(m_bufA, A, sizeof(*A) * n * n);

        for (unsigned int i = 0; i < n; i++)
        {
            unsigned int pivot = i;
            double maxcolumn = 0.0;
            for (unsigned int r = i; r < n; r++)
            {
                if (std::abs(m_bufA[r * n + i]) > maxcolumn)
                {
                    maxcolumn = std::abs(m_bufA[r * n + i]);
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
                Complex t = B[i];
                B[i] = B[pivot];
                B[pivot] = t;
            }

            if (maxcolumn <= 0.0)
            {
                if (optimizer())
                    UPDATE_RC(optimizer()->singularRow(m_bufA, i, n));
                else
                    return CERR_SINGULAR_MATRIX;
            }

            /* Gaussian elimination */
            for (unsigned int r = i + 1; r < n; r++)
            {
                Complex f = m_bufA[r * n + i] / m_bufA[i * n + i];
                for (unsigned int c = i + 1; c < n; c++)
                {
                    m_bufA[r * n + c] -= f * m_bufA[i * n + c];
                }
                B[r] -= f * B[i];
            }
        }

        for (long i = (long)n - 1; i >= 0; i--)
        {
            Complex f = 0.0;
            for (unsigned int c = i + 1; c < n; c++)
            {
                f += m_bufA[i * n + c] * x[c];
            }
            x[i] = (B[i] - f) / m_bufA[i * n + i];
        }

        return 0;
    }

}
