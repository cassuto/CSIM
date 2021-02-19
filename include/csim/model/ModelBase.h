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
        virtual void configure() = 0;
        virtual void prepareDC() = 0;
        virtual void prepareAC() = 0;
        virtual void prepareTR() = 0;

    public:
        PropertyBag &property();

        int getNumTerml() const;
        int getNumNode() const;
        int getNumVS() const;
        void setNode(int terml, int node);
        int getNode(int terml) const;
        void setVS(int idx, int branch);
        int getVS(int idx) const;

        /* MNA matrices */
        MComplex getY(int row, int col) const;
        MComplex getB(int row, int col) const;
        MComplex getC(int row, int col) const;
        MComplex getD(int row, int col) const;
        MComplex getI(int row) const;
        MComplex getE(int row) const;
        MComplex getU(int row) const;
        MComplex getJ(int row) const;

    protected:
        void resizeModel(int numTermls, int numNodes, int numVS);

        /* MNA matrices */
        void setY(int row, int col, const MComplex &val);
        void addY(int row, int col, const MComplex &delta);
        void setB(int row, int col, const MComplex &val);
        void setC(int row, int col, const MComplex &val);
        void setD(int row, int col, const MComplex &val);
        void setI(int row, const MComplex &val);
        void addI(int row, const MComplex &delta);
        void setE(int row, const MComplex &val);
        void setU(int row, const MComplex &val);
        void addU(int row, const MComplex &delta);
        void setJ(int row, const MComplex &val);

    private:
        csim::Circuit *m_circuit;
        PropertyBag m_props;
        int m_numNodes;
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

    extern const ModelDescriptor *descriptor;
}

#endif // CSIM_MODELBASE_H_
