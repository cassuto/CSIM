/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition; you can redistribute it and/or           
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation; either      
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

    int ModelBase::prepareOP()
    {
        return prepareDC(); /* Default case: Quiescent operating point analysis == DC analysis */
    }
    int ModelBase::iterateOP()
    {
        return iterateDC(); /* Default case: Quiescent operating point analysis == DC analysis */
    }

    int ModelBase::prepareTrOP()
    {
        return 0; /* Default case: no transient operating point analysis */
    }
    int ModelBase::iterateTrOP()
    {
        return 0; /* Default case: no transient operating point analysis */
    }

    int ModelBase::saveOP()
    {
        return 0; /* Default: not a non-linear device and no need to store operating point */
    }

    void ModelBase::stepChangedTR(double tTime, double nstep)
    {
        (void)tTime;
        (void)nstep;
    }

    void ModelBase::adaptStep(double *step)
    {
        (void)step;
    }

    bool ModelBase::checkConvergence()
    {
        return true; /* Default: no model-specific checks for convergence */
    }

    Variant ModelBase::queryStatus(unsigned int id, int *rc)
    {
        *rc = CERR_UNIMPLEMENTED;
        return Variant(Variant::VariantUnknown);
    }

    const char *ModelBase::name() const
    {
        return m_name.c_str();
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
    const MComplex &ModelBase::getY(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getA(row, col);
    }
    const MComplex &ModelBase::getB(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getA(row, m_circuit->netlist()->getNumNodes() + col);
    }
    const MComplex &ModelBase::getC(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getA(m_circuit->netlist()->getNumNodes() + row, col);
    }
    const MComplex &ModelBase::getD(unsigned int row, unsigned int col) const
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getA(m_circuit->netlist()->getNumNodes() + row, m_circuit->netlist()->getNumNodes() + col);
    }
    const MComplex &ModelBase::getI(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getZ(row);
    }
    const MComplex &ModelBase::getE(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getZ(m_circuit->netlist()->getNumNodes() + row);
    }
    const MComplex &ModelBase::getU(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getX(row);
    }
    const MComplex &ModelBase::getJ(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getX(m_circuit->netlist()->getNumNodes() + row);
    }
    const MComplex &ModelBase::getPrevU(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getPrevX(row);
    }
    const MComplex &ModelBase::getPrevJ(unsigned int row) const
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getPrevX(m_circuit->netlist()->getNumNodes() + row);
    }

    double *ModelBase::getYPtr(unsigned int row, unsigned int col)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getAPtr(row, col);
    }

    double *ModelBase::getBPtr(unsigned int row, unsigned int col)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getAPtr(row, m_circuit->netlist()->getNumNodes() + col);
    }

    double *ModelBase::getCPtr(unsigned int row, unsigned int col)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumNodes());
        return m_circuit->getAPtr(m_circuit->netlist()->getNumNodes() + row, col);
    }

    double *ModelBase::getDPtr(unsigned int row, unsigned int col)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumVS());
        return m_circuit->getAPtr(m_circuit->netlist()->getNumNodes() + row, m_circuit->netlist()->getNumNodes() + col);
    }

    double *ModelBase::getIPtr(unsigned int row)
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getZPtr(row);
    }

    double *ModelBase::getEPtr(unsigned int row)
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getZPtr(m_circuit->netlist()->getNumNodes() + row);
    }

    double *ModelBase::getUPtr(unsigned int row)
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        return m_circuit->getXPtr(row);
    }

    double *ModelBase::getJPtr(unsigned int row)
    {
        assert(row < m_circuit->netlist()->getNumVS());
        return m_circuit->getXPtr(m_circuit->netlist()->getNumNodes() + row);
    }

    void ModelBase::addY(unsigned int row, unsigned int col, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumNodes());
        m_circuit->addA(row, col, delta);
    }
    void ModelBase::addB(unsigned int row, unsigned int col, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumNodes() && col < m_circuit->netlist()->getNumVS());
        m_circuit->addA(row, m_circuit->netlist()->getNumNodes() + col, delta);
    }
    void ModelBase::addC(unsigned int row, unsigned int col, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumNodes());
        m_circuit->addA(m_circuit->netlist()->getNumNodes() + row, col, delta);
    }
    void ModelBase::addD(unsigned int row, unsigned int col, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumVS() && col < m_circuit->netlist()->getNumVS());
        m_circuit->addA(m_circuit->netlist()->getNumNodes() + row, m_circuit->netlist()->getNumNodes() + col, delta);
    }
    void ModelBase::addI(unsigned int row, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumNodes());
        m_circuit->addZ(row, delta);
    }
    void ModelBase::addE(unsigned int row, const MComplex &delta)
    {
        assert(row < m_circuit->netlist()->getNumVS());
        m_circuit->addZ(m_circuit->netlist()->getNumNodes() + row, delta);
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

    void ModelBase::registerIntegralU(unsigned int col)
    {
        m_circuit->registerIntegralU(col);
    }
    void ModelBase::registerIntegralJ(unsigned int col)
    {
        m_circuit->registerIntegralJ(col);
    }

    double ModelBase::integrate(unsigned int nint, double x, double k, double *c0, double *c1)
    {
        assert(nint < m_numIntegrators);
        m_historyX[nint].set(0, x);
        return integrate(nint, k, c0, c1);
    }

    double ModelBase::integrate(unsigned int nint, double k, double *c0, double *c1)
    {
        assert(nint < m_numIntegrators);
        csim::IntegralHistory *hX = &m_historyX[nint];
        csim::IntegralHistory *hY = &m_historyY[nint];
        m_circuit->corrector()->integrate(hX, hY, k, c0, c1);
        return hY->get(0);
    }

    double ModelBase::getIntegratorCoeff(unsigned int index) const
    {
        return m_circuit->corrector()->getIntegralCoeff(index);
    }

    double ModelBase::getIntegratorTruncErrorCoeff() const
    {
        return m_circuit->corrector()->getTruncErrorCoeff();
    }

    unsigned int ModelBase::getIntegratorOrder() const
    {
        return m_circuit->corrector()->getOrder();
    }

    const csim::IntegralHistory *ModelBase::getIntegratorTimestep() const
    {
        return m_circuit->getTimestep();
    }

    Environment *ModelBase::getEnvironment() const
    {
        return m_circuit->environment();
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

    void ModelBase::registerAdoptStep()
    {
        m_circuit->registerAdoptStepCallback(this);
    }

    void ModelBase::registerStatus(const StatusDescriptor *array, int count)
    {
        /*TODO*/
    }

}
