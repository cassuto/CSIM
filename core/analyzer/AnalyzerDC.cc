/**
 * @file DC analysis
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

#include <cctype>
#include "csim/model/ModelBase.h"
#include "csim/utils/errors.h"
#include "csim/utils/string.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "AnalyzerDC.h"

namespace csim
{

    AnalyzerDC::AnalyzerDC(Circuit *circuit)
        : AnalyzerBase(circuit)
    {
        property().addProperty("vstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0), "Start value", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("vstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0), "Stop value", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("vincr", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0), "Increment value", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("srcname", csimModel::Variant(csimModel::Variant::VariantString).setString(""), "Name of source instance", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }
    AnalyzerDC::~AnalyzerDC()
    {
    }

    int AnalyzerDC::analyze(Dataset *dataset)
    {
        if (property().missingRequired())
            return CERR_MSSING_PARAMETERS;
        double vstart = property().getProperty("vstart").getDouble();
        double vstop = property().getProperty("vstop").getDouble();
        double vincr = property().getProperty("vincr").getDouble();
        const char *srcname = property().getProperty("srcname").getString();
        bool sweepTemp = false;
        csimModel::ModelBase *model;
        csimModel::Variant *sweepVariable = 0l;
        std::string sweepName;

        switch (std::toupper(*srcname))
        {
        case 'V':
        case 'R':
            model = circuit()->netlist()->getComponent(srcname);
            sweepName = srcname;
            break;
        case 'T':
            if (strcmp_nocase(srcname, "TEMP") == 0)
            {
                sweepTemp = true;
                sweepName = "Tempature";
            }
            /* Fall through */
        default:
            return CERR_INVALID_SRCNAME;
        }

        if (!sweepTemp)
        {
            switch (std::toupper(*srcname))
            {
            case 'V':
                sweepVariable = &model->property().getProperty("V");
                break;
            case 'R':
                sweepVariable = &model->property().getProperty("R");
                break;
            }
        }

        /*
         * Format data set
         */
        dataset->clear();
        dataset->setName("DC sweep analysis");
        Variable &dsweep = dataset->addIndependentVar(sweepName.c_str());
        unsigned int N = getNumInterestNodes(), M = getNumInterestBranches();
        std::vector<Variable *> dvolt(N), dcurrent(M);
        for (unsigned int i = 0; i < N; ++i)
        {
            std::string varName = makeVarName("V", getInterestNode(i));
            dvolt[i] = &dataset->addDependentVar("voltage", varName);
            dvolt[i]->addIndependVar(dsweep);
        }
        for (unsigned int i = 0; i < M; ++i)
        {
            std::string varName = makeVarName("I", getInterestBranch(i));
            dcurrent[i] = &dataset->addDependentVar("current", varName);
            dcurrent[i]->addIndependVar(dsweep);
        }

        UPDATE_RC(circuit()->initMNA());
#if defined(ENABLE_SPICE_COMPATIBLE)
        UPDATE_RC(circuit()->spiceCompatible()->setFlagsDC());
#endif
        UPDATE_RC(circuit()->prepareMNA(this));

        double sweepValue = vstart;
        while (sweepValue <= vstop)
        {
            if (sweepTemp)
            {
                circuit()->environment()->setTemp(sweepValue);
            }
            else
            {
                sweepVariable->setDouble(sweepValue);
            }
            UPDATE_RC(circuit()->solveMNA(this));
#if defined(ENABLE_SPICE_COMPATIBLE)
            circuit()->spiceCompatible()->setFlagsTRPred();
#endif

            /* Save the result */
            for (unsigned int i = 0; i < N; ++i)
            {
                dvolt[i]->addValue(circuit()->getNodeVolt(getInterestNode(i)));
            }
            for (unsigned int i = 0; i < M; ++i)
            {
                dcurrent[i]->addValue(circuit()->getBranchCurrent(getInterestBranch(i)));
            }
            dsweep.addValue(sweepValue);
            sweepValue += vincr;
        }

        return 0;
    }

    int AnalyzerDC::prepareMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->prepareDC());
        }
        return 0;
    }

    int AnalyzerDC::iterateMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->iterateDC());
        }
        return 0;
    }

}
