/**
 * @file AC analysis
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

#include <algorithm>
#include "csim/model/ModelBase.h"
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "AnalyzerAC.h"

namespace csim
{

    AnalyzerAC::AnalyzerAC(Circuit *circuit)
        : AnalyzerBase(circuit),
          m_currentPos(0.0),
          m_currentOmega(0.0),
          m_analyzeOp(false)
    {
        property().addProperty("fstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0), "Start frequency", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("fstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1000.0), "Stop frequency", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("fstep", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.1), "Frequency step", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }
    AnalyzerAC::~AnalyzerAC()
    {
    }

    int AnalyzerAC::analyze(Dataset *dataset)
    {
        /*
         * Format data set
         */
        dataset->clear();
        dataset->setName("AC analysis");
        Variable &dfreq = dataset->addIndependentVar("frequency");
        unsigned int N = getNumInterestNodes(), M = getNumInterestBranches();
        std::vector<Variable *> dvolt(N), dcurrent(M);
        for (unsigned int i = 0; i < N; ++i)
        {
            std::string varName = makeVarName("V", getInterestNode(i));
            dvolt[i] = &dataset->addDependentVar("voltage", varName);
            dvolt[i]->addIndependVar(dfreq);
        }
        for (unsigned int i = 0; i < M; ++i)
        {
            std::string varName = makeVarName("I", getInterestBranch(i));
            dcurrent[i] = &dataset->addDependentVar("current", varName);
            dcurrent[i]->addIndependVar(dfreq);
        }

        /*
         * See the sweep range
         */
        double fstart = property().getProperty("fstart").getDouble();
        double fstop = property().getProperty("fstop").getDouble();
        double fstep = property().getProperty("fstep").getDouble();

        if (fstop < fstart)
        {
            return CERR_INVALD_RANGE;
        }
        unsigned int numSteps = std::max(1.0, (fstop - fstart) / fstep);

        UPDATE_RC(circuit()->initMNA());

        /*
         * Solve operating point
         */
        m_analyzeOp = true;
#if defined(ENABLE_SPICE_COMPATIBLE)
        UPDATE_RC(circuit()->spiceCompatible()->setFlagsOP());
#endif
        UPDATE_RC(circuit()->prepareMNA(this, Circuit::ANALYSIS_FLAG_NO_TIME_DOMAIN));
        UPDATE_RC(circuit()->solveMNA(this));
        UPDATE_RC(circuit()->saveOP());
        m_analyzeOp = false;

        /*
         * Solve AC
         */
#if defined(ENABLE_SPICE_COMPATIBLE)
        UPDATE_RC(circuit()->spiceCompatible()->setFlagsAC());
#endif
        UPDATE_RC(circuit()->prepareMNA(this, Circuit::ANALYSIS_FLAG_NO_TIME_DOMAIN));

        for (unsigned int i = 0; i < numSteps; ++i)
        {
            m_currentPos = fstart + fstep * i;
            m_currentOmega = 2 * M_PI * m_currentPos;

            UPDATE_RC(circuit()->solveMNA(this));

            /* Save the result */
            for (unsigned int i = 0; i < N; ++i)
            {
                dvolt[i]->addValue(circuit()->getNodeVolt(getInterestNode(i)));
            }
            for (unsigned int i = 0; i < M; ++i)
            {
                dcurrent[i]->addValue(circuit()->getBranchCurrent(getInterestBranch(i)));
            }

            dfreq.addValue(m_currentPos);
        }
        return 0;
    }

    int AnalyzerAC::prepareMNA()
    {
        if (m_analyzeOp)
        {
            /* Prepare operating point analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->prepareOP());
            }
        }
        else
        {
            /* Prepare AC analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->prepareAC());
            }
        }
        return 0;
    }

    int AnalyzerAC::iterateMNA()
    {
        if (m_analyzeOp)
        {
            /* Iteration step for operating point analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->iterateOP());
            }
        }
        else
        {
            /* Iteration step for AC analysis */
            for (auto &mif : circuit()->netlist()->models())
            {
                UPDATE_RC(mif.model->iterateAC(m_currentOmega));
            }
        }

        return 0;
    }

}
