/**
 * @file Ideal Voltage Controlled Voltage Source Model
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

#include "VCVS.h"

namespace csimModel
{
    VCVS::VCVS(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_mu(0.0)
    {
        property().addProperty("Mu", Variant(Variant::VariantDouble).setDouble(1.0), 0l, csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }

    VCVS::~VCVS()
    {
    }

    int VCVS::configure()
    {
        m_mu = property().getProperty("Mu").getDouble();
        resizeModel(4, 0, 1);
        return 0;
    }

    int VCVS::prepareDC()
    {
        return 0;
    }
    int VCVS::prepareAC()
    {
        return 0;
    }
    int VCVS::prepareTR()
    {
        return 0;
    }

    int VCVS::iterateDC()
    {
        unsigned int k = getBranch(0);
        addB(getNode(S_NODE), k, +1.0);
        addB(getNode(T_NODE), k, -1.0);
        addC(k, getNode(S_NODE), 1.0), addC(k, getNode(T_NODE), -1.0);
        addC(k, getNode(P_NODE), -m_mu), addC(k, getNode(Q_NODE), m_mu);
        addE(k, 0);
        return 0;
    }

    int VCVS::iterateAC(double omega)
    {
        return iterateDC();
    }

    int VCVS::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::VCVS(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "VCVS",
    /* description */
    "Generic ideal Voltage Controlled Voltage Source"};
