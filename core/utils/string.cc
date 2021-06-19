/**
 * @file String
 */

/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include <cctype>
#include "csim/utils/string.h"

namespace csim
{
    std::string toUpper(const std::string &src)
    {
        std::string ret(src);
        for (size_t i = 0; i < ret.size(); i++)
            ret[i] = std::toupper(ret[i]);

        return ret;
    }
    int strcmp_nocase(const char *src, const char *dst)
    {
        while (*src && *dst && std::toupper(*src) == std::toupper(*dst))
        {
            src++, dst++;
        }
        return *src - *dst;
    }
}