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

#ifndef CSIM_MODELLOADER_H_
#define CSIM_MODELLOADER_H_

#include <map>
#include "csim/model/ModelBase.h"

namespace csimModel
{
    class ModelBase;
}

namespace csim
{
    class Circuit;

    class ModelEntry
    {
    public:
        ModelEntry(void *handle, csimModel::pfnCreateModel_t pfnCreate, csimModel::pfnDeleteModel_t pfnDelete,
                   const ModelDescriptor *descriptor,
                   const PropertyMdlDescriptor *mdlDescriptors,
                   size_t numMdlDescriptors);
        ~ModelEntry();

        csimModel::ModelBase *createInstance(MODELBASE_CONSTRUCTOR_DEF) const;

        void deleteInstance(csimModel::ModelBase *model) const;

        inline const ModelDescriptor *descriptor() const
        {
            return m_descriptor;
        }

        inline size_t numMdlDescriptors() const
        {
            return m_numMdlDescriptors;
        }

        inline const PropertyMdlDescriptor *mdlDescriptors(size_t i) const
        {
            return m_mdlDescriptors + i;
        }

        class MdlEntry
        {
        public:
            csimModel::PropertyMdl *createInstance() const;
            void deleteInstance(csimModel::PropertyMdl *mdl) const;
            const PropertyMdlPropDescriptor *getProperty(const char *name) const;

        public:
            const PropertyMdlDescriptor *desc;
            std::map<std::string, const PropertyMdlPropDescriptor *> props;
        };

        int getMdlEntry(const char *mdl, const MdlEntry **out) const;

    private:
        int buildMdlIndexs();

    private:
        void *m_dllHandle;
        csimModel::pfnCreateModel_t m_pfnCreate;
        csimModel::pfnDeleteModel_t m_pfnDelete;
        const ModelDescriptor *m_descriptor;
        const PropertyMdlDescriptor *m_mdlDescriptors;
        size_t m_numMdlDescriptors;
        std::map<std::string, MdlEntry> m_mdls;
    };

    class ModelLoader
    {
    public:
        static ModelEntry *load(const char *filename);
    };

}

#endif // CSIM_MODELLOADER_H_
