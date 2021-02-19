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
        csimModel::pfnCreateModel_t pfnCreate = 0l;
        csimModel::pfnDeleteModel_t pfnDelete = 0l;
        const ModelDescriptor *descriptor = 0l;

#if defined(CSIM_IN_WIN32)

#elif defined(IN_UNIX)
        void *handle = dlopen(filename, RTLD_LAZY);
        if (!handle)
        {
            fprintf(stderr, "ERROR: ModelLoader::load() %s \n", dlerror());
            return 0l;
        }

        pfnCreate = (csimModel::pfnCreateModel_t)dlsym(handle, "createModel");
        if (!pfnCreate || ((error = dlerror()) != NULL))
        {
            fprintf(stderr, "ERROR: pfnCreate() %s \n", error);
            return 0l;
        }

        pfnDelete = (csimModel::pfnDeleteModel_t)dlsym(handle, "deleteModel");
        if (!pfnDelete || ((error = dlerror()) != NULL))
        {
            fprintf(stderr, "ERROR: pfnDelete() %s \n", error);
            return 0l;
        }

        descriptor = (const ModelDescriptor *)dlsym(handle, "descriptor");
        if (!descriptor || ((error = dlerror()) != NULL))
        {
            fprintf(stderr, "ERROR: descriptor %s \n", error);
            return 0l;
        }
#endif

        ModelEntry *model = new ModelEntry(pfnCreate, pfnDelete, descriptor);
        return model;
    }

    csimModel::ModelBase *ModelEntry::createInstance(MODELBASE_CONSTRUCTOR_DEF)
    {
        return m_pfnCreate(MODELBASE_CONSTRUCTOR_VAR);
    }

}
