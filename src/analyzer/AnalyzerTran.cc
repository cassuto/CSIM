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
#include "csim/internal/AnalyzerTran.h"

namespace csim
{

    AnalyzerTran::AnalyzerTran(Circuit *circuit)
        : AnalyzerBase(circuit)
    {
    }
    AnalyzerTran::~AnalyzerTran()
    {
    }

    int AnalyzerTran::analyze(Dataset *dataset)
    {
        UPDATE_RC(circuit()->initMNA(this));
        UPDATE_RC(circuit()->solveMNA(this));
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
            UPDATE_RC(mif.model->iterateTR());
        }
        return 0;
    }

}
