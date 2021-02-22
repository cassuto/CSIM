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
#include "csim/internal/Circuit.h"
#include <iostream>

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

    int Circuit::analyseDC()
    {
        UPDATE_RC(initMNA());
        for (auto &mif : m_netlist->models())
        {
            UPDATE_RC(mif.model->prepareDC());
        }

        return solveMNA(0);
    }

    int Circuit::analyseAC()
    {
        UPDATE_RC(initMNA());
        for (auto &mif : m_netlist->models())
        {
            UPDATE_RC(mif.model->prepareAC());
        }
        return solveMNA(1);
    }

    int Circuit::startTR()
    {
        UPDATE_RC(initMNA());
        for (auto &mif : m_netlist->models())
        {
            UPDATE_RC(mif.model->prepareTR());
        }
        return solveMNA(2);
    }

    Complex Circuit::getNodeVolt(unsigned int node)
    {
        assert(node < m_netlist->getNumNodes());
        return m_x[node];
    }

    int Circuit::initMNA()
    {
        createMatrix(m_netlist->getNumNodes(), m_netlist->getNumVS());
        return 0;
    }

    int Circuit::solveMNA(int analysis)
    {
        int ret;
        int iteration = 0;
        do
        {
            memset(m_A, 0, sizeof(*m_A) * m_matrixRows * m_matrixRows);
            memset(m_z, 0, sizeof(*m_z) * m_matrixRows);

            switch (analysis)
            {
            case 0:
                for (auto &mif : m_netlist->models())
                {
                    UPDATE_RC(mif.model->iterateDC());
                }
                break;
            case 1:
                for (auto &mif : m_netlist->models())
                {
                    UPDATE_RC(mif.model->iterateAC());
                }
                break;
            case 2:
                for (auto &mif : m_netlist->models())
                {
                    UPDATE_RC(mif.model->iterateTR());
                }
                break;
            default:
                assert(0);
            }
            
            /*std::cout << "A=\n";
            for (int i = 0; i < m_matrixRows; ++i)
            {
                for (int j = 0; j < m_matrixRows; ++j)
                {
                    std::cout << m_A[i * m_matrixRows + j].real() << " ";
                }
                std::cout << "\n";
            }
            std::cout << "z=\n";
            for (int i = 0; i < m_matrixRows; ++i)
            {
                std::cout << m_z[i].real() << "\n";
            }
            std::cout << "---\n";*/

            ret = m_linearSolver->solve(m_A, m_matrixRows, m_x, m_z);

            /*std::cout << "x=\n";
            for (int i = 0; i < m_matrixRows; ++i)
            {
                std::cout << m_x[i].real() << "\n";
            }
            std::cout << "===\n";*/

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
