/**
 * @file Resistor model
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

#include "Resistor.h"

namespace csimModel
{

    Resistor::Resistor(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_G(0.0),
          m_cutThrough(false)
    {
        property().addProperty("R", Variant(Variant::VariantDouble).setDouble(1.0), true);
    }

    Resistor::~Resistor()
    {
    }

    int Resistor::configure()
    {
        double R = property().getProperty("R").getDouble();
        if (R == 0.0)
        {
            resizeModel(2, 0, 1);
            m_cutThrough = true;
        }
        else
        {
            resizeModel(2, 0, 0);
            m_G = 1.0 / R;
            m_cutThrough = false;
        }
        return 0;
    }

    int Resistor::prepareDC()
    {
        return 0;
    }
    int Resistor::prepareAC()
    {
        return 0;
    }
    int Resistor::prepareTR()
    {
        return 0;
    }

    int Resistor::iterateDC()
    {
        if (m_cutThrough)
        {
            unsigned int k = getVS(0);
            setB(getNode(0), k, +1.0);
            setB(getNode(1), k, -1.0);
            setC(k, getNode(0), 1.0), setC(k, getNode(1), -1.0);
            setE(k, 0.0);
        }
        else
        {
            addY(getNode(0), getNode(0), +m_G), addY(getNode(0), getNode(1), -m_G);
            addY(getNode(1), getNode(0), -m_G), addY(getNode(1), getNode(1), +m_G);
        }
        return 0;
    }
    int Resistor::iterateAC(double omega)
    {
        return iterateDC();
    }
    int Resistor::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::Resistor(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "R",
    /* description */
    "Generic ideal resistor"};
