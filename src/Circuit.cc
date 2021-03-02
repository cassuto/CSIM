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

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cfloat>
#include "csim/utils/errors.h"
#include "csim/model/ModelBase.h"
#include "csim/internal/LinearSolver.h"
#include "csim/internal/MNAOptimizer.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/IntegralPredictor.h"
#include "csim/internal/IntegralCorrector.h"
#include "csim/internal/Circuit.h"

namespace csim
{

    Circuit::Circuit()
        : m_matrixRows(0),
          m_A(nullptr), m_x(nullptr), m_x_1(nullptr), m_z(nullptr), m_z_1(nullptr),
          m_linearSolver(LinearSolver::createInstance("LU")),
          m_maxIterations(1000),
          maxIntegralIterations(100),
          m_VepsMax(1e-6), m_VepsrMax(0.001),
          m_IepsMax(1e-12), m_IepsrMax(0.001),
          m_Gmin(1e-12),
          m_predictor(IntegralPredictor::createInstance("euler")),
          m_corrector(IntegralCorrector::createInstance("gear")),
          m_hsteps(new IntegralHistory()),
          m_hPredictorX(nullptr),
          m_tOrder(4),
          m_tMaxStep(1e-3), m_tMinStep(1e-16),
          m_tEpsMax(1e-6), m_tEpsrMax(0.001),
          m_tTOEF(1.0),
          m_tTime(0.0)
    {
        m_netlist = new Netlist(this);
        m_MNAOptimizer = new MNAOptimizer(m_netlist);
        m_MNAOptimizer->enableGmin(true);
        m_linearSolver->setOptimizer(m_MNAOptimizer);
    }

