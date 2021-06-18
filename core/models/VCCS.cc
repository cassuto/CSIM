/**
 * @file Ideal Voltage Controlled Current Source Model
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

#include "VCCS.h"

namespace csimModel
{
    VCCS::VCCS(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR)
    {
        property().addProperty("G", Variant(Variant::VariantDouble).setDouble(1.0), 0l, csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }

    VCCS::~VCCS()
    {
    }

    int VCCS::configure()
    {
        m_g = property().getProperty("G").getDouble();
        resizeModel(4, 0, 0);
        return 0;
    }

    int VCCS::prepareDC()
    {
        return 0;
    }
    int VCCS::prepareAC()
    {
        return 0;
    }
    int VCCS::prepareTR()
    {
        return 0;
    }

    int VCCS::iterateDC()
    {
        addY(getNode(S_NODE), getNode(P_NODE), m_g), addY(getNode(S_NODE), getNode(Q_NODE), -m_g);
        addY(getNode(T_NODE), getNode(P_NODE), -m_g), addY(getNode(T_NODE), getNode(Q_NODE), m_g);
        return 0;
    }

    int VCCS::iterateAC(double omega)
    {
        return iterateDC();
    }

    int VCCS::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::VCCS(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "VCCS",
    /* description */
    "Generic Ideal Voltage Controlled Current Source"};
