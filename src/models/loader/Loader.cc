/**
 * @file Model Library Loader
 */

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

#if defined(CSIM_IN_WIN32)
#include <Windows.h>
#elif defined(CSIM_IN_UNIX)
#include <dlfcn.h>
#include <cstdlib>
#else
#error Unsupported OS!
#endif
#include <cstdio>
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

        goto success;
    error_out:
        dlclose(handle);
        return nullptr;
    success:
#endif

        ModelEntry *model = new ModelEntry(pfnCreate, pfnDelete, descriptor);
        return model;
    }

    csimModel::ModelBase *ModelEntry::createInstance(MODELBASE_CONSTRUCTOR_DEF) const
    {
        return m_pfnCreate(MODELBASE_CONSTRUCTOR_VAR);
    }

    void ModelEntry::deleteInstance(csimModel::ModelBase *model) const
    {
        m_pfnDelete(model);
    }

}
