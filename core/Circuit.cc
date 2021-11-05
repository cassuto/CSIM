/**
 * @file Circuit
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
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
#include <cfloat>
#include "csim/utils/errors.h"
#include "csim/model/ModelBase.h"
#include "csim/model/Environment.h"
#include "csim/internal/LinearSolver.h"
#include "csim/internal/GminOptimizer.h"
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
          m_maxIterations(100),
          maxIntegralIterations(100),
          m_predictor(IntegralPredictor::createInstance("gear")),
          m_corrector(IntegralCorrector::createInstance("gear")),
          m_hsteps(new IntegralHistory()),
          m_hPredictorX(nullptr),
          m_tOrder(4),
          m_tMaxStep(1e-3), m_tMinStep(1e-18),
          m_tTime(0.0),
          m_allowRegister(false),
          m_firstIntegralStep(true)
    {
        m_environment = new csimModel::Environment(this);
        m_netlist = new Netlist(this);
        m_GminOptimizer = new GminOptimizer(m_netlist);
        m_GminOptimizer->enableGmin(true);
        m_linearSolver->setOptimizer(m_GminOptimizer);
#if defined(ENABLE_SPICE_COMPATIBLE)
        m_spiceCompatible = new csimModel::SPICE_Compatible();
#endif
#if defined(ENABLE_STAT)
        resetStat();
#endif
    }

    Circuit::~Circuit()
    {
        delete[] m_A;
        delete[] m_x;
        delete[] m_x_1;
        delete[] m_z;
        delete[] m_z_1;
        delete m_GminOptimizer;
        delete m_linearSolver;
        delete m_netlist;
        delete m_predictor;
        delete m_corrector;
        delete[] m_hPredictorX;
        delete m_hsteps;
        delete m_environment;
        delete m_spiceCompatible;
    }

    /**
     * @brief Create matrices
     * @param numNodes The number of nodes.
     * @param numBranch The number of current branches, 0 if none.
     */
    void Circuit::createMatrix(unsigned int numNodes, unsigned int numBranch)
    {
        assert(numNodes > 0);

        m_matrixRows = (numNodes + numBranch);

        delete[] m_A;
        delete[] m_x;
        delete[] m_x_1;
        delete[] m_z;
        delete[] m_z_1;
        m_A = new csimModel::MComplex[m_matrixRows * m_matrixRows];
        m_x = new csimModel::MComplex[m_matrixRows];
        m_x_1 = new csimModel::MComplex[m_matrixRows];
        m_z = new csimModel::MComplex[m_matrixRows];
        m_z_1 = new csimModel::MComplex[m_matrixRows];
    }

    int Circuit::initMNA()
    {
        createMatrix(m_netlist->getNumNodes(), m_netlist->getNumBranches());
#if defined(ENABLE_STAT)
        resetStat();
#endif
        return 0;
    }

    int Circuit::prepareMNA(AnalyzerBase *analyzer)
    {
        m_GminOptimizer->setGmin(m_environment->m_Gmin);
        m_GminOptimizer->reset();
        m_tTime = 0.0;
        m_hsteps->setInitial(m_tMinStep); /* initial step */
        UPDATE_RC(m_predictor->setOrder(m_tOrder, m_hsteps));
        UPDATE_RC(m_corrector->setOrder(m_tOrder, m_hsteps));
        m_firstIntegralStep = true;

        /* Phase 1: Clear integral nodes and branches. */
        m_setIntegralU.clear(); /* for duplicate removal */
        m_setIntegralJ.clear(); /* for duplicate removal */
        m_adoptStepModels.clear();
        m_allowRegister = true;

        UPDATE_RC(analyzer->prepareMNA());

        /* Phase 2: Collect integral nodes and branches from prepareMNA(). */
        m_allowRegister = false;
        m_integralU.clear();
        m_integralJ.clear();
        for (auto &node : m_setIntegralU)
            m_integralU.push_back(node);
        for (auto &vs : m_setIntegralJ)
            m_integralJ.push_back(vs);

        delete[] m_hPredictorX;
        m_numIntegralNodes = m_integralU.size() + m_integralJ.size();
        m_hPredictorX = new IntegralHistory[m_matrixRows];

        /* Apply previous operation points for integral nodes */
        for (size_t k = 0; k < m_matrixRows; ++k)
        {
            m_hPredictorX[k].set(0, m_x[k].real());
        }

        /* Setup property models */
        UPDATE_RC(setupMdl());

        /* Load environment temperature */
        UPDATE_RC(loadTempature(m_environment->getNormTemp()));
        return 0;
    }

    int Circuit::solveMNA(AnalyzerBase *analyzer)
    {
        int rc;
        bool converged, cont = true;
        unsigned int iteration = 0;
        do
        {
            std::memset((void *)m_A, 0, sizeof(*m_A) * m_matrixRows * m_matrixRows);
            std::memset((void *)m_z, 0, sizeof(*m_z) * m_matrixRows);

            converged = false;
            rc = analyzer->iterateMNA();
            switch (rc)
            {
            case 0:
            case CERR_NON_CONVERGENCE:
                break;
            default:
                return rc; /* Unhandled errors */
            }

            if (m_netlist->hasGroundNode())
            {
                unsigned int ngnd = m_netlist->getGroundNode();
                m_A[ngnd * m_matrixRows + ngnd] = 0.0;
            }

            UPDATE_RC(m_linearSolver->solve(m_A, m_matrixRows, m_x, m_z));

            if (iteration)
                converged = isConverged();
#if defined(ENABLE_SPICE_COMPATIBLE)
            switch (m_spiceCompatible->upateStateMachine(converged))
            {
            case csimModel::SPICE_Compatible::Result::ContinueIter:
                break;
            case csimModel::SPICE_Compatible::Result::BreakIter:
                cont = false;
                break;
            }
#else
            cont = !converged;
#endif

            /* save vector x and z */
            memcpy((void *)m_x_1, m_x, sizeof(*m_x) * m_matrixRows);
            memcpy((void *)m_z_1, m_z, sizeof(*m_z) * m_matrixRows);

            iteration++;
        } while (cont && (iteration < m_maxIterations));

#if defined(ENABLE_STAT)
        m_statNumNonlinearIters += iteration;
#endif

        if (iteration >= m_maxIterations)
            return CERR_MAX_ITERATION_EXCEEDED;

        return 0;
    }

    bool Circuit::isConverged()
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
            if (Veps > m_environment->m_VepsMax + m_environment->m_VepsrMax * maxX)
            {
                return false;
            }

            /* I */
            double Ieps = std::abs(m_z[i] - m_z_1[i]);
            if (Ieps > m_environment->m_IepsMax + m_environment->m_IepsrMax * maxZ)
            {
                return false;
            }
        }

        unsigned int lowerb = m_netlist->getNumNodes();
        unsigned int upperb = m_netlist->getNumNodes() + m_netlist->getNumBranches();
        for (unsigned int i = lowerb; i < upperb; i++)
        {
            double maxX = std::max(std::abs(m_x[i]), std::abs(m_x_1[i]));
            double maxZ = std::max(std::abs(m_z[i]), std::abs(m_z_1[i]));

            /* J */
            double Ieps = std::abs(m_x[i] - m_x_1[i]);
            if (Ieps > m_environment->m_IepsMax + m_environment->m_IepsrMax * maxX)
            {
                return false;
            }

            /* E */
            double Veps = std::abs(m_z[i] - m_z_1[i]);
            if (Veps > m_environment->m_VepsMax + m_environment->m_VepsrMax * maxZ)
            {
                return false;
            }
        }

        /*
         * Call model-specific convergence check
         */
        for (auto &mif : m_netlist->models())
        {
            if (!mif.model->checkConvergence())
                return false;
        }
        return true;
    }

    int Circuit::saveOP()
    {
#if defined(ENABLE_SPICE_COMPATIBLE)
        m_spiceCompatible->setFlagsSmallSig();
#endif
        for (auto &mif : m_netlist->models())
        {
            UPDATE_RC(mif.model->saveOP());
        }
        return 0;
    }

    int Circuit::setupMdl()
    {
        for (auto &mif : m_netlist->mdls())
        {
            UPDATE_RC(mif.mdl->setup(m_environment));
        }
        return 0;
    }

    int Circuit::loadTempature(double temp)
    {
        m_environment->setTemp(temp);
        for (auto &mif : m_netlist->models())
        {
            UPDATE_RC(mif.model->loadTempature());
        }
        return 0;
    }

    int Circuit::stepIntegral(AnalyzerBase *analyzer)
    {
        double nstep = m_hsteps->get(0);
        bool stepChanged = false;

        for (;;)
        {
            /* Predictor for the registered integral nodes */
            for (unsigned int i = 0; i < m_matrixRows; i++)
            {
                m_x[i] = m_predictor->predict(&m_hPredictorX[i], m_hsteps);
                m_hPredictorX[i].set(0, m_x[i].real());
            }

            if (stepChanged)
            {
                /* Inform the models that the step size has been changed */
                for (auto &mif : m_netlist->models())
                {
                    mif.model->stepChangedTR(getTimeInStep(), m_hsteps->get(0));
                }
                stepChanged = false;
#if defined(ENABLE_STAT)
                m_statNumStepChanges++;
#endif
            }

            /* Run linear and non-linear iteration with integral corrector */
            UPDATE_RC(solveMNA(analyzer));

#if defined(ENABLE_SPICE_COMPATIBLE)
            m_spiceCompatible->setFlagsTRPred();
#endif

            if (m_firstIntegralStep)
            {
                m_firstIntegralStep = false;
                continue; /* No time step adaption */
            }

            /* Check the truncation error and update step size */
            nstep = adaptStep();
            if (nstep < m_hsteps->get(0))
            {
#if defined(ENABLE_STAT)
                if (nstep <= m_tMinStep)
                    m_statNumMinSteps++;
#endif
                m_hsteps->set(0, nstep);
                UPDATE_RC(m_predictor->setStep(m_hsteps));
                UPDATE_RC(m_corrector->setStep(m_hsteps));
                stepChanged = true;
                continue;
            }

            break;
        }

#if defined(ENABLE_STAT)
        m_statTotalStep += m_hsteps->get(0);
        m_sumNumIntegralIters++;
#endif

        /* Go to the next time step */
        m_tTime += m_hsteps->get(0);
        m_hsteps->push();
        m_hsteps->set(0, nstep);
        UPDATE_RC(m_predictor->setStep(m_hsteps));
        UPDATE_RC(m_corrector->setStep(m_hsteps));
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

        /* Compute the registered integral voltage/current nodes */
        for (size_t i = 0; i < m_numIntegralNodes; i++)
        {
            unsigned int row;
            double tEpsMax, tEpsrMax;
            if (i < m_integralU.size())
            {
                /* is a voltage node */
                row = m_integralU[i];
                tEpsMax = m_environment->m_VepsMax;
                tEpsrMax = m_environment->m_VepsrMax;
            }
            else
            {
                /* is a current branch */
                row = m_integralJ[i - m_integralU.size()] + m_netlist->getNumNodes();
                tEpsMax = m_environment->m_IepsMax;
                tEpsrMax = m_environment->m_IepsrMax;
            }

            double x = m_x[row].real();
            double x_1 = m_hPredictorX[row].get(0);

            double epsilon = x - x_1;
            if (!std::isfinite(epsilon) || epsilon == 0.0)
                continue; /* Non-convergence or invariant */

            double elimit = tEpsMax + tEpsrMax * std::max(std::abs(x), std::abs(x_1));

            double p = m_predictor->getTruncErrorCoeff();
            double c = m_corrector->getTruncErrorCoeff();
            double q = m_environment->m_tTOEF * c * epsilon / (p * elimit - c * elimit);
            double t = m_hsteps->get(0) * std::pow(1.0 / std::abs(q), 1.0 / (1 + m_corrector->getOrder()));
            nstep = std::min(nstep, t);
        }

        /* Call the model-specific adaptive step size method */
        size_t N = m_adoptStepModels.size();
        for (size_t i = 0; i < N; i++)
        {
            m_adoptStepModels[i]->adaptStep(&nstep);
        }

        /*
         * Limit the growth rate of step to 2
         * To avoid the singular matrix in the calculation of integrator coefficient due to the large change of step.
         */
        nstep = std::min((nstep > (2 - 0.1) * m_hsteps->get(0)) ? 2 * m_hsteps->get(0) : m_hsteps->get(0), nstep);

        nstep = std::max(nstep, m_tMinStep);
        return std::min(nstep, m_tMaxStep);
    }

    /* Functions named `register*` must be called in prepare() callback */

    void Circuit::registerAdoptStepCallback(csimModel::ModelBase *model)
    {
        assert(m_allowRegister);
        m_adoptStepModels.push_back(model);
    }
    void Circuit::registerIntegralU(unsigned int node)
    {
        assert(m_allowRegister);
        m_setIntegralU.insert(node);
    }
    void Circuit::registerIntegralJ(unsigned int VS)
    {
        assert(m_allowRegister);
        m_setIntegralJ.insert(VS);
    }

    unsigned int Circuit::getIntegralNode(size_t idx)
    {
        unsigned int i;
        if (idx < m_integralU.size())
            i = m_integralU[idx];
        else
            i = m_integralJ[idx - m_integralU.size()] + m_netlist->getNumNodes();
        return i;
    }

#if defined(ENABLE_STAT)
    void Circuit::resetStat()
    {
        m_statNumStepChanges = 0;
        m_statNumNonlinearIters = 0;
        m_sumNumIntegralIters = 0;
        m_statTotalStep = 0.0;
        m_statNumMinSteps = 0;
    }

    void Circuit::stat()
    {
        std::cout << "Number of step size changes = " << m_statNumStepChanges << "\n";
        std::cout << "Average step size = " << m_statTotalStep / m_sumNumIntegralIters << "\n";
        std::cout << "Number of min steps = " << m_statNumMinSteps << "\n";
        std::cout << "Number of non-linear iterations = " << m_statNumNonlinearIters << "\n";
    }
#endif
}
