/**
 * @file Analyzer base and factory
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

#include <cassert>
#include <cstring>
#include "csim/internal/AnalyzerDC.h"
#include "csim/internal/AnalyzerAC.h"
#include "csim/internal/AnalyzerTran.h"
#include "csim/internal/Analyzers.h"

namespace csim
{

    AnalyzerBase::AnalyzerBase(Circuit *circuit)
        : m_circuit(circuit)
    {
    }

    AnalyzerBase::~AnalyzerBase()
    {
    }

    std::string AnalyzerBase::makeVarName(const char *name, unsigned int node)
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%u", node);
        return (std::string(name) + "(" + buf) + ")";
    }

    AnalyzerBase *Analyzers::createInstance(const char *name, Circuit *circuit)
    {
        if (std::strcmp(name, "DC") == 0)
        {
            return new AnalyzerDC(circuit);
        }
        else if (std::strcmp(name, "AC") == 0)
        {
            return new AnalyzerAC(circuit);
        }
        else if (std::strcmp(name, "transient") == 0)
        {
            return new AnalyzerTran(circuit);
        }
        return nullptr;
    }

}
