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

    Resistor::Resistor() : m_G(0.0)
    {
        property().addProperty("R", Variant(Variant::VariantDouble).setDouble(1.0), true);
    }

    Resistor::~Resistor()
    {
    }

    void Resistor::configure()
    {
        double R = property().getProperty("R").getDouble();
        if (R == 0.0)
        {
            createMatrix(2, 1);
        }
        else
        {
            createMatrix(2, 0);
            m_G = 1.0 / R;
        }
    }

    void Resistor::prepareDC()
    {
        double R = property().getProperty("R").getDouble();

        if (R == 0.0)
        {
            setVS(0, 0, 1, 0.0);
        }
        else
        {
            setY(0, 0, +m_G), setY(0, 1, -m_G);
            setY(1, 0, -m_G), setY(1, 1, +m_G);
        }
    }
    void Resistor::prepareAC()
    {
        prepareDC();
    }
    void Resistor::prepareTR()
    {
        prepareDC();
    }

    void Resistor::iterateStep()
    {
    }

}
