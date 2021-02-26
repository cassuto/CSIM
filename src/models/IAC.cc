/**
 * @file Ideal AC Current Source Model
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

#include <cmath>
#include "constants.h"
#include "IAC.h"

namespace csimModel
{

    IAC::IAC(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_Ip(0.0),
          m_omega(0.0),
          m_phase(0.0)
    {
        property().addProperty("Ip", Variant(Variant::VariantDouble).setDouble(0.2), true);
        property().addProperty("freq", Variant(Variant::VariantDouble).setDouble(50.0), true);
        property().addProperty("phase", Variant(Variant::VariantDouble).setDouble(0.0), true);
    }

    IAC::~IAC()
    {
    }

    int IAC::configure()
    {
        resizeModel(2, 0, 0);
        m_Ip = property().getProperty("Ip").getDouble();
        m_omega = M_PI * property().getProperty("freq").getDouble() / 180.0;
        m_phase = M_PI * property().getProperty("phase").getDouble() / 180.0;

        m_I = MComplex(m_Ip * std::cos(m_phase), m_Ip * std::sin(m_phase));
        return 0;
    }

    int IAC::prepareDC()
    {
        return 0;
    }
    int IAC::prepareAC()
    {
        return 0;
    }
    int IAC::prepareTR()
    {
        return 0;
    }

    int IAC::iterateDC()
    {
        addI(getNode(0), -m_Ip);
        addI(getNode(1), m_Ip);
        return 0;
    }

    int IAC::iterateAC(double omega)
    {
        addI(getNode(0), -m_I);
        addI(getNode(1), m_I);
        return 0;
    }

    int IAC::iterateTR(double tTime)
    {
        double I = m_Ip * std::sin(m_omega * tTime + m_phase);
        addI(getNode(0), -I);
        addI(getNode(1), I);
        return 0;
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::IAC(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "IAC",
    /* description */
    "Generic ideal AC Current Source"};
