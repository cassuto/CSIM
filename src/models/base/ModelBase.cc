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
#include "csim/internal/Circuit.h"
#include "csim/model/ModelBase.h"

namespace csimModel
{

    ModelBase::ModelBase(MODELBASE_CONSTRUCTOR_DEF)
        : m_circuit(MODELBASE_CONSTRUCTOR_VAR),
          m_numNodes(0)
    {
    }

    ModelBase::~ModelBase()
    {
    }

    void ModelBase::resizeModel(int numTermls, int numNodes, int numVS)
    {
        assert(numTermls && numNodes >= numTermls);
        m_termls.resize(numTermls);
        m_VS.resize(numVS);
        m_numNodes = numNodes;
    }

    PropertyBag &ModelBase::property()
    {
        return m_props;
    }

    int ModelBase::getNumTerml() const
    {
        return m_termls.size();
    }
    int ModelBase::getNumNode() const
    {
        return m_numNodes;
    }
    int ModelBase::getNumVS() const
    {
        return m_VS.size();
    }

    void ModelBase::setNode(int terml, int node)
    {
        m_termls[terml] = node;
    }
    int ModelBase::getNode(int terml) const
    {
        return m_termls[terml];
    }
    void ModelBase::setVS(int idx, int branch)
    {
        m_VS[idx] = branch;
    }
    int ModelBase::getVS(int idx) const
    {
        return m_VS[idx];
    }

    /* MNA matrices */
    MComplex ModelBase::getY(int row, int col) const
    {
        assert(row < m_circuit->getNumNodes() && col < m_circuit->getNumNodes());
        return m_circuit->getA(row, col).toMComplex();
    }
    MComplex ModelBase::getB(int row, int col) const
    {
        assert(row < m_circuit->getNumNodes() && col < m_circuit->getNumVS());
        return m_circuit->getA(row, m_circuit->getNumNodes() + col).toMComplex();
    }
    MComplex ModelBase::getC(int row, int col) const
    {
        assert(row < m_circuit->getNumVS() && col < m_circuit->getNumNodes());
        return m_circuit->getA(m_circuit->getNumNodes() + row, col).toMComplex();
    }
    MComplex ModelBase::getD(int row, int col) const
    {
        assert(row < m_circuit->getNumVS() && col < m_circuit->getNumVS());
        return m_circuit->getA(m_circuit->getNumNodes() + row, m_circuit->getNumNodes() + col).toMComplex();
    }
    MComplex ModelBase::getI(int row) const
    {
        assert(row < m_circuit->getNumNodes());
        return m_circuit->getZ(row).toMComplex();
    }
    MComplex ModelBase::getE(int row) const
    {
        assert(row < m_circuit->getNumVS());
        return m_circuit->getZ(m_circuit->getNumNodes() + row).toMComplex();
    }
    MComplex ModelBase::getU(int row) const
    {
        assert(row < m_circuit->getNumNodes());
        return m_circuit->getX(row).toMComplex();
    }
    MComplex ModelBase::getJ(int row) const
    {
        assert(row < m_circuit->getNumVS());
        return m_circuit->getX(m_circuit->getNumNodes() + row).toMComplex();
    }

    /* MNA matrices */
    void ModelBase::setY(int row, int col, const MComplex &val)
    {
        assert(row < m_circuit->getNumNodes() && col < m_circuit->getNumNodes());
        m_circuit->setA(row, col, val);
    }
    void ModelBase::addY(int row, int col, const MComplex &delta)
    {
        setY(row, col, getY(row, col) + delta);
    }
    void ModelBase::setB(int row, int col, const MComplex &val)
    {
        assert(row < m_circuit->getNumNodes() && col < m_circuit->getNumVS());
        m_circuit->setA(row, m_circuit->getNumNodes() + col, val);
    }
    void ModelBase::setC(int row, int col, const MComplex &val)
    {
        assert(row < m_circuit->getNumVS() && col < m_circuit->getNumNodes());
        m_circuit->setA(m_circuit->getNumNodes() + row, col, val);
    }
    void ModelBase::setD(int row, int col, const MComplex &val)
    {
        assert(row < m_circuit->getNumVS() && col < m_circuit->getNumVS());
        m_circuit->setA(m_circuit->getNumNodes() + row, m_circuit->getNumNodes() + col, val);
    }
    void ModelBase::setI(int row, const MComplex &val)
    {
        assert(row < m_circuit->getNumNodes());
        m_circuit->setZ(row, val);
    }
    void ModelBase::addI(int row, const MComplex &delta)
    {
        setI(row, getI(row) + delta);
    }
    void ModelBase::setE(int row, const MComplex &val)
    {
        assert(row < m_circuit->getNumVS());
        m_circuit->setZ(m_circuit->getNumNodes() + row, val);
    }
    void ModelBase::setU(int row, const MComplex &val)
    {
        assert(row < m_circuit->getNumNodes());
        m_circuit->setX(row, val);
    }
    void ModelBase::addU(int row, const MComplex &delta)
    {
        setU(row, getU(row) + delta);
    }
    void ModelBase::setJ(int row, const MComplex &val)
    {
        assert(row < m_circuit->getNumVS());
        m_circuit->setX(m_circuit->getNumNodes() + row, val);
    }

}
