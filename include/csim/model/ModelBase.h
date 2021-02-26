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

#ifndef CSIM_MODELBASE_H_
#define CSIM_MODELBASE_H_

#include <vector>
#include "csim/model/Types.h"
#include "csim/model/PropertyBag.h"

namespace csim
{
    class IntegralHistory;
}

namespace csimModel
{

#define MODELBASE_CONSTRUCTOR_DEF csim::Circuit *circuit_
#define MODELBASE_CONSTRUCTOR_VAR circuit_

    class ModelBase
    {
    public:
        ModelBase(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~ModelBase();

    public:
        virtual int configure() = 0;
        virtual int prepareDC() = 0;
        virtual int prepareAC() = 0;
        virtual int prepareTR() = 0;
        virtual int iterateDC() = 0;
        virtual int iterateAC(double omega) = 0;
        virtual int iterateTR(double tTime) = 0;

    public:
        PropertyBag &property();

        unsigned int getNumTerml() const;
        unsigned int getNumInnerNode() const;
        unsigned int getNumVS() const;
        void setNode(unsigned int terml, unsigned int node);
        unsigned int getNode(unsigned int terml) const;
        void setInnerNode(unsigned int index, unsigned int node);
        unsigned int getInnerNode(unsigned int index) const;
        void setVS(unsigned int idx, unsigned int branch);
        unsigned int getVS(unsigned int idx) const;

        /* MNA matrices */
        MComplex getY(unsigned int row, unsigned int col) const;
        MComplex getB(unsigned int row, unsigned int col) const;
        MComplex getC(unsigned int row, unsigned int col) const;
        MComplex getD(unsigned int row, unsigned int col) const;
        MComplex getI(unsigned int row) const;
        MComplex getE(unsigned int row) const;
        MComplex getU(unsigned int row) const;
        MComplex getJ(unsigned int row) const;

        unsigned int getNumIntegrators() const;
        csim::IntegralHistory *getIntegratorX(unsigned int nint);
        csim::IntegralHistory *getIntegratorY(unsigned int nint);

    protected:
        void resizeModel(unsigned int numTermls, unsigned int numInnerNodes, unsigned int numVS);
        void resizeIntegrator(unsigned int numIntegrators);
        double integrate(unsigned int nint, double x, double k, double *c0, double *c1);

        /* MNA matrices */
        void addY(unsigned int row, unsigned int col, const MComplex &delta);
        void addB(unsigned int row, unsigned int col, const MComplex &delta);
        void addC(unsigned int row, unsigned int col, const MComplex &delta);
        void addD(unsigned int row, unsigned int col, const MComplex &delta);
        void addI(unsigned int row, const MComplex &delta);
        void addE(unsigned int row, const MComplex &delta);

    private:
        csim::Circuit *m_circuit;
        PropertyBag m_props;
        std::vector<unsigned int> m_termls;
        std::vector<unsigned int> m_innerNodes;
        std::vector<unsigned int> m_VS;
        csim::IntegralHistory *m_historyX, *m_historyY;
        unsigned int m_numIntegrators;
    };

    typedef ModelBase *(*pfnCreateModel_t)(MODELBASE_CONSTRUCTOR_DEF);
    typedef void (*pfnDeleteModel_t)(ModelBase *model);
}

extern "C"
{
    /*
     * Entry of Model Library.
     * Called by Model Loader
     */
    extern csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF);

    /*
     * Exit of Model Library.
     * Called by Model Loader
     */
    extern void deleteModel(csimModel::ModelBase *model);

    struct ModelDescriptor
    {
        const char *id;
        const char *description;
    };

    extern const ModelDescriptor descriptor;
}

#endif // CSIM_MODELBASE_H_
