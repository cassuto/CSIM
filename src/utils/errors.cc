/**
 * @file Error codes
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cstring>
#include "csim/utils/errors.h"

namespace csim
{
    static const ErrorInfo errorDescriptors[] = {
#include "errors-generated.h"
        {"Unknown", "Unknown status", "UNKNOWN", 0}};

    static const unsigned numErrors = sizeof(errorDescriptors) / sizeof(errorDescriptors[0]);

    /**
     * Get the messages by error code.
     * @param   rc          Returned status code.
     * @return A poninter to ErrorInfo struct whose content is
     *         read only, if the status code is matched.
     * @return Default 'UNKNOWN' struct if rc is not matched.
     */
    const ErrorInfo &ErrorInfo::get(int rc)
    {
        if (rc < 0)
            rc = -rc;
        for (unsigned i = 0; i < numErrors; i++)
        {
            if (errorDescriptors[i].code == rc)
                return errorDescriptors[i];
        }
        return errorDescriptors[numErrors - 1];
    }
}
