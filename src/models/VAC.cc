/**
 * @file Ideal AC Voltage Source Model
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

#include <cmath>
#include "VAC.h"

namespace csimModel
{

    VAC::VAC(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR)
    {
        property().addProperty("Vp", Variant(Variant::VariantDouble).setDouble(5.0), true);
        property().addProperty("freq", Variant(Variant::VariantDouble).setDouble(50.0), true);
        property().addProperty("phase", Variant(Variant::VariantDouble).setDouble(0.0), true);
    }

    VAC::~VAC()
    {
    }

    int VAC::configure()
    {
        double Vrms = property().getProperty("Vp").getDouble();
        double phase = property().getProperty("phase").getDouble();

        phase = M_PI * phase / 180.0;
        m_E = MComplex(Vrms * std::cos(phase), Vrms * std::sin(phase));

        resizeModel(2, 0, 1);
        return 0;
    }

    int VAC::prepareDC()
    {
        return 0;
    }
    int VAC::prepareAC()
    {
        return 0;
    }
    int VAC::prepareTR()
    {
        return 0;
    }

    int VAC::iterateDC()
    {
        unsigned int k = getVS(0);
        setB(getNode(0), k, +1.0);
        setB(getNode(1), k, -1.0);
        setC(k, getNode(0), 1.0), setC(k, getNode(1), -1.0);
        setE(k, 0.0);
        return 0;
    }

    int VAC::iterateAC(double omega)
    {
        unsigned int k = getVS(0);
        setB(getNode(0), k, +1.0);
        setB(getNode(1), k, -1.0);
        setC(k, getNode(0), 1.0), setC(k, getNode(1), -1.0);
        setE(k, m_E);
        return 0;
    }

    int VAC::iterateTR()
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::VAC(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "VAC",
    /* description */
    "Generic ideal AC Voltage Source"};
