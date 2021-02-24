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

#ifndef CSIM_MODELLOADER_H_
#define CSIM_MODELLOADER_H_

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
        ModelEntry(csimModel::pfnCreateModel_t pfnCreate, csimModel::pfnDeleteModel_t pfnDelete, const ModelDescriptor *descriptor)
            : m_pfnCreate(pfnCreate),
              m_pfnDelete(pfnDelete),
              m_descriptor(descriptor)
        {
        }

        csimModel::ModelBase *createInstance(MODELBASE_CONSTRUCTOR_DEF) const;

        void deleteInstance(csimModel::ModelBase *model) const;

        inline const ModelDescriptor *descriptor() const
        {
            return m_descriptor;
        }

    private:
        csimModel::pfnCreateModel_t m_pfnCreate;
        csimModel::pfnDeleteModel_t m_pfnDelete;
        const ModelDescriptor *m_descriptor;
    };

    class ModelLoader
    {
    public:
        static ModelEntry *load(const char *filename);
    };

}

#endif // CSIM_MODELLOADER_H_
