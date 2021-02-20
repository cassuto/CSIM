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
        virtual int iterateTR() = 0;

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

    protected:
        void resizeModel(int numTermls, int numInnerNodes, int numVS);

        /* MNA matrices */
        void setY(unsigned int row, unsigned int col, const MComplex &val);
        void addY(unsigned int row, unsigned int col, const MComplex &delta);
        void setB(unsigned int row, unsigned int col, const MComplex &val);
        void setC(unsigned int row, unsigned int col, const MComplex &val);
        void setD(unsigned int row, unsigned int col, const MComplex &val);
        void setI(unsigned int row, const MComplex &val);
        void addI(unsigned int row, const MComplex &delta);
        void setE(unsigned int row, const MComplex &val);
        void setU(unsigned int row, const MComplex &val);
        void addU(unsigned int row, const MComplex &delta);
        void setJ(unsigned int row, const MComplex &val);

    private:
        csim::Circuit *m_circuit;
        PropertyBag m_props;
        std::vector<int> m_innerNodes;
        std::vector<int> m_termls;
        std::vector<int> m_VS;
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
