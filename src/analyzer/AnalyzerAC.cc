/**
 * @file AC analysis
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
#include "csim/utils/constants.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/AnalyzerAC.h"

namespace csim
{

    AnalyzerAC::AnalyzerAC(Circuit *circuit)
        : AnalyzerBase(circuit),
          m_currentPos(0.0),
          m_currentOmega(0.0)
    {
        property().addProperty("start", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(50.0), true);
        property().addProperty("stop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1000.0), true);
        property().addProperty("step", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.1), true);
    }
    AnalyzerAC::~AnalyzerAC()
    {
    }

    int AnalyzerAC::analyze()
    {
        UPDATE_RC(circuit()->initMNA(this));

        double start = property().getProperty("start").getDouble();
        double end = property().getProperty("start").getDouble();
        double step = property().getProperty("step").getDouble();

        if (start < end)
        {
            return -CERR_INVALD_RANGE;
        }
        m_steps = (end - start) / step;

        createVectors(circuit()->netlist()->getNumNodes(), circuit()->netlist()->getNumVS(), m_steps);

        for (unsigned int i = 0; i < m_steps; ++i)
        {
            m_currentPos = start + m_steps * i;
            m_currentOmega = 2 * M_PI * m_currentPos;

            UPDATE_RC(circuit()->solveMNA(this));

            /* Save the result */
            setNodeVoltVector(i, circuit()->getNodeVoltVector());
            setBranchCurrentVector(i, circuit()->getBranchCurrentVector());
        }
        return 0;
    }

    int AnalyzerAC::prepareMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->prepareAC());
        }
        return 0;
    }

    int AnalyzerAC::iterateMNA()
    {
        for (auto &mif : circuit()->netlist()->models())
        {
            UPDATE_RC(mif.model->iterateAC(m_currentOmega));
        }
        return 0;
    }

}
