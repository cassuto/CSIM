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

#include <cassert>
#include "csim/model/ModelBase.h"

namespace csimModel
{

    ModelBase::ModelBase()
        : m_numNodes(0),
          m_numVS(0),
          m_Y(0l), m_B(0l), m_C(0l), m_D(0l), m_I(0l), m_E(0l), m_U(0l), m_J(0l)
    {
    }

    ModelBase::~ModelBase()
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

    /**
     * @brief Create matrices
     * @param numNodes The number of nodes.
     * @param numVS The number of voltage sources, 0 if none.
     */
    void ModelBase::createMatrix(int numNodes, int numVS)
    {
        m_numNodes = numNodes;
        m_numVS = numVS;

        if (m_numNodes)
        {
            delete[] m_Y;
            delete[] m_U;
            delete[] m_I;
            m_Y = new MComplex[m_numNodes * m_numNodes];
            m_U = new MComplex[m_numNodes];
            m_I = new MComplex[m_numNodes];
        }
        if (m_numVS)
        {
            delete[] m_B;
            delete[] m_C;
            delete[] m_D;
            delete[] m_J;
            delete[] m_E;
            m_B = new MComplex[m_numNodes * m_numVS];
            m_C = new MComplex[m_numVS * m_numNodes];
            m_D = new MComplex[m_numVS * m_numVS];
            m_J = new MComplex[m_numVS];
            m_E = new MComplex[m_numVS];
        }
    }

    /**
     * @brief Set an independent voltage source.
     * @param k Index of SubCircuit branch
     * @param nodeP Node index of positive pole
     * @param nodeN Node index of negative pole
     * @param volt Voltage value.
     */
    void ModelBase::setVS(int k, int nodeP, int nodeN, double volt)
    {
        setB(nodeP, k, 1.0);
        setB(nodeN, k, -1.0);
        setC(k, nodeP, 1.0), setC(k, nodeN, -1.0);
        setE(k, volt);
    }

    PropertyBag &ModelBase::property()
    {
        return m_props;
    }

    /* MNA matrices */
    const MComplex &ModelBase::getY(int row, int col) const
    {
        assert(row < m_numNodes && col < m_numNodes);
        return m_Y[row * m_numNodes + col];
    }
    const MComplex &ModelBase::getB(int row, int col) const
    {
        assert(row < m_numNodes && col < m_numVS);
        return m_B[row * m_numVS + col];
    }
    const MComplex &ModelBase::getC(int row, int col) const
    {
        assert(row < m_numVS && col < m_numNodes);
        return m_C[row * m_numNodes + col];
    }
    const MComplex &ModelBase::getD(int row, int col) const
    {
        assert(row < m_numVS && col < m_numVS);
        return m_D[row * m_numVS + col];
    }
    const MComplex &ModelBase::getI(int row) const
    {
        assert(row < m_numNodes);
        return m_I[row];
    }
    const MComplex &ModelBase::getE(int row) const
    {
        assert(row < m_numVS);
        return m_E[row];
    }
    const MComplex &ModelBase::getU(int row) const
    {
        assert(row < m_numNodes);
        return m_U[row];
    }
    const MComplex &ModelBase::getJ(int row) const
    {
        assert(row < m_numVS);
        return m_J[row];
    }

    /* MNA matrices */
    void ModelBase::setY(int row, int col, const MComplex &val)
    {
        assert(row < m_numNodes && col < m_numNodes);
        m_Y[row * m_numNodes + col] = val;
    }
    void ModelBase::setB(int row, int col, const MComplex &val)
    {
        assert(row < m_numNodes && col < m_numVS);
        m_B[row * m_numVS + col] = val;
    }
    void ModelBase::setC(int row, int col, const MComplex &val)
    {
        assert(row < m_numVS && col < m_numNodes);
        m_C[row * m_numNodes + col] = val;
    }
    void ModelBase::setD(int row, int col, const MComplex &val)
    {
        assert(row < m_numVS && col < m_numVS);
        m_D[row * m_numVS + col] = val;
    }
    void ModelBase::setI(int row, const MComplex &val)
    {
        assert(row < m_numNodes);
        m_I[row] = val;
    }
    void ModelBase::setE(int row, const MComplex &val)
    {
        assert(row < m_numVS);
        m_E[row] = val;
    }
    void ModelBase::setU(int row, const MComplex &val)
    {
        assert(row < m_numNodes);
        m_U[row] = val;
    }
    void ModelBase::setJ(int row, const MComplex &val)
    {
        assert(row < m_numVS);
        m_J[row] = val;
    }

}
