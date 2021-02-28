/**
 * @file LU decomposition based solver
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
 *  MERCHANTABILITY or FITNESS FOR m_bufA PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cassert>
#include <cstring>
#include <algorithm>
#include "csim/utils/errors.h"
#include "LinearSolverLU.h"

namespace csim
{
    LinearSolverLU::LinearSolverLU()
        : m_bufRow(nullptr),
          m_bufA(nullptr),
          m_rowLoc(nullptr), m_privot(nullptr),
          m_bufRows(0),
          m_lenRow(0)
    {
    }

    LinearSolverLU::~LinearSolverLU()
    {
        delete[] m_bufRow;
        delete[] m_bufA;
        delete[] m_rowLoc;
        delete[] m_privot;
    }

    int LinearSolverLU::solve(Complex *A, unsigned int n, Complex *x, Complex *B)
    {
        if (m_bufRows != n)
        {
            m_bufRows = n;
            m_lenRow = sizeof(Complex) * n;
            delete[] m_bufRow;
            delete[] m_bufA;
            delete[] m_rowLoc;
            delete[] m_privot;
            /* Memory to temporarily store matrix */
            m_bufRow = new Complex[m_bufRows];
            m_bufA = new Complex[m_bufRows * m_bufRows];
            /* LU matrix */
            m_rowLoc = new unsigned int[m_bufRows * m_bufRows];
            m_privot = new double[m_bufRows];
        }

        if (A)
        {
            /*
             * Do LU decomposition if matrix m_bufA has been changed.
             */
            memcpy(m_bufA, A, m_lenRow * n);

            /* Compute internal scale factors */
            for (unsigned int r = 0; r < n; r++)
            {
                double maxPivot = 0.0;
                for (unsigned int c = 0; c < n; c++)
                {
                    double t = std::abs(m_bufA[r * n + c]);
                    if (t > maxPivot)
                        maxPivot = t;
                }
                if (maxPivot <= 0.0)
                {
                    return CERR_SINGULAR_MATRIX;
                }
                m_privot[r] = 1.0 / maxPivot;
                m_rowLoc[r] = r;
            }

            Complex f;
            for (unsigned int c = 0; c < n; c++)
            {
                double maxPivot = 0.0;
                unsigned int pivot = c;

                /* Upper matrix */
                for (unsigned int r = 0; r < c; r++)
                {
                    f = m_bufA[r * n + c];
                    for (unsigned int k = 0; k < r; k++)
                        f -= m_bufA[r * n + k] * m_bufA[k * n + c];
                    m_bufA[r * n + c] = f / m_bufA[r * n + r];
                }
                /* Lower matrix */
                for (unsigned int r = c; r < n; r++)
                {
                    f = m_bufA[r * n + c];
                    for (unsigned int k = 0; k < c; k++)
                        f -= m_bufA[r * n + k] * m_bufA[k * n + c];
                    m_bufA[r * n + c] = f;
                    /* Find the max pivot */
                    double p = m_privot[r] * std::abs(f);
                    if (p > maxPivot)
                    {
                        maxPivot = p;
                        pivot = r;
                    }
                }

                if (maxPivot <= 0.0)
                {
                    if (optimizer())
                        UPDATE_RC(optimizer()->singularRow(m_bufA, c, n));
                    else
                        return CERR_SINGULAR_MATRIX;
                }

                /* Swap the matrix row if needed */
                if (c != pivot)
                {
                    /* m_bufA r[c] <-> r[prvot] */
                    memcpy(m_bufRow, &m_bufA[c * n], m_lenRow);
                    memcpy(&m_bufA[c * n], &m_bufA[pivot * n], m_lenRow);
                    memcpy(&m_bufA[pivot * n], m_bufRow, m_lenRow);

                    /* LU r[c] <-> r[prvot] */
                    std::swap(m_privot[c], m_privot[pivot]);
                    std::swap(m_rowLoc[c], m_rowLoc[pivot]);
                }
            }
            /* Now m_bufA is LU matrix */
        }

        /* LY = B solves Y */
        for (unsigned int i = 0; i < n; i++)
        {
            Complex f = 0.0;
            for (unsigned int c = 0; c < i; c++)
                f += m_bufA[i * n + c] * x[c];
            x[i] = (B[m_rowLoc[i]] - f) / m_bufA[i * n + i];
        }

        /* UX = Y solves X */
        for (long i = (long)n - 1; i >= 0; i--)
        {
            Complex f = 0.0;
            for (unsigned int c = i + 1; c < n; c++)
                f += m_bufA[i * n + c] * x[c];
            x[i] = x[i] - f;
        }

        return 0;
    }
}
