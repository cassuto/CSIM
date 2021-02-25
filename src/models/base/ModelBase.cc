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
#include "csim/internal/Netlist.h"
#include "csim/internal/IntegralCorrector.h"
#include "csim/model/ModelBase.h"

namespace csimModel
{

    ModelBase::ModelBase(MODELBASE_CONSTRUCTOR_DEF)
        : m_circuit(MODELBASE_CONSTRUCTOR_VAR),
          m_historyX(nullptr), m_historyY(nullptr),
          m_numIntegrators(0U)
    {
    }

    ModelBase::~ModelBase()
    {
        delete[] m_historyX;
        delete[] m_historyY;
    }

    void ModelBase::resizeModel(unsigned int numTermls, unsigned int numInnerNodes, unsigned int numVS)
    {
        m_termls.resize(numTermls);
        m_innerNodes.resize(numInnerNodes);
        m_VS.resize(numVS);
    }

    PropertyBag &ModelBase::property()
    {
        return m_props;
    }

    unsigned int ModelBase::getNumTerml() const
    {
        return m_termls.size();
    }
    unsigned int ModelBase::getNumInnerNode() const
    {
        return m_innerNodes.size();
    }
    unsigned int ModelBase::getNumVS() const
    {
        return m_VS.size();
    }

    void ModelBase::setNode(unsigned int terml, unsigned int node)
    {
        m_termls[terml] = node;
    }
    unsigned int ModelBase::getNode(unsigned int terml) const
    {
        return m_termls[terml];
    }
    void ModelBase::setInnerNode(unsigned int index, unsigned int node)
    {
        m_innerNodes[index] = node;
    }
    unsigned int ModelBase::getInnerNode(unsigned int index) const
    {
        return m_innerNodes[index];
    }
    void ModelBase::setVS(unsigned int idx, unsigned int branch)
    {
        m_VS[idx] = branch;
    }
    unsigned int ModelBase::getVS(unsigned int idx) const
    {
        return m_VS[idx];
    }

    /* MNA matrices */
    MComplex ModelBase::getY(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getA(row, col).toMComplex();
    }
    MComplex ModelBase::getB(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getA(row, m_circuit->netlist()->getNumNodes() + col).toMComplex();
    }
    MComplex ModelBase::getC(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getA(m_circuit->netlist()->getNumNodes() + row, col).toMComplex();
    }
    MComplex ModelBase::getD(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getA(m_circuit->netlist()->getNumNodes() + row, m_circuit->netlist()->getNumNodes() + col).toMComplex();
    }
    MComplex ModelBase::getI(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getZ(row).toMComplex();
    }
    MComplex ModelBase::getE(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getZ(m_circuit->netlist()->getNumNodes() + row).toMComplex();
    }
    MComplex ModelBase::getU(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getX(row).toMComplex();
    }
    MComplex ModelBase::getJ(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getX(m_circuit->netlist()->getNumNodes() + row).toMComplex();
    }

    void ModelBase::setY(unsigned int row, unsigned int col, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumNodes());
        m_circuit->setA(row, col, val);
    }
    void ModelBase::addY(unsigned int row, unsigned int col, const MComplex &delta)
    {
        setY(row, col, getY(row, col) + delta);
    }
    void ModelBase::setB(unsigned int row, unsigned int col, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumVS());
        m_circuit->setA(row, m_circuit->netlist()->getNumNodes() + col, val);
    }
    void ModelBase::setC(unsigned int row, unsigned int col, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumNodes());
        m_circuit->setA(m_circuit->netlist()->getNumNodes() + row, col, val);
    }
    void ModelBase::setD(unsigned int row, unsigned int col, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumVS());
        m_circuit->setA(m_circuit->netlist()->getNumNodes() + row, m_circuit->netlist()->getNumNodes() + col, val);
    }
    void ModelBase::setI(unsigned int row, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        m_circuit->setZ(row, val);
    }
    void ModelBase::addI(unsigned int row, const MComplex &delta)
    {
        setI(row, getI(row) + delta);
    }
    void ModelBase::setE(unsigned int row, const MComplex &val)
    {
        assert(row < m_circuit->netlist()->getNumVS());
        m_circuit->setZ(m_circuit->netlist()->getNumNodes() + row, val);
    }

    unsigned int ModelBase::getNumIntegrators() const
    {
        return m_numIntegrators;
    }

    void ModelBase::resizeIntegrator(unsigned int numIntegrators)
    {
        delete[] m_historyX;
        delete[] m_historyY;
        m_historyX = new csim::IntegralHistory[numIntegrators];
        m_historyY = new csim::IntegralHistory[numIntegrators];
        m_numIntegrators = numIntegrators;
    }

    double ModelBase::integrate(unsigned int nint, double x, double k, double *c0, double *c1)
    {
        assert(nint < m_numIntegrators);
        csim::IntegralHistory *hX = &m_historyX[nint];
        csim::IntegralHistory *hY = &m_historyY[nint];
        hX->set(0, x);
        m_circuit->corrector()->integrate(hX, hY, k, c0, c1);
        return hY->get(0);
    }

    csim::IntegralHistory *ModelBase::getIntegratorX(unsigned int nint)
    {
        assert(nint < m_numIntegrators);
        return &m_historyX[nint];
    }
    csim::IntegralHistory *ModelBase::getIntegratorY(unsigned int nint)
    {
        assert(nint < m_numIntegrators);
        return &m_historyY[nint];
    }
}
