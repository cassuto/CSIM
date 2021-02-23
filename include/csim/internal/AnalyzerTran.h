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

#ifndef CSIM_ANALYZERTRAN_H_
#define CSIM_ANALYZERTRAN_H_

#include "csim/internal/Analyzers.h"

namespace csim
{
    class AnalyzerTran : public AnalyzerBase
    {
    public:
        AnalyzerTran(Circuit *circuit);
        ~AnalyzerTran();

        virtual int analyze();
        virtual double getPosition(unsigned int step);

        virtual int prepareMNA();
        virtual int iterateMNA();
    };
}

#endif // CSIM_ANALYZERAC_H_
