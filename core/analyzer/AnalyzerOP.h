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

#ifndef CSIM_ANALYZEROP_H_
#define CSIM_ANALYZEROP_H_

#include "csim/internal/Analyzers.h"

namespace csim
{
    class AnalyzerOP : public AnalyzerBase
    {
    public:
        AnalyzerOP(Circuit *circuit);
        ~AnalyzerOP();

        virtual int analyze(Dataset *dataset);

        virtual int prepareMNA();
        virtual int iterateMNA();
    };
}

#endif // CSIM_ANALYZEROP_H_
