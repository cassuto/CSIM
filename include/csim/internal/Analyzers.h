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

#ifndef CSIM_ANALYZEES_H_
#define CSIM_ANALYZEES_H_

#include "csim/model/PropertyBag.h"
#include "csim/internal/Complex.h"

namespace csim
{
    class Circuit;
    class Netlist;

    class AnalyzerBase
    {
    public:
        AnalyzerBase(Circuit *circuit);
        virtual ~AnalyzerBase();

        virtual int analyze() = 0;
        virtual unsigned int getNumSteps();
        virtual double getPosition(unsigned int step) = 0;
        virtual const Complex *getNodeVoltVector(unsigned int step);
        virtual const Complex *getBranchCurrentVector(unsigned int step);

        /* Callbacks for MNA */
        virtual int prepareMNA() = 0;
        virtual int iterateMNA() = 0;

    public:
        inline csimModel::PropertyBag &property()
        {
            return m_property;
        }

        inline Circuit *circuit() const
        {
            return m_circuit;
        }

    protected:
        unsigned int m_numNodes;
        unsigned int m_numVS;
        unsigned int m_steps;
        Complex *m_nodeVolt;
        Complex *m_branchCurrent;

        void createVectors(unsigned int numNodes, unsigned int numVS, unsigned int numSteps);
        void setNodeVoltVector(unsigned int step, const Complex *values);
        void setBranchCurrentVector(unsigned int step, const Complex *values);

    private:
        csimModel::PropertyBag m_property;
        Circuit *m_circuit;
    };

    class Analyzers
    {
    public:
        static AnalyzerBase *createInstance(const char *name, Circuit *circuit);
    };
}

#endif // CSIM_ANALYZEES_H_
