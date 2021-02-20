/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#ifndef CSIM_NETLSIT_H_
#define CSIM_NETLSIT_H_

#include <map>
#include <string>
#include "csim/model/Variant.h"

namespace csimModel
{
    class ModelBase;
}

namespace csim
{
    class ModelEntry;
    class Circuit;

    class Netlist
    {
    public:
        Netlist(Circuit *circuit);
        ~Netlist();

        int addComponent(const char *ref, const ModelEntry *modelEnt);
        int configComponent(const char *ref, const char *property, const csimModel::Variant &value);
        int getTermlNode(const char *ref, unsigned int terml, unsigned int *out);
        void clear();
        int prepare();
        int wire(const char *ref_A, unsigned int terml_A, const char *ref_B, unsigned int terml_B);
        int generateNodes();

        inline unsigned int getNumNodes() const
        {
            return m_numNodes;
        }
        inline unsigned int getNumVS() const
        {
            return m_numVS;
        }

        class ModelInfo
        {
        public:
            const ModelEntry *entry;
            csimModel::ModelBase *model;

        private:
            unsigned int termlIndexOffset;
            friend Netlist;
        };

        inline const std::vector<ModelInfo> &models() const
        {
            return m_models;
        }

    private:
        unsigned int ufsetGetRoot(unsigned int v);
        void ufsetMerge(unsigned int x, unsigned int y);

    private:
        unsigned int m_numNodes, m_numVS, m_numTermls;
        Circuit *m_circuit;
        std::map<std::string, unsigned int> m_modelIndex;
        std::vector<ModelInfo> m_models;

        std::vector<unsigned int> m_ufset, m_ufsetRanks, m_ufsetCount;
    };
}

#endif // CSIM_NETLSIT_H_
