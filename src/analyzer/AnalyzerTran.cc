/**
 * @file Transient analysis
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

#include "csim/model/ModelBase.h"
#include "csim/utils/errors.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "AnalyzerTran.h"

namespace csim
{

    AnalyzerTran::AnalyzerTran(Circuit *circuit)
        : AnalyzerBase(circuit)
    {
        property().addProperty("tstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0), true);
        property().addProperty("tstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1e-3), true);
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
        circuit()->setIntegralStep(tstep);

        UPDATE_RC(circuit()->initMNA(this));

        double tTime = 0.0;
        do
        {
            UPDATE_RC(circuit()->stepMNA(this));
            tTime = circuit()->getIntegralTime();

            /* Save the result */
            for (unsigned int i = 0; i < N; ++i)
            {
                dvolt[i]->addValue(circuit()->getNodeVolt(getInterestNode(i)));
            }
            for (unsigned int i = 0; i < M; ++i)
            {
                dcurrent[i]->addValue(circuit()->getBranchCurrent(getInterestBranch(i)));
            }
            dtime.addValue(tTime);
        } while (tTime < tstop);

        return 0;
    }

    int AnalyzerTran::prepareMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->prepareTR());
        }
        return 0;
    }

    int AnalyzerTran::iterateMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->iterateTR(circuit()->getIntegralTime()));
        }
        return 0;
    }

}
