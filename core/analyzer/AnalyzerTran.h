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

#ifndef CSIM_ANALYZERTRAN_H_
#define CSIM_ANALYZERTRAN_H_

#include <vector>
#include "csim/internal/Analyzers.h"

namespace csim
{
    class Variable;
    class AnalyzerTran : public AnalyzerBase
    {
    public:
        AnalyzerTran(Circuit *circuit);
        ~AnalyzerTran();

        virtual int analyze(Dataset *dataset);

        virtual int prepareMNA();
        virtual int iterateMNA();

    private:
        void saveDataset(std::vector<Variable *> &dvolt, std::vector<Variable *> &dcurrent, Variable &dtime, double tTime, unsigned int N, unsigned int M);
    private:
        bool m_analyzeTrOP;
    };
}

#endif // CSIM_ANALYZERAC_H_