    Circuit::~Circuit()
    {
        delete[] m_A;
        delete[] m_x;
        delete[] m_x_1;
        delete[] m_z;
        delete[] m_z_1;
        delete m_MNAOptimizer;
        delete m_linearSolver;
        delete m_netlist;
        delete m_predictor;
        delete m_corrector;
        delete m_hsteps;
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

    void Circuit::addA(unsigned int row, unsigned int col, const Complex &delta)
    {
        assert(row < m_matrixRows && col < m_matrixRows);
        m_A[row * m_matrixRows + col] += delta;
    }
    void Circuit::addX(unsigned int row, const Complex &delta)
    {
        assert(row < m_matrixRows);
        m_x[row] += delta;
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

    void Circuit::registerIntegralU(unsigned int node)
    {
        m_setIntegralU.insert(node);
    }
    void Circuit::registerIntegralJ(unsigned int VS)
    {
        m_setIntegralJ.insert(VS);
    }

    int Circuit::initMNA(AnalyzerBase *analyzer)
    {
        createMatrix(m_netlist->getNumNodes(), m_netlist->getNumVS());
        m_MNAOptimizer->setGmin(m_Gmin);
        m_MNAOptimizer->reset();
        m_hPredictorX = new IntegralHistory[m_matrixRows];
        m_tTime = 0.0;
        double step = m_tMaxStep / 100.0; /* initial step */
        m_hsteps->setInitial(step);
        m_corrector->setStep(step);
        m_corrector->setOrder(m_tOrder);

        /* Phase 1: Clear integral nodes and branches. */
        m_setIntegralU.clear();
        m_setIntegralJ.clear();

        analyzer->prepareMNA();

        /* Phase 2: Collect integral nodes and branches from prepareMNA(). */
        m_integralU.clear();
        m_integralJ.clear();
        for (auto &node : m_setIntegralU)
            m_integralU.push_back(node);
        for (auto &vs : m_setIntegralJ)
            m_integralJ.push_back(vs);
        return 0;
    }

    int Circuit::solveMNA(AnalyzerBase *analyzer)
    {
        unsigned int iteration = 0;
        do
        {
            memset(m_A, 0, sizeof(*m_A) * m_matrixRows * m_matrixRows);
            memset(m_z, 0, sizeof(*m_z) * m_matrixRows);

            UPDATE_RC(analyzer->iterateMNA());

            if (m_netlist->hasGroundNode())
            {
                unsigned int ngnd = m_netlist->getGroundNode();
                m_A[ngnd * m_matrixRows + ngnd] = 0.0;
            }

            UPDATE_RC(m_linearSolver->solve(m_A, m_matrixRows, m_x, m_z));

            if (iteration)
            {
                if (converged())
                    break;
            }
            /* save vector x and z */
            memcpy(m_x_1, m_x, sizeof(*m_x) * m_matrixRows);
            memcpy(m_z_1, m_z, sizeof(*m_z) * m_matrixRows);

            iteration++;
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
            double maxX = std::max(std::abs(m_x[i]), std::abs(m_x_1[i]));
            double maxZ = std::max(std::abs(m_z[i]), std::abs(m_z_1[i]));

            /* U */
            double Veps = std::abs(m_x[i] - m_x_1[i]);
            if (Veps > m_VepsMax + m_VepsrMax * maxX)
            {
                return false;
            }

            /* I */
            double Ieps = std::abs(m_z[i] - m_z_1[i]);
            if (Ieps > m_IepsMax + m_IepsrMax * maxZ)
            {
                return false;
            }
        }

        unsigned int lowerb = m_netlist->getNumNodes();
        unsigned int upperb = m_netlist->getNumNodes() + m_netlist->getNumVS();
        for (unsigned int i = lowerb; i < upperb; i++)
        {
            double maxX = std::max(std::abs(m_x[i]), std::abs(m_x_1[i]));
            double maxZ = std::max(std::abs(m_z[i]), std::abs(m_z_1[i]));

            /* J */
            double Ieps = std::abs(m_x[i] - m_x_1[i]);
            if (Ieps > m_IepsMax + m_IepsrMax * maxX)
            {
                return false;
            }

            /* E */
            double Veps = std::abs(m_z[i] - m_z_1[i]);
            if (Veps > m_VepsMax + m_VepsrMax * maxZ)
            {
                return false;
            }
        }
        return true;
    }

    int Circuit::stepMNA(AnalyzerBase *analyzer)
    {
        double nstep = m_hsteps->get(0);
        for (;;)
        {
            /* Run integral predictor */
            for (unsigned int i = 0; i < m_matrixRows; ++i)
            {
                m_x[i] = m_predictor->predict(&m_hPredictorX[i], m_hsteps);
                m_hPredictorX[i].set(0, m_x[i].real());
            }

            /* Run linear and non-linear iteration with integral corrector */
            UPDATE_RC(solveMNA(analyzer));

            /* Check the truncation error and update step size */
            nstep = adaptStep();
            if (nstep > 0.9 * m_hsteps->get(0))
                break;
            if (nstep < m_hsteps->get(0))
            {
                //std::cout << "restep="<<m_hsteps->get(0)<<"-->"<<nstep<<"\n";
                m_hsteps->set(0, nstep);
                m_corrector->setStep(nstep);
                continue;
            }

            break;
        }

        //std::cout<<nstep<<"----\n";
        //std::cout << m_tTime << "\n";

        /* Go to the next time step */
        m_tTime += m_hsteps->get(0);
        m_hsteps->push();
        m_hsteps->set(0, nstep);
        m_corrector->setStep(nstep);
        for (auto &mif : m_netlist->models())
        {
            unsigned int numIntegrators = mif.model->getNumIntegrators();
            for (unsigned int i = 0; i < numIntegrators; ++i)
            {
                mif.model->getIntegratorX(i)->push();
                mif.model->getIntegratorY(i)->push();
            }
        }
        for (unsigned int i = 0; i < m_matrixRows; ++i)
        {
            m_hPredictorX[i].set(0, m_x[i].real());
            m_hPredictorX[i].push();
        }
        return 0;
    }

    double Circuit::adaptStep()
    {
        double nstep = DBL_MAX;
        size_t N = m_integralU.size() + m_integralJ.size();
        for (size_t i = 0; i < N; i++)
        {
            unsigned int row;
            if (i < m_integralU.size())
                row = m_integralU[i];
            else
                row = m_integralJ[i - m_integralJ.size()] + m_netlist->getNumNodes();

            double x = m_x[row].real();
            double x_1 = m_hPredictorX[row].get(0);

            double epsilon = x - x_1;
            if (!std::isfinite(epsilon) || epsilon == 0.0)
                continue; /* Non-convergence or invariant */

            double errlimit = m_tEpsMax + m_tEpsrMax * std::max(std::abs(x), std::abs(x_1));
            double p = m_predictor->getTruncErrorCoeff();
            double c = m_corrector->getTruncErrorCoeff();
            double q = m_tTOEF * c * epsilon / (p * errlimit - c * errlimit);
            double t = m_hsteps->get(0) * std::pow(1.0 / std::abs(q), 1.0 / (1 + m_corrector->getOrder()));

            nstep = std::min(nstep, t);
        }
        //std::cout<<nstep<<"\n";
        nstep = std::max(nstep, m_tMinStep);
        return std::min(nstep, m_tMaxStep);
    }
}
