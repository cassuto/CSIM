/**
 * @file Netlist class: Generate circuit netlist. Manage life cycle of components.
 */

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

#include <algorithm>
#include <cassert>
#include "csim/utils/errors.h"
#include "csim/utils/string.h"
#include "csim/model/ModelBase.h"
#include "csim/internal/ModelLoader.h"
#include "csim/internal/Netlist.h"

namespace csim
{
    Netlist::Netlist(Circuit *circuit)
        : m_numNodes(0),
          m_numBranches(0),
          m_numTermls(0),
          m_circuit(circuit),
          m_hasGround(false)
    {
    }

    Netlist::~Netlist()
    {
        clear();
    }

    /**
     * @brief Add a model to the netlist
     * @param ref Reference text of model in the circuit
     * @param modelEnt Entry of target model
     * @return Status code.
     */
    int Netlist::addComponent(const char *ref, const ModelEntry *modelEnt)
    {
        std::string refNorm = toUpper(ref);
        if (m_modelIndex.find(refNorm) != m_modelIndex.end())
            return CERR_DUP_COMPONENT_REFERENCE;

        csimModel::ModelBase *model = modelEnt->createInstance(m_circuit);
        ModelInfo mif;
        mif.entry = modelEnt;
        mif.model = model;
        model->setName(refNorm.c_str());
        m_models.push_back(mif);
        m_modelIndex[refNorm] = m_models.size() - 1;
        return 0;
    }

    /**
     * @brief Get the instance of a component by its name.
     * @param ref Name of the instance
     * @retval NULL if no such instance
     * @retval Pointer to the component instance
     */
    csimModel::ModelBase *Netlist::getComponent(const char *ref)
    {
        std::string refNorm = toUpper(ref);
        if (m_modelIndex.find(refNorm) == m_modelIndex.end())
            return nullptr;
        return m_models[m_modelIndex.at(refNorm)].model;
    }

    int Netlist::configComponent(const char *ref, const char *property, const csimModel::Variant &value)
    {
        std::string refNorm = toUpper(ref);
        if (m_modelIndex.find(refNorm) == m_modelIndex.end())
            return CER_NO_SUCH_COMPONENT_REFERENCE;

        auto &mif = m_models[m_modelIndex.at(refNorm)];
        csimModel::PropertyBag &props = mif.model->property();

        if (props.hasProperty(property))
            props.setProperty(property, value);
        else
            return CERR_NO_SUCH_PROPERTY;

        return 0;
    }

    int Netlist::getTermlNode(const char *ref, unsigned int terml, unsigned int *out)
    {
        std::string refNorm = toUpper(ref);
        *out = (unsigned int)-1;
        if (m_modelIndex.find(refNorm) == m_modelIndex.end())
            return CER_NO_SUCH_COMPONENT_REFERENCE;
        auto &mif = m_models[m_modelIndex.at(refNorm)];
        if (terml >= mif.model->getNumTerml())
            return CERR_INVALID_TERML_INDEX;
        *out = mif.model->getNode(terml);
        return 0;
    }

    int Netlist::getBranch(const char *ref, unsigned int idx, unsigned int *out)
    {
        std::string refNorm = toUpper(ref);
        *out = (unsigned int)-1;
        if (m_modelIndex.find(refNorm) == m_modelIndex.end())
            return CER_NO_SUCH_COMPONENT_REFERENCE;
        auto &mif = m_models[m_modelIndex.at(refNorm)];
        if (idx >= mif.model->getNumBranches())
            return CERR_INVALID_BRANCH_INDEX;
        *out = mif.model->getBranch(idx);
        return 0;
    }

    void Netlist::clear()
    {
        for (auto &mif : m_models)
        {
            mif.entry->deleteInstance(mif.model);
        }
        m_modelIndex.clear();
        m_models.clear();
        for (auto &mif : m_mdls)
        {
            mif.entry->deleteInstance(mif.mdl);
        }
        m_mdlIndex.clear();
        m_mdls.clear();
        m_numNodes = m_numBranches = m_numTermls = 0;
        m_ufset.clear();
        m_hasGround = false;
    }

    int Netlist::prepare()
    {
        for (auto &mif : m_models)
        {
            if (mif.model->property().missingRequired())
                return CERR_MSSING_PARAMETERS;
            UPDATE_RC(mif.model->configure());
        }

        m_numNodes = m_numBranches = m_numTermls = 0;
        for (auto &minf : m_models)
        {
            m_numBranches += minf.model->getNumBranches();
            minf.termlIndexOffset = m_numTermls; /* Allocate global index of terminals */
            m_numTermls += minf.model->getNumTerml();
        }

        /* Init union-find set */
        m_ufset.resize(m_numTermls + 1);
        m_ufsetRanks.resize(m_numTermls + 1);
        m_ufsetCount.resize(m_numTermls + 1);
        for (unsigned int i = 0; i < m_numTermls + 1; ++i)
        {
            m_ufset[i] = i;
            m_ufsetRanks[i] = 0;
            m_ufsetCount[i] = 1;
        }

        return 0;
    }

