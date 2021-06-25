/**
 * @file Ideal DC Current Source Model
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

#include "IDC.h"

namespace csimModel
{

    IDC::IDC(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR)
    {
        property().addProperty("I", Variant(Variant::VariantDouble).setDouble(0.2), nullptr, csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
    }

    IDC::~IDC()
    {
    }

    int IDC::configure()
    {
        resizeModel(2, 0, 0);
        return 0;
    }

    int IDC::prepareDC()
    {
        return 0;
    }
    int IDC::prepareAC()
    {
        return 0;
    }
    int IDC::prepareTR()
    {
        return 0;
    }

    int IDC::iterateDC()
    {
        double I = property().getProperty("I").getDouble();

        addI(getNode(0), -I);
        addI(getNode(1), I);
        return 0;
    }

    int IDC::iterateAC(double omega)
    {
        /* Remove any DC offset */
        return 0;
    }

    int IDC::iterateTR(double tTime)
    {
        return iterateDC();
    }

}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::IDC(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const PropertyMdlDescriptor *mdlDescriptors = nullptr;

const size_t numMdlDescriptors = 0;

const ModelDescriptor descriptor = {
    /* id */
    "IDC",
    /* description */
    "Generic ideal DC Current Source"};
