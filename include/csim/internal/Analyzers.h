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

#ifndef CSIM_ANALYZEES_H_
#define CSIM_ANALYZEES_H_

#include <vector>
#include "csim/model/PropertyBag.h"

namespace csim
{
    class Circuit;
    class Netlist;
    class Dataset;

    class AnalyzerBase
    {
    public:
        AnalyzerBase(Circuit *circuit);
        virtual ~AnalyzerBase();

        virtual int analyze(Dataset *dataset) = 0;

        /* Callbacks for MNA */
        virtual int prepareMNA() = 0;
        virtual int iterateMNA() = 0;

    public:
        std::string makeVarName(const char *name, unsigned int node);

        inline void addInterestNode(unsigned int node)
        {
            m_interestNodes.push_back(node);
        }
        inline void addInterestBranch(unsigned int vs)
        {
            m_interestNodes.push_back(vs);
        }
        inline unsigned int getNumInterestNodes() const
        {
            return m_interestNodes.size();
        }
        inline unsigned int getNumInterestBranches() const
        {
            return m_interestBranches.size();
        }

        inline unsigned int getInterestNode(unsigned int index) const
        {
            return m_interestNodes[index];
        }

        inline unsigned int getInterestBranch(unsigned int index) const
        {
            return m_interestBranches[index];
        }
        inline void clearInterests()
        {
            m_interestNodes.clear();
            m_interestBranches.clear();
        }

        inline csimModel::PropertyBag &property()
        {
            return m_property;
        }

        inline Circuit *circuit() const
        {
            return m_circuit;
        }

    private:
        csimModel::PropertyBag m_property;
        Circuit *m_circuit;
        std::vector<unsigned int> m_interestNodes;
        std::vector<unsigned int> m_interestBranches;
    };

    class Analyzers
    {
    public:
        static AnalyzerBase *createInstance(const char *name, Circuit *circuit);
    };
}

#endif // CSIM_ANALYZEES_H_
