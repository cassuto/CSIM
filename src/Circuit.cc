/**
 * @file Circuit
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

#include <algorithm>
#include <cassert>
#include <cstring>
#include "csim/utils/errors.h"
#include "csim/model/ModelBase.h"
#include "csim/internal/LinearSolver.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"

namespace csim
{

    Circuit::Circuit()
        : m_matrixRows(0),
          m_A(nullptr), m_x(nullptr), m_x_1(nullptr), m_z(nullptr), m_z_1(nullptr),
          m_linearSolver(LinearSolver::createInstance("gauss")),
          m_maxIterations(1000),
          m_VepsMax(0.0), m_VepsrMax(0.1),
          m_IepsMax(0.0), m_IepsrMax(0.1)
    {
        m_netlist = new Netlist(this);
    }

    Circuit::~Circuit()
    {
        delete[] m_A;
        delete[] m_x;
        delete[] m_x_1;
        delete[] m_z;
        delete[] m_z_1;
        delete m_linearSolver;
        delete m_netlist;
    }

    /**
     * @brief Create matrices
     * @param numNodes The number of nodes.
     * @param numVS The number of voltage sources, 0 if none.
     */
    void Circuit::createMatrix(unsigned int numNodes, unsigned int numVS)
    {
        assert(numNodes > 0);

        m_matrixRows = (numNodes + numVS);

        delete[] m_A;
        delete[] m_x;
        delete[] m_x_1;
        delete[] m_z;
        delete[] m_z_1;
        m_A = new Complex[m_matrixRows * m_matrixRows];
        m_x = new Complex[m_matrixRows];
        m_x_1 = new Complex[m_matrixRows];
        m_z = new Complex[m_matrixRows];
        m_z_1 = new Complex[m_matrixRows];
    }

    /* MNA matrices */
    const Complex &Circuit::getA(unsigned int row, unsigned int col) const
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        return m_A[row * m_matrixRows + col];
    }
    const Complex &Circuit::getX(unsigned int row) const
    {
        assert(row < m_matrixRows);
        return m_x[row];
    }
    const Complex &Circuit::getZ(unsigned int row) const
    {
        assert(row < m_matrixRows);
        return m_z[row];
    }

    void Circuit::setA(unsigned int row, unsigned int col, const Complex &val)
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        m_A[row * m_matrixRows + col] = val;
    }
    void Circuit::addA(unsigned int row, unsigned int col, const Complex &delta)
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        m_A[row * m_matrixRows + col] += delta;
    }
    void Circuit::setX(unsigned int row, const Complex &val)
    {
        assert(row < m_matrixRows);
        m_x[row] = val;
    }
    void Circuit::setZ(unsigned int row, const Complex &val)
    {
        assert(row < m_matrixRows);
        m_z[row] = val;
    }
    void Circuit::addZ(unsigned int row, const Complex &delta)
    {
        assert(row < m_matrixRows);
        m_z[row] += delta;
    }

    Complex Circuit::getNodeVolt(unsigned int node) const
    {
        assert(node < m_netlist->getNumNodes());
        return m_x[node];
    }

    Complex Circuit::getBranchCurrent(unsigned int vs) const
    {
        assert(vs < m_netlist->getNumVS());
        return m_x[m_netlist->getNumNodes() + vs];
    }

    const Complex *Circuit::getNodeVoltVector() const
    {
        return m_x;
    }
    const Complex *Circuit::getBranchCurrentVector() const
    {
        return m_x + m_netlist->getNumNodes();
    }

    int Circuit::initMNA(AnalyzerBase *analyzer)
    {
        createMatrix(m_netlist->getNumNodes(), m_netlist->getNumVS());
        analyzer->prepareMNA();
        return 0;
    }

    int Circuit::solveMNA(AnalyzerBase *analyzer)
    {
        int ret;
        int iteration = 0;
        do
        {
            memset(m_A, 0, sizeof(*m_A) * m_matrixRows * m_matrixRows);
            memset(m_z, 0, sizeof(*m_z) * m_matrixRows);

            UPDATE_RC(analyzer->iterateMNA());

            ret = m_linearSolver->solve(m_A, m_matrixRows, m_x, m_z);

            if (CSIM_OK(ret))
            {
                if (iteration)
                {
                    if (converged())
                        break;
                }
                /* save vector x and z */
                memcpy(m_x_1, m_x, sizeof(*m_x) * m_matrixRows);
                memcpy(m_z_1, m_z, sizeof(*m_z) * m_matrixRows);
                iteration++;
            }
        } while (iteration < m_maxIterations);

        if (iteration >= m_maxIterations)
            return CERR_MAX_ITERATION_EXCEEDED;

        return 0;
    }

    bool Circuit::converged()
    {
        /*
         * Check infinity norm || x - x_1 || and norm || z - z_1 ||
         */

        for (unsigned int i = 0; i < m_netlist->getNumNodes(); i++)
        {
            /* U */
            double Veps = std::abs(m_x[i] - m_x_1[i]);
            if (Veps > m_VepsMax + m_VepsrMax * std::abs(m_x[i]))
            {
                return false;
            }

            /* I */
            double Ieps = std::abs(m_z[i] - m_z_1[i]);
            if (Ieps > m_IepsMax + m_IepsrMax * std::abs(m_z[i]))
            {
                return false;
            }
        }

        unsigned int lowerb = m_netlist->getNumNodes();
        unsigned int upperb = m_netlist->getNumNodes() + m_netlist->getNumVS();
        for (unsigned int i = lowerb; i < upperb; i++)
        {
            /* J */
            double Ieps = std::abs(m_x[i] - m_x_1[i]);
            if (Ieps > m_IepsMax + m_IepsrMax * std::abs(m_x[i]))
            {
                return false;
            }

            /* E */
            double Veps = std::abs(m_z[i] - m_z_1[i]);
            if (Veps > m_VepsMax + m_VepsrMax * std::abs(m_z[i]))
            {
                return false;
            }
        }
        return true;
    }
}
