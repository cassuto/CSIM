/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
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
#include <vector>
#include <string>
#include <istream>
#include "csim/model/Variant.h"
#include "csim/internal/ModelLoader.h"

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
        csimModel::ModelBase *getComponent(const char *ref);
        int configComponent(const char *ref, const char *property, const csimModel::Variant &value);
        int getTermlNode(const char *ref, unsigned int terml, unsigned int *out);
        int getBranch(const char *ref, unsigned int idx, unsigned int *out);
        void clear();
        int prepare();
        int wire(const char *ref_A, unsigned int terml_A, const char *ref_B, unsigned int terml_B);
        int ground(const char *ref, unsigned int terml);
        int parse(std::istream &stream);
        int generateNodes();
        bool hasGroundNode();
        unsigned int getGroundNode();
        int addMdl(const char *name, const ModelEntry::MdlEntry *entry);
        csimModel::PropertyMdl *getMdl(const char *name);

        inline unsigned int getNumNodes() const
        {
            return m_numNodes;
        }
        inline unsigned int getNumBranches() const
        {
            return m_numBranches;
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

        class MdlInfo
        {
        public:
            const ModelEntry::MdlEntry *entry;
            csimModel::PropertyMdl *mdl;
            std::string name;
        };

        inline const std::vector<ModelInfo> &models() const
        {
            return m_models;
        }
        inline const std::vector<MdlInfo> &mdls() const
        {
            return m_mdls;
        }

    private:
        unsigned int ufsetGetRoot(unsigned int v);
        void ufsetMerge(unsigned int x, unsigned int y);

    private:
        unsigned int m_numNodes, m_numBranches, m_numTermls;
        Circuit *m_circuit;
        std::map<std::string, unsigned int> m_modelIndex;
        std::vector<ModelInfo> m_models;
        std::map<std::string, unsigned int> m_mdlIndex;
        std::vector<MdlInfo> m_mdls;
        std::vector<unsigned int> m_ufset, m_ufsetRanks, m_ufsetCount;
        bool m_hasGround;
    };
}

#endif // CSIM_NETLSIT_H_
