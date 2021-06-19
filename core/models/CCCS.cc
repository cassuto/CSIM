/**
 * @file Ideal Current Controlled Current Source Model
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

#include "CCCS.h"

namespace csimModel
{
    CCCS::CCCS(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR)
    {
        property().addProperty("alpha", Variant(Variant::VariantDouble).setDouble(1.0), 0l, csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }

    CCCS::~CCCS()
    {
    }

    int CCCS::configure()
    {
        m_alpha = property().getProperty("alpha").getDouble();
        resizeModel(4, 0, 1);
        return 0;
    }

    int CCCS::prepareDC()
    {
        return 0;
    }
    int CCCS::prepareAC()
    {
        return 0;
    }
    int CCCS::prepareTR()
    {
        return 0;
    }

    int CCCS::iterateDC()
    {
        unsigned int c = getBranch(0);
        addB(getNode(S_NODE), c, m_alpha);
        addB(getNode(T_NODE), c, -m_alpha);
        addB(getNode(P_NODE), c, 1.0);
        addB(getNode(Q_NODE), c, -1.0);
        addC(c, getNode(P_NODE), 1.0), addC(c, getNode(Q_NODE), -1.0);
        addD(c, c, 0.0);
        addE(c, 0.0);
        return 0;
    }

    int CCCS::iterateAC(double omega)
    {
        return iterateDC();
    }

    int CCCS::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::CCCS(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "CCCS",
    /* description */
    "Generic Ideal Current Controlled Current Source"};
