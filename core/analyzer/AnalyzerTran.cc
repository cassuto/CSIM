/**
 * @file Transient analysis
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition; you can redistribute it and/or           
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation; either      
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <iostream>
#include "csim/model/ModelBase.h"
#include "csim/utils/errors.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "AnalyzerTran.h"

namespace csim
{

    AnalyzerTran::AnalyzerTran(Circuit *circuit)
        : AnalyzerBase(circuit),
          m_analyzeTrOP(false)
    {
        property().addProperty("tstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(.05), "Stop time", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("tstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-3), "Max time step", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }
    AnalyzerTran::~AnalyzerTran()
    {
    }

    int AnalyzerTran::analyze(Dataset *dataset)
    {
        /*
         * Format data set
         */
        dataset->clear();
        dataset->setName("Transient analysis");
        Variable &dtime = dataset->addIndependentVar("time");
        unsigned int N = getNumInterestNodes(), M = getNumInterestBranches();
        std::vector<Variable *> dvolt(N), dcurrent(M);
        for (unsigned int i = 0; i < N; ++i)
        {
            std::string varName = makeVarName("V", getInterestNode(i));
            dvolt[i] = &dataset->addDependentVar("voltage", varName);
            dvolt[i]->addIndependVar(dtime);
        }
        for (unsigned int i = 0; i < M; ++i)
        {
            std::string varName = makeVarName("I", getInterestBranch(i));
            dcurrent[i] = &dataset->addDependentVar("current", varName);
            dcurrent[i]->addIndependVar(dtime);
        }

        double tstop = property().getProperty("tstop").getDouble();
        double tstep = property().getProperty("tstep").getDouble();
        circuit()->setTmaxStep(tstep);

        double tTime = 0.0;
        UPDATE_RC(circuit()->initMNA());

        /*
         * Solve transient operating point
         */
        m_analyzeTrOP = true;
#if defined(ENABLE_SPICE_COMPATIBLE)
        UPDATE_RC(circuit()->spiceCompatible()->setFlagsTrOP());
#endif
        UPDATE_RC(circuit()->prepareMNA(this, Circuit::ANALYSIS_FLAG_NO_TIME_DOMAIN));
        UPDATE_RC(circuit()->solveMNA(this));
        m_analyzeTrOP = false;
        saveDataset(dvolt, dcurrent, dtime, tTime, N, M);

        /*
         * Transient analysis
         */
#if defined(ENABLE_SPICE_COMPATIBLE)
        UPDATE_RC(circuit()->spiceCompatible()->setFlagsTR());
#endif
        UPDATE_RC(circuit()->prepareMNA(this, 0));
        do
        {
            UPDATE_RC(circuit()->stepIntegral(this));
            tTime = circuit()->getTimeAfterStep();
            saveDataset(dvolt, dcurrent, dtime, tTime, N, M);
        } while (tTime < tstop);

#if defined(ENABLE_STAT)
        circuit()->stat();
#endif

        return 0;
    }

    void AnalyzerTran::saveDataset(std::vector<Variable *> &dvolt, std::vector<Variable *> &dcurrent, Variable &dtime, double tTime, unsigned int N, unsigned int M)
    {
        for (unsigned int i = 0; i < N; ++i)
        {
            dvolt[i]->addValue(circuit()->getNodeVolt(getInterestNode(i)));
        }
        for (unsigned int i = 0; i < M; ++i)
        {
            dcurrent[i]->addValue(circuit()->getBranchCurrent(getInterestBranch(i)));
        }
        dtime.addValue(tTime);
    }

    int AnalyzerTran::prepareMNA()
    {
        if (m_analyzeTrOP)
        {
            /* Prepare transient operating point analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->prepareTrOP());
            }
        }
        else
        {
            /* Prepare transient analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->prepareTR());
            }
        }
        return 0;
    }

    int AnalyzerTran::iterateMNA()
    {
        if (m_analyzeTrOP)
        {
            /* Iteration for transient operating point analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->iterateTrOP());
            }
        }
        else
        {
            /* Iteration for transient analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->iterateTR(circuit()->getTimeInStep()));
            }
        }
        return 0;
    }

}
