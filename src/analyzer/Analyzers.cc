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
        : m_numNodes(0),
          m_numVS(0),
          m_steps(0),
          m_nodeVolt(nullptr),
          m_branchCurrent(nullptr),
          m_circuit(circuit)
    {
    }

    AnalyzerBase::~AnalyzerBase()
    {
        delete[] m_nodeVolt;
        delete[] m_branchCurrent;
    }

    void AnalyzerBase::createVectors(unsigned int numNodes, unsigned int numVS, unsigned int numSteps)
    {
        m_steps = numSteps;
        m_numNodes = numNodes;
        m_numVS = numVS;
        m_nodeVolt = new Complex[numSteps * numNodes];
        m_branchCurrent = new Complex[numSteps * numVS];
    }

    unsigned int AnalyzerBase::getNumSteps()
    {
        return m_steps;
    }
    const Complex *AnalyzerBase::getNodeVoltVector(unsigned int step)
    {
        assert(step < getNumSteps());
        return &m_nodeVolt[step * m_numNodes];
    }
    const Complex *AnalyzerBase::getBranchCurrentVector(unsigned int step)
    {
        assert(step < getNumSteps());
        return &m_branchCurrent[step * m_numVS];
    }

    void AnalyzerBase::setNodeVoltVector(unsigned int step, const Complex *values)
    {
        memcpy(&m_nodeVolt[step * m_numNodes], values, sizeof(Complex) * m_numNodes);
    }
    void AnalyzerBase::setBranchCurrentVector(unsigned int step, const Complex *values)
    {
        memcpy(&m_branchCurrent[step * m_numVS], values, sizeof(Complex) * m_numVS);
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
