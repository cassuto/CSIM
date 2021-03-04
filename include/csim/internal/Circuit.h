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

#ifndef CSIM_CIRCUIT_H_
#define CSIM_CIRCUIT_H_

#include <vector>
#include <set>
#include "csim/internal/Complex.h"
#include "csim/internal/IntegralCorrector.h"

#define ENABLE_STAT

namespace csimModel
{
    class ModelBase;
}

namespace csim
{

    class LinearSolver;
    class MNAOptimizer;
    class Netlist;
    class AnalyzerBase;
    class IntegralCorrector;
    class IntegralPredictor;

    class Circuit
    {
    public:
        Circuit();
        ~Circuit();

        /* MNA matrices */
        const Complex &getA(unsigned int row, unsigned int col) const;
        const Complex &getX(unsigned int row) const;
        const Complex &getZ(unsigned int row) const;

        void addA(unsigned int row, unsigned int col, const Complex &delta);
        void addX(unsigned int row, const Complex &delta);
        void addZ(unsigned int row, const Complex &delta);

        void registerIntegralU(unsigned int node);
        void registerIntegralJ(unsigned int VS);

        const std::vector<unsigned int> &getIntegralU() const
        {
            return m_integralU;
        }
        const std::vector<unsigned int> &getIntegralJ() const
        {
            return m_integralJ;
        }

        inline Netlist *netlist() const
        {
            return m_netlist;
        }
        inline IntegralCorrector *corrector()
        {
            return m_corrector;
        }

        int initMNA(AnalyzerBase *analyzer);
        int solveMNA(AnalyzerBase *analyzer);
        int stepMNA(AnalyzerBase *analyzer);

#if defined(ENABLE_STAT)
        void resetStat();
        void stat();
#endif

        inline double getDataPointTime() const
        {
            return m_tTime;
        }
        inline double getIntegralTime() const
        {
            return m_tTime + m_hsteps->get(0);
        }
        inline void setTmaxStep(double step)
        {
            m_tMaxStep = step;
        }

        Complex getNodeVolt(unsigned int node) const;
        Complex getBranchCurrent(unsigned int vs) const;
        const Complex *getNodeVoltVector() const;
        const Complex *getBranchCurrentVector() const;

    private:
        void createMatrix(unsigned int numNodes, unsigned int numVS);
        bool converged();
        double adaptStep();

    private:
        unsigned int m_matrixRows;
        Complex *m_A, *m_x, *m_x_1, *m_z, *m_z_1;
        LinearSolver *m_linearSolver;
        MNAOptimizer *m_MNAOptimizer;
        Netlist *m_netlist;
        unsigned int m_maxIterations, maxIntegralIterations;
        double m_VepsMax, m_VepsrMax;
        double m_IepsMax, m_IepsrMax;
        double m_Gmin;
        IntegralPredictor *m_predictor;
        IntegralCorrector *m_corrector;
        IntegralHistory *m_hsteps;
        IntegralHistory *m_hPredictorX;
        unsigned int m_tOrder;
        double m_tMaxStep, m_tMinStep;
        double m_tEpsMax, m_tEpsrMax;
        double m_tTOEF;
        double m_tTime;
        std::set<unsigned int> m_setIntegralU, m_setIntegralJ;
        std::vector<unsigned int> m_integralU, m_integralJ;
#if defined(ENABLE_STAT)
        unsigned long long m_statNumStepChanges, m_statNumNonlinearIters, m_sumNumIntegralIters;
        double m_statTotalStep;
        unsigned long long m_statNumMinSteps;
#endif
    };

}

#endif // CSIM_CIRCUIT_H_
