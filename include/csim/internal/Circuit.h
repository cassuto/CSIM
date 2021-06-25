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

#ifndef CSIM_CIRCUIT_H_
#define CSIM_CIRCUIT_H_

#include <vector>
#include <set>
#include <cassert>
#include "csim/model/Types.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/IntegralCorrector.h"

#define ENABLE_STAT

#include "csim/model/compact/SPICE-Compatible.h"

namespace csimModel
{
    class ModelBase;
    class Environment;
}

namespace csim
{

    class LinearSolver;
    class GminOptimizer;
    class Netlist;
    class AnalyzerBase;
    class IntegralCorrector;
    class IntegralPredictor;
    class SPICE_Compatible;

    class Circuit
    {
    public:
        Circuit();
        ~Circuit();

        /* MNA matrices */
        inline const csimModel::MComplex &getA(unsigned int row, unsigned int col) const
        {
            assert(row < m_matrixRows && col < m_matrixRows);
            return m_A[row * m_matrixRows + col];
        }
        inline const csimModel::MComplex &getX(unsigned int row) const
        {
            assert(row < m_matrixRows);
            return m_x[row];
        }
        inline const csimModel::MComplex &getPrevX(unsigned int row) const
        {
            assert(row < m_matrixRows);
            return m_x_1[row];
        }
        inline const csimModel::MComplex &getZ(unsigned int row) const
        {
            assert(row < m_matrixRows);
            return m_z[row];
        }

        inline void addA(unsigned int row, unsigned int col, const csimModel::MComplex &delta)
        {
            assert(row < m_matrixRows && col < m_matrixRows);
            m_A[row * m_matrixRows + col] += delta;
        }
        inline void addX(unsigned int row, const csimModel::MComplex &delta)
        {
            assert(row < m_matrixRows);
            m_x[row] += delta;
        }
        inline void addZ(unsigned int row, const csimModel::MComplex &delta)
        {
            assert(row < m_matrixRows);
            m_z[row] += delta;
        }

        inline double *getAPtr(unsigned int row, unsigned int col)
        {
            assert(row < m_matrixRows && col < m_matrixRows);
            return m_A[row * m_matrixRows + col].ptr();
        }
        inline double *getXPtr(unsigned int row)
        {
            assert(row < m_matrixRows);
            return m_x[row].ptr();
        }
        inline double *getZPtr(unsigned int row)
        {
            assert(row < m_matrixRows);
            return m_z[row].ptr();
        }

        inline csimModel::MComplex getNodeVolt(unsigned int node) const
        {
            assert(node < m_netlist->getNumNodes());
            return m_x[node];
        }

        inline csimModel::MComplex getBranchCurrent(unsigned int vs) const
        {
            assert(vs < m_netlist->getNumBranches());
            return m_x[m_netlist->getNumNodes() + vs];
        }

        inline const csimModel::MComplex *getNodeVoltVector() const
        {
            return m_x;
        }
        inline const csimModel::MComplex *getBranchCurrentVector() const
        {
            return m_x + m_netlist->getNumNodes();
        }

        inline Netlist *netlist() const
        {
            return m_netlist;
        }
        inline IntegralCorrector *corrector()
        {
            return m_corrector;
        }
#if defined(ENABLE_SPICE_COMPATIBLE)
        inline csimModel::SPICE_Compatible *spiceCompatible() const
        {
            return m_spiceCompatible;
        }
#endif
        inline csimModel::Environment *environment() const
        {
            return m_environment;
        }

        int initMNA();
        int prepareMNA(AnalyzerBase *analyzer);
        int solveMNA(AnalyzerBase *analyzer);
        int saveOP();
        int setupMdl();
        int loadTempature(double temp);
        int stepIntegral(AnalyzerBase *analyzer);

#if defined(ENABLE_STAT)
        void resetStat();
        void stat();
#endif

        inline double getTimeInStep() const
        {
            return m_tTime + m_hsteps->get(0);
        }
        inline double getTimeAfterStep() const
        {
            return m_tTime;
        }
        inline void setTmaxStep(double step)
        {
            m_tMaxStep = step;
        }
        inline const IntegralHistory *getTimestep() const
        {
            return m_hsteps;
        }

        void registerIntegralU(unsigned int node);
        void registerIntegralJ(unsigned int VS);
        unsigned int getIntegralNode(size_t idx);
        void registerAdoptStepCallback(csimModel::ModelBase *model);

    private:
        void createMatrix(unsigned int numNodes, unsigned int numBranches);
        bool isConverged();
        double adaptStep();

    private:
        csimModel::Environment *m_environment;
        unsigned int m_matrixRows;
        csimModel::MComplex *m_A, *m_x, *m_x_1, *m_z, *m_z_1;
        LinearSolver *m_linearSolver;
        GminOptimizer *m_GminOptimizer;
        Netlist *m_netlist;
        unsigned int m_maxIterations, maxIntegralIterations;
        IntegralPredictor *m_predictor;
        IntegralCorrector *m_corrector;
        IntegralHistory *m_hsteps;
        IntegralHistory *m_hPredictorX;
        unsigned int m_tOrder;
        double m_tMaxStep, m_tMinStep;
        double m_tTime;
        bool m_allowRegister;
        bool m_firstIntegralStep;
        std::set<unsigned int> m_setIntegralU, m_setIntegralJ;
        std::vector<unsigned int> m_integralU, m_integralJ;
        size_t m_numIntegralNodes;
#if defined(ENABLE_STAT)
        unsigned long long m_statNumStepChanges, m_statNumNonlinearIters, m_sumNumIntegralIters;
        double m_statTotalStep;
        unsigned long long m_statNumMinSteps;
#endif
        std::vector<csimModel::ModelBase *> m_adoptStepModels;
#if defined(ENABLE_SPICE_COMPATIBLE)
        csimModel::SPICE_Compatible *m_spiceCompatible;
#endif
    };

}

#endif // CSIM_CIRCUIT_H_
