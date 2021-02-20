/**
 * @file Circuit
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
#include "csim/internal/LinearSolver.h"
#include "csim/internal/Circuit.h"

namespace csim
{

    Circuit::Circuit()
        : m_numNodes(0),
          m_numVS(0),
          m_matrixRows(0),
          m_A(nullptr), m_x(nullptr), m_z(nullptr),
          m_linearSolver(LinearSolver::createInstance("gauss"))
    {
    }

    Circuit::~Circuit()
    {
        delete[] m_A;
        delete[] m_x;
        delete[] m_z;
        delete m_linearSolver;
    }

    /**
     * @brief Create matrices
     * @param numNodes The number of nodes.
     * @param numVS The number of voltage sources, 0 if none.
     */
    void Circuit::createMatrix(int numNodes, int numVS)
    {
        assert(numNodes > 0);
        m_numNodes = numNodes;
        m_numVS = numVS;

        m_matrixRows = (m_numNodes + m_numVS);

        delete[] m_A;
        delete[] m_x;
        delete[] m_z;
        m_A = new Complex[m_matrixRows * m_matrixRows];
        m_x = new Complex[m_matrixRows];
        m_z = new Complex[m_matrixRows];
    }

    /* MNA matrices */
    const Complex &Circuit::getA(int row, int col) const
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        return m_A[row * m_matrixRows + col];
    }
    const Complex &Circuit::getX(int row) const
    {
        assert(row < m_matrixRows);
        return m_x[row];
    }
    const Complex &Circuit::getZ(int row) const
    {
        assert(row < m_matrixRows);
        return m_z[row];
    }

    void Circuit::setA(int row, int col, const Complex &val)
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        m_A[row * m_matrixRows + col] = val;
    }
    void Circuit::setX(int row, const Complex &val)
    {
        assert(row < m_matrixRows);
        m_x[row] = val;
    }
    void Circuit::setZ(int row, const Complex &val)
    {
        assert(row < m_matrixRows);
        m_z[row] = val;
    }

    int Circuit::solveMNA()
    {
        int ret = 0;

        ret = m_linearSolver->solve(m_A, m_matrixRows, m_x, m_z);
        
        return ret;
    }

}