    int Netlist::wire(const char *ref_A, unsigned int terml_A, const char *ref_B, unsigned int terml_B)
    {
        if (m_modelIndex.find(ref_A) == m_modelIndex.end() || m_modelIndex.find(ref_B) == m_modelIndex.end())
            return CER_NO_SUCH_COMPONENT_REFERENCE;

        const ModelInfo &mif_A = m_models[m_modelIndex.at(ref_A)];
        const ModelInfo &mif_B = m_models[m_modelIndex.at(ref_B)];

        if (terml_A >= mif_A.model->getNumTerml() || terml_B >= mif_B.model->getNumTerml())
            return CERR_INVALID_TERML_INDEX;

        unsigned int terml_gindex_A = terml_A + mif_A.termlIndexOffset;
        unsigned int terml_gindex_B = terml_B + mif_B.termlIndexOffset;

        ufsetMerge(terml_gindex_A, terml_gindex_B);
        return 0;
    }

    int Netlist::ground(const char *ref, unsigned int terml)
    {
        std::string refNorm = toUpper(ref);
        if (m_modelIndex.find(refNorm) == m_modelIndex.end())
            return CER_NO_SUCH_COMPONENT_REFERENCE;
        const ModelInfo &mif = m_models[m_modelIndex.at(refNorm)];
        if (terml >= mif.model->getNumTerml())
            return CERR_INVALID_TERML_INDEX;

        m_hasGround = true;
        ufsetMerge(terml + mif.termlIndexOffset, m_numTermls);
        return 0;
    }

    int Netlist::parse(std::istream &stream)
    {
        return 0;
    }

    int Netlist::generateNodes()
    {
        assert(m_ufset.size());
        m_numNodes = 0;

        unsigned int N = m_hasGround ? m_numTermls + 1 : m_numTermls;
        for (unsigned int i = 0; i < N; ++i)
        {
            if (m_ufset[i] == i)
            {
                /*
                 * Allocate node indexes.
                 * NOTE: As m_ufsetRanks is no longer in use, we use
                 * m_ufsetRanks to store node indexes.
                 */
                m_ufsetRanks[i] = m_numNodes;
                ++m_numNodes;
            }
        }

        /*
         * Assign index for inner nodes of models
         */
        for (auto &mif : m_models)
        {
            for (unsigned int i = 0; i < mif.model->getNumInnerNode(); ++i)
            {
                mif.model->setInnerNode(i, m_numNodes);
                ++m_numNodes;
            }
        }

        /*
         * Assign index to voltage sources
         */
        unsigned int vsIndex = 0;
        for (auto &mif : m_models)
        {
            for (unsigned int i = 0; i < mif.model->getNumBranches(); ++i)
            {
                mif.model->setBranch(i, vsIndex);
                vsIndex++;
            }
        }
        assert(vsIndex == m_numBranches);

        /* Assign the index to the node of each terminal */
        for (auto &mif : m_models)
        {
            for (unsigned int i = 0; i < mif.model->getNumTerml(); ++i)
            {
                unsigned int root = ufsetGetRoot(mif.termlIndexOffset + i);

                /* Check floating terminals */
                if (m_ufsetCount[root] < 2)
                {
                    return CERR_FLOATING_TERMINALS;
                }

                /* NOTE: Now m_ufsetRanks is node index table, no longer tree ranks */
                mif.model->setNode(i, m_ufsetRanks[root]);
            }
        }

        return 0;
    }

    bool Netlist::hasGroundNode()
    {
        return m_hasGround;
    }
    unsigned int Netlist::getGroundNode()
    {
        assert(hasGroundNode());
        return m_ufsetRanks[ufsetGetRoot(m_numTermls)];
    }

    /**
     * @brief Append a property model
     * @param name Name of the instance
     * @param entry Mdl entry
     * @return status code.
     */
    int Netlist::addMdl(const char *name, const ModelEntry::MdlEntry *entry)
    {
        std::string refNorm = toUpper(name);
        if (m_modelIndex.find(refNorm) != m_modelIndex.end())
            return CERR_DUP_COMPONENT_REFERENCE;

        csimModel::PropertyMdl *mdl = entry->createInstance();
        MdlInfo mif;
        mif.entry = entry;
        mif.mdl = mdl;
        mif.name = refNorm;
        m_mdls.push_back(mif);
        m_mdlIndex[refNorm] = m_mdls.size() - 1;
        return 0;
    }

    csimModel::PropertyMdl *Netlist::getMdl(const char *name)
    {
        std::string refNorm = toUpper(name);
        if (m_mdlIndex.find(refNorm) == m_mdlIndex.end())
            return nullptr;
        return m_mdls[m_mdlIndex.at(refNorm)].mdl;
    }

    unsigned int Netlist::ufsetGetRoot(unsigned int v)
    {
        /* Find the root of v */
        unsigned int p = v;
        while (m_ufset[p] != p)
            p = m_ufset[p];

        /* Path compression */
        unsigned int t;
        while (v != p)
        {
            t = m_ufset[v];
            m_ufset[v] = p;
            v = t;
        }
        return v;
    }

    void Netlist::ufsetMerge(unsigned int x, unsigned int y)
    {
        unsigned int rx = ufsetGetRoot(x), ry = ufsetGetRoot(y);
        if (rx == ry)
            return;
        if (m_ufsetRanks[rx] > m_ufsetRanks[ry])
        {
            m_ufset[ry] = rx; /* As x is the higher tree, merge y to x */
            m_ufsetCount[rx] += m_ufsetCount[ry];
        }
        else
        {
            m_ufset[rx] = ry; /* Merge x to y */
            m_ufsetCount[ry] += m_ufsetCount[rx];
            if (m_ufsetRanks[rx] == m_ufsetRanks[ry])
                m_ufsetRanks[ry]++; /* Updating tree rank */
        }
    }
}
