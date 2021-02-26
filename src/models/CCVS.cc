/**
 * @file Ideal Current Controlled Voltage Source Model
 */

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

#include "CCVS.h"

namespace csimModel
{
    CCVS::CCVS(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_r(0.0)
    {
        property().addProperty("r", Variant(Variant::VariantDouble).setDouble(1.0), true);
    }

    CCVS::~CCVS()
    {
    }

    int CCVS::configure()
    {
        m_r = property().getProperty("r").getDouble();
        resizeModel(4, 0, 2);
        return 0;
    }

    int CCVS::prepareDC()
    {
        return 0;
    }
    int CCVS::prepareAC()
    {
        return 0;
    }
    int CCVS::prepareTR()
    {
        return 0;
    }

    int CCVS::iterateDC()
    {
        unsigned int k = getVS(0), c = getVS(1);
        addB(getNode(S_NODE), k, +1.0);
        addB(getNode(T_NODE), k, -1.0);
        addB(getNode(P_NODE), c, +1.0);
        addB(getNode(Q_NODE), c, -1.0);
        addC(k, getNode(S_NODE), 1.0), addC(k, getNode(T_NODE), -1.0);
        addC(c, getNode(P_NODE), 1.0), addC(c, getNode(Q_NODE), -1.0);
        addD(k, c, -m_r);
        addD(c, c, 0.0);
        addE(k, 0.0);
        addE(c, 0.0);
        return 0;
    }

    int CCVS::iterateAC(double omega)
    {
        return iterateDC();
    }

    int CCVS::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::CCVS(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "CCVS",
    /* description */
    "Generic Ideal Current Controlled Voltage Source Source"};
