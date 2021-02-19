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

#include "csim/SubCircuit.h"

namespace csim
{

    SubCircuit::SubCircuit(int numNodes)
        : m_numNodes(numNodes),
          m_numVS(0),
          m_Y(0l), m_B(0l), m_C(0l), m_D(0l), m_I(0l), m_E(0l), m_U(0l), m_J(0l)
    {
    }

    SubCircuit::~SubCircuit()
    {
        delete[] m_Y;
        delete[] m_U;
        delete[] m_I;
        delete[] m_B;
        delete[] m_C;
        delete[] m_D;
        delete[] m_J;
        delete[] m_E;
    }

    void SubCircuit::createMatrix()
    {
        if (m_numNodes)
        {
            delete[] m_Y;
            delete[] m_U;
            delete[] m_I;
            m_Y = new Complex[m_numNodes * m_numNodes];
            m_U = new Complex[m_numNodes];
            m_I = new Complex[m_numNodes];
        }
        if (m_numVS)
        {
            delete[] m_B;
            delete[] m_C;
            delete[] m_D;
            delete[] m_J;
            delete[] m_E;
            m_B = new Complex[m_numNodes * m_numVS];
            m_C = new Complex[m_numVS * m_numNodes];
            m_D = new Complex[m_numVS * m_numVS];
            m_J = new Complex[m_numVS];
            m_E = new Complex[m_numVS];
        }
    }

    /**
     * @brief Set an independent voltage source.
     * @param k Index of SubCircuit branch
     * @param nodeP Node index of positive pole
     * @param nodeN Node index of negative pole
     * @param volt Voltage value.
     */
    void SubCircuit::setVS(int k, int nodeP, int nodeN, double volt)
    {
        setB(nodeP, k, 1.0);
        setB(nodeN, k, -1.0);
        setC(k, nodeP, 1.0);
        setC(k, nodeN, -1.0);
        setE(k, volt);
    }

}
