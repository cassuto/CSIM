/**
 * @file Complex type restricted to model SDK
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto
 *  This project is free edition{} you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation{} either      
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of         
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cassert>
#include <cstring>
#include "csim/internal/LinearSolver.h"

namespace csim
{
    LinearSolverGauss::LinearSolverGauss()
        : m_buf(nullptr),
          m_bufRows(0)
    {
    }

    LinearSolverGauss::~LinearSolverGauss()
    {
        delete[] m_buf;
    }

    int LinearSolverGauss::solve(Complex *A, int n, Complex *x, Complex *B)
    {
        if (m_bufRows != n)
        {
            m_bufRows = n;
            m_lenRow = sizeof(Complex) * n;
            delete[] m_buf;
            /* Memory to temporarily store a row */
            m_buf = new Complex[m_bufRows];
        }

        for (int i = 0; i < n; i++)
        {
            int pivot = i;
            double maxcolumn = 0.0;
            for (int r = i; r < n; r++)
            {
                if (abs(A[r * n + i]) > maxcolumn)
                {
                    maxcolumn = abs(A[r * n + i]);
                    pivot = r;
                }
            }
            if (i != pivot)
            {
                /* A r[i] <-> r[prvot] */
                memcpy(m_buf, &A[i * n], m_lenRow);
                memcpy(&A[i * n], &A[pivot * n], m_lenRow);
                memcpy(&A[pivot * n], m_buf, m_lenRow);

                /* B r[i] <-> r[prvot] */
                Complex t = B[i];
                B[i] = B[pivot];
                B[pivot] = t;
            }

            /* Gaussian elimination */
            for (int r = i + 1; r < n; r++)
            {
                Complex f = A[r * n + i] / A[i * n + i];
                for (int c = i + 1; c < n; c++)
                {
                    A[r * n + c] -= f * A[i * n + c];
                }
                B[r] -= f * B[i];
            }
        }

        for (int i = n - 1; i >= 0; i--)
        {
            Complex f = B[i];
            for (int c = i + 1; c < n; c++)
            {
                f -= A[i * n + c] * x[c];
            }
            x[i] = f / A[i * n + i];
        }

        return 0;
    }

}
