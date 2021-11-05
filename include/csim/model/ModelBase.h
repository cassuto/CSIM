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

#ifndef CSIM_MODELBASE_H_
#define CSIM_MODELBASE_H_

#include <string>
#include <vector>
#include "csim/model/Types.h"
#include "csim/model/PropertyBag.h"
#include "csim/model/PropertyMdl.h"
#include "csim/utils/errors.h"

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
        virtual int prepareOP();
        virtual int prepareTrOP();
        virtual int prepareAC() = 0;
        virtual int prepareTR() = 0;
        virtual int iterateDC() = 0;
        virtual int iterateOP();
        virtual int iterateTrOP();
        virtual int iterateAC(double omega) = 0;
        virtual int iterateTR(double tTime) = 0;
        virtual int saveOP();
        virtual int loadTempature();
        virtual void stepChangedTR(double tTime, double nstep);
        virtual void adaptStep(double *step);
        virtual bool checkConvergence();
        virtual Variant queryStatus(unsigned int id, int *rc);

        const char *name() const;

    public:
        PropertyBag &property();
        void setName(const char *name);
        unsigned int getNumTerml() const;
        unsigned int getNumBranches() const;
        unsigned int getNumInnerNode() const;
        void setNode(unsigned int idx, unsigned int node);
        unsigned int getNode(unsigned int idx) const;
        void setBranch(unsigned int idx, unsigned int branch);
        unsigned int getBranch(unsigned int idx) const;
        void setInnerNode(unsigned int innerIdx, unsigned int node);

        /* MNA matrices */
        const MComplex &getY(unsigned int row, unsigned int col) const;
        const MComplex &getB(unsigned int row, unsigned int col) const;
        const MComplex &getC(unsigned int row, unsigned int col) const;
        const MComplex &getD(unsigned int row, unsigned int col) const;
        const MComplex &getI(unsigned int row) const;
        const MComplex &getE(unsigned int row) const;
        const MComplex &getU(unsigned int row) const;
        const MComplex &getJ(unsigned int row) const;
        const MComplex &getPrevU(unsigned int row) const;
        const MComplex &getPrevJ(unsigned int row) const;

        double *getYPtr(unsigned int row, unsigned int col);
        double *getBPtr(unsigned int row, unsigned int col);
        double *getCPtr(unsigned int row, unsigned int col);
        double *getDPtr(unsigned int row, unsigned int col);
        double *getIPtr(unsigned int row);
        double *getEPtr(unsigned int row);
        double *getUPtr(unsigned int row);
        double *getJPtr(unsigned int row);

        unsigned int getNumIntegrators() const;
        csim::IntegralHistory *getIntegratorX(unsigned int nint);
        csim::IntegralHistory *getIntegratorY(unsigned int nint);

    protected:
        void resizeModel(unsigned int numTermls, unsigned int numInnerNodes, unsigned int numBranches);
        void resizeIntegrator(unsigned int numIntegrators);
        void registerIntegralU(unsigned int col);
        void registerIntegralJ(unsigned int col);
        double integrate(unsigned int nint, double x, double k, double *c0, double *c1);
        double integrate(unsigned int nint, double k, double *c0, double *c1);
        double getIntegratorCoeff(unsigned int index) const;
        double getIntegratorTruncErrorCoeff() const;
        unsigned int getIntegratorOrder() const;
        const csim::IntegralHistory *getIntegratorTimestep() const;
        Environment *getEnvironment() const;

        void registerAdoptStep();

        class StatusDescriptor
        {
        public:
            const char *name;
            Variant::VariantType type;
            const char *desc;
            unsigned int id;
        };

        void registerStatus(const StatusDescriptor *array, int count);

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
        unsigned int m_numTermls;
        std::vector<unsigned int> m_nodes; /* Voltage nodes */
        std::vector<unsigned int> m_branches; /* Current branches */
        csim::IntegralHistory *m_historyX, *m_historyY;
        unsigned int m_numIntegrators;
        std::string m_name;
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

    struct PropertyMdlPropDescriptor
    {
        const char *name;
        int id;
        csimModel::Variant::VariantType type;
        const char *desc;
        uint32_t flags;
    };

    typedef csimModel::PropertyMdl *(*pfnCreateMdl)();
    typedef void (*pfnDeleteMdl)(csimModel::PropertyMdl *mdl);

    struct PropertyMdlDescriptor
    {
        const char *name;
        const PropertyMdlPropDescriptor *entries;
        size_t numEntries;
        pfnCreateMdl pfnCreate;
        pfnDeleteMdl pfnDelete;
    };

    /*
     * Exported symbols for model
     */
    extern const ModelDescriptor descriptor;
    extern const PropertyMdlDescriptor *mdlDescriptors;
    extern const size_t numMdlDescriptors;
}

#endif // CSIM_MODELBASE_H_
