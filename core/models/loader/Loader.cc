/**
 * @file Model Library Loader
 */

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

#if defined(CSIM_IN_WIN32)
#include <Windows.h>
#elif defined(CSIM_IN_UNIX)
#include <dlfcn.h>
#include <cstdlib>
#else
#error Unsupported OS!
#endif
#include <cstdio>
#include "csim/utils/string.h"
#include "csim/utils/errors.h"
#include "csim/internal/ModelLoader.h"

namespace csim
{

    /**
     * @brief Load a Model Library into memory
     * @param filename Pathname of the target DLL/so/...
     * @retval NULL if failed
     * @retval Pointer to the Model Entry
     */
    ModelEntry *ModelLoader::load(const char *filename)
    {
        csimModel::pfnCreateModel_t pfnCreate = nullptr;
        csimModel::pfnDeleteModel_t pfnDelete = nullptr;
        const ModelDescriptor *descriptor = nullptr;
        const PropertyMdlDescriptor **pMdlDescriptors = nullptr;
        size_t *pNumMdlDescriptors = nullptr;
        const PropertyMdlDescriptor *mdlDescriptors;
        size_t numMdlDescriptors;

#if defined(CSIM_IN_WIN32)
        HINSTANCE handle = LoadLibraryA(filename);
        if (!handle)
        {
            return nullptr;
        }

        pfnCreate = (csimModel::pfnCreateModel_t)GetProcAddress(handle, "createModel");
        if (!pfnCreate)
        {
            goto error_out;
        }

        pfnDelete = (csimModel::pfnDeleteModel_t)GetProcAddress(handle, "deleteModel");
        if (!pfnDelete)
        {
            goto error_out;
        }

        descriptor = (const ModelDescriptor *)GetProcAddress(handle, "descriptor");
        if (!descriptor)
        {
            goto error_out;
        }

        pMdlDescriptors = (const PropertyMdlDescriptor **)GetProcAddress(handle, "mdlDescriptors");

        pNumMdlDescriptors = (size_t *)GetProcAddress(handle, "numMdlDescriptors");

        goto success;
    error_out:
        FreeLibrary(handle);
        return nullptr;
    success:

#elif defined(CSIM_IN_UNIX)
        void *handle = dlopen(filename, RTLD_LAZY);
        if (!handle)
        {
            fprintf(stderr, "ERROR: ModelLoader::load() %s \n", dlerror());
            return nullptr;
        }
        char *error;

        pfnCreate = (csimModel::pfnCreateModel_t)dlsym(handle, "createModel");
        if (!pfnCreate || ((error = dlerror()) != NULL))
        {
            goto error_out;
        }

        pfnDelete = (csimModel::pfnDeleteModel_t)dlsym(handle, "deleteModel");
        if (!pfnDelete || ((error = dlerror()) != NULL))
        {
            goto error_out;
        }

        descriptor = (const ModelDescriptor *)dlsym(handle, "descriptor");
        if (!descriptor || ((error = dlerror()) != NULL))
        {
            goto error_out;
        }

        pMdlDescriptors = (const PropertyMdlDescriptor **)dlsym(handle, "mdlDescriptors");

        pNumMdlDescriptors = (size_t *)dlsym(handle, "numMdlDescriptors");

        goto success;
    error_out:
        dlclose(handle);
        return nullptr;
    success:
#endif

        mdlDescriptors = pMdlDescriptors ? *pMdlDescriptors : nullptr;
        numMdlDescriptors = pNumMdlDescriptors ? *pNumMdlDescriptors : 0;

        ModelEntry *model = new ModelEntry(handle, pfnCreate, pfnDelete, descriptor, mdlDescriptors, numMdlDescriptors);
        return model;
    }

    ModelEntry::ModelEntry(void *handle, csimModel::pfnCreateModel_t pfnCreate, csimModel::pfnDeleteModel_t pfnDelete,
                           const ModelDescriptor *descriptor,
                           const PropertyMdlDescriptor *mdlDescriptors,
                           size_t numMdlDescriptors)
        : m_dllHandle(handle),
          m_pfnCreate(pfnCreate),
          m_pfnDelete(pfnDelete),
          m_descriptor(descriptor),
          m_mdlDescriptors(mdlDescriptors),
          m_numMdlDescriptors(numMdlDescriptors)
    {
        buildMdlIndexs();
    }

    ModelEntry::~ModelEntry()
    {
#if defined(CSIM_IN_WIN32)
        FreeLibrary((HINSTANCE)m_dllHandle);
#elif defined(CSIM_IN_UNIX)
        dlclose(m_dllHandle);
#endif
    }

    csimModel::ModelBase *ModelEntry::createInstance(MODELBASE_CONSTRUCTOR_DEF) const
    {
        return m_pfnCreate(MODELBASE_CONSTRUCTOR_VAR);
    }

    void ModelEntry::deleteInstance(csimModel::ModelBase *model) const
    {
        m_pfnDelete(model);
    }

    int ModelEntry::buildMdlIndexs()
    {
        std::string mdlName, propName;
        for (size_t i = 0; i < m_numMdlDescriptors; i++)
        {
            mdlName = toUpper(m_mdlDescriptors[i].name);
            if (m_mdls.find(mdlName) != m_mdls.end())
                return CERR_MDL_EXISTING;
            MdlEntry &mdl = m_mdls[mdlName];
            mdl.desc = &m_mdlDescriptors[i];

            for (size_t j = 0; j < mdl.desc->numEntries; j++)
            {
                const PropertyMdlPropDescriptor *prop = &mdl.desc->entries[j];
                propName = toUpper(prop->name);
                if (mdl.props.find(propName) != mdl.props.end())
                    return CERR_MDL_PROP_EXISTING;
                mdl.props[propName] = prop;
            }
        }
        return 0;
    }

    int ModelEntry::getMdlEntry(const char *mdl, const MdlEntry **out) const
    {
        std::string mdlName = toUpper(mdl);
        if (m_mdls.find(mdlName) == m_mdls.end())
            return CERR_NO_SUCH_MDL;
        *out = &m_mdls.at(mdlName);
        return 0;
    }

    csimModel::PropertyMdl *ModelEntry::MdlEntry::createInstance() const
    {
        return desc->pfnCreate();
    }
    void ModelEntry::MdlEntry::deleteInstance(csimModel::PropertyMdl *mdl) const
    {
        desc->pfnDelete(mdl);
    }

    const PropertyMdlPropDescriptor *ModelEntry::MdlEntry::getProperty(const char *name) const
    {
        std::string uniname = toUpper(name);
        if (props.find(uniname) == props.end())
            return nullptr;
        return props.at(uniname);
    }

}
