/**
 * @file Ideal DC Voltage Source Model
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

#include "VDC.h"

namespace csimModel
{

    VDC::VDC(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR)
    {
        property().addProperty("V", Variant(Variant::VariantDouble).setDouble(5.0), true);
    }

    VDC::~VDC()
    {
    }

    int VDC::configure()
    {
        resizeModel(2, 0, 1);
        return 0;
    }

    int VDC::prepareDC()
    {
        return 0;
    }
    int VDC::prepareAC()
    {
        return 0;
    }
    int VDC::prepareTR()
    {
        return 0;
    }

    int VDC::iterateDC()
    {
        double V = property().getProperty("V").getDouble();

        unsigned int k = getVS(0);
        setB(getNode(0), k, +1.0);
        setB(getNode(1), k, -1.0);
        setC(k, getNode(0), 1.0), setC(k, getNode(1), -1.0);
        setE(k, V);
        return 0;
    }
    int VDC::iterateAC()
    {
        return iterateDC();
    }
    int VDC::iterateTR()
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::VDC(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "VDC",
    /* description */
    "Generic ideal DC Voltage Source"};
