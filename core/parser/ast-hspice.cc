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

#include <cassert>
#include <cmath>
#include <string>
#include <cmath>
#include <exception>
#include <iostream>
#include "csim/utils/errors.h"
#include "csim/model/PropertyMdl.h"
#include "csim/internal/parser/algebraic-defs.h"
#include "csim/internal/parser/hspice-defs.h"

namespace csim
{
    HSPICE_AST *hspice_ast;
    bool hspice_err;

    HSPICE_Block::~HSPICE_Block()
    {
        for (size_t i = 0; i < subs.size(); i++)
            delete subs[i];

        for (size_t i = 0; i < models.size(); i++)
        {
            csim::HSPICE_KeyValueList *modelParams = models[i].params;
            for (size_t j = 0; j < modelParams->kvs.size(); j++)
            {
                /* Delete expression of model parameter */
                if (modelParams->kvs[j].second.getType() == csimModel::Variant::VariantAlgebraic)
                    delete modelParams->kvs[j].second.getAlgebraic();
            }
            delete modelParams;
        }
    }

    HSPICE_SubBlock::~HSPICE_SubBlock()
    {
        delete nodes;
        delete block;
    }

    int HSPICE_Model::configureMdl(const ModelEntry::MdlEntry *mdlEntry, csimModel::PropertyMdl *mdl)
    {
        for (auto p = params->kvs.rbegin(); p != params->kvs.rend(); p++)
        {
            const PropertyMdlPropDescriptor *prop = mdlEntry->getProperty(p->first.c_str());
            if (prop)
            {
                if (p->second.getType() == csimModel::Variant::VariantAlgebraic)
                {
                    double val;
                    UPDATE_RC(p->second.getAlgebraic()->evaluate(parent, &val));
                    UPDATE_RC(mdl->setProperty(prop->id, csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(val)));
                }
                else {
                    UPDATE_RC(mdl->setProperty(prop->id, p->second));
                }
            }
            else
            {
                std::cout << "Warning: No such property '" << p->first.c_str() << "'" << std::endl;
            }
        }
        return 0;
    }

    HSPICE_AST::HSPICE_AST()
        : blockRoot(nullptr)
    {
    }
    HSPICE_AST::~HSPICE_AST()
    {
        delete blockRoot;
    }

    csimModel::Variant hspice_parseReal(const char *str)
    {
        return csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(algebraic_parseReal(str));
    }

    int HSPICE_AST::parse(const char *filename)
    {
        int rc = 0;
        delete blockRoot;

        /*TODO: Global lock here */
        hspice_in = fopen(filename, "r");
        if (!hspice_in)
            return CERR_OPEN_FILE;
        hspice_restart(hspice_in);
        hspice_wrap();
        hspice_lineno = 1;
        hspice_ast = this;
        hspice_err = false;
        if (hspice_parse())
            rc = CERR_PARSE_FILE;
        if (hspice_err)
            rc = CERR_PARSE_FILE;
        fclose(hspice_in);
        hspice_in = nullptr;
        hspice_ast = nullptr;
        /*TODO: Global unlock herse */
        return rc;
    }

    void HSPICE_AST::dump()
    {
        std::cout << "Title = " << getTitle() << std::endl;

        dump(blockRoot, 0);
    }

    void HSPICE_AST::dumpIndent(int level)
    {
        while (level--)
        {
            std::cout << "\t";
        }
    }

    void HSPICE_AST::dump(HSPICE_Block *block, int level)
    {
        for (auto model = block->models.rbegin(); model != block->models.rend(); model++)
        {
            dumpIndent(level);
            std::cout << ".Model = " << model->name << ":" << std::endl;
            for (auto param = model->params->kvs.rbegin(); param != model->params->kvs.rend(); param++)
            {
                dumpIndent(level);
                std::cout << "\t" << param->first << " = ";
                switch (param->second.getType())
                {
                case csimModel::Variant::VariantDouble:
                    std::cout << param->second.getDouble();
                    break;
                case csimModel::Variant::VariantString:
                    std::cout << "\"" << param->second.getString() << "\"";
                    break;
                case csimModel::Variant::VariantAlgebraic:
                {
                    double paramValue;
                    if (param->second.getAlgebraic()->evaluate(block, &paramValue))
                        std::cout << "(failed evaluation)";
                    else
                        std::cout << paramValue;
                    break;
                }
                default:
                    std::cout << "(unknown type)";
                }
                std::cout << std::endl;
            }
        }

        for (auto it = block->subs.rbegin(); it != block->subs.rend(); it++)
        {
            HSPICE_SubBlock *sub = *it;
            dumpIndent(level);
            std::cout << ".SUBCKT = " << sub->name << ":" << std::endl;
            dumpIndent(level);
            std::cout << "\tNodes = ";
            for (auto node = sub->nodes->nodes.rbegin(); node != sub->nodes->nodes.rend(); node++)
            {
                std::cout << "\"" << *node << "\" ";
            }
            std::cout << std::endl;
            dump(sub->block, level + 1);

            dumpIndent(level);
            std::cout << ".End SUBCKT " << sub->name << std::endl;
        }
    }
}
