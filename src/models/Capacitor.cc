/**
 * @file Capacitor model
 */

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

#include "Constants.h"
#include "Capacitor.h"

namespace csimModel
{

    Capacitor::Capacitor(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_kZimag(0.0)
    {
        property().addProperty("C", Variant(Variant::VariantDouble).setDouble(1e-5), false);
    }

    Capacitor::~Capacitor()
    {
    }

    int Capacitor::configure()
    {
        m_kZimag = property().getProperty("C").getDouble();
        resizeModel(2, 0, 0);
        return 0;
    }

    int Capacitor::prepareDC()
    {
        return 0;
    }
    int Capacitor::prepareAC()
    {
        return 0;
    }
    int Capacitor::prepareTR()
    {
        return 0;
    }

    int Capacitor::iterateDC()
    {
        return 0;
    }

    int Capacitor::iterateAC(double omega)
    {
        MComplex z = MComplex(0, m_kZimag * omega);
        setY(getNode(0), getNode(0), z), setY(getNode(0), getNode(1), -z);
        setY(getNode(1), getNode(0), -z), setY(getNode(1), getNode(1), z);
        return 0;
    }

    int Capacitor::iterateTR()
    {
        return 0;
    }
}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::Capacitor(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "CAP",
    /* description */
    "Generic  capacitor"};
