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
#ifndef CSIM_UTILS_STRING_H_
#define CSIM_UTILS_STRING_H_

#include <string>
#include <cstring>

namespace csim
{
    std::string toUpper(const std::string &src);
    int strcmp_nocase(const char *src, const char *dst);
}

#endif // CSIM_UTILS_STRING_H_
