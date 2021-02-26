/**
 * @file Inductor model
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
#include "Inductor.h"

namespace csimModel
{

    Inductor::Inductor(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_kZimag(0.0),
          m_cutThrough(false)
    {
        property().addProperty("L", Variant(Variant::VariantDouble).setDouble(1e-5), false);
    }

    Inductor::~Inductor()
    {
    }

    int Inductor::configure()
    {
        m_kZimag = property().getProperty("L").getDouble();
        m_cutThrough = (m_kZimag == 0.0);

        resizeModel(2, 0, 1);
        return 0;
    }

    int Inductor::prepareDC()
    {
        return 0;
    }
    int Inductor::prepareAC()
    {
        return 0;
    }
    int Inductor::prepareTR()
    {
        resizeIntegrator(1);
        return 0;
    }

    int Inductor::iterateDC()
    {
        unsigned int k = getVS(0);
        addB(getNode(0), k, +1.0);
        addB(getNode(1), k, -1.0);
        addC(k, getNode(0), 1.0), addC(k, getNode(1), -1.0);
        addE(k, 0.0);
        return 0;
    }

    int Inductor::iterateAC(double omega)
    {
        unsigned int k = getVS(0);
        addB(getNode(0), k, +1.0);
        addB(getNode(1), k, -1.0);

        if (m_cutThrough)
        {
            addC(k, getNode(0), 1.0), addC(k, getNode(1), -1.0);
            addE(k, 0.0);
        }
        else
        {
            addD(k, k, 1.0);
            addE(k, 0.0);

            MComplex z = MComplex(0, -1.0 / (m_kZimag * omega));
            addY(getNode(0), getNode(0), z), addY(getNode(0), getNode(1), -z);
            addY(getNode(1), getNode(0), -z), addY(getNode(1), getNode(1), z);
        }
        return 0;
    }
    int Inductor::iterateTR(double tTime)
    {
        double volt = (getU(getNode(0)) - getU(getNode(1))).real();
        double req, Ueq;

        integrate(0, volt, m_kZimag, &req, &Ueq);

        unsigned int k = getVS(0);
        addB(getNode(0), k, +1.0);
        addB(getNode(1), k, -1.0);
        addC(k, getNode(0), +1.0), addC(k, getNode(1), -1.0);
        addD(k, k, -req);
        addE(k, Ueq);
        return 0;
    }
}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::Inductor(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "INDUCTOR",
    /* description */
    "Generic Inductor"};
