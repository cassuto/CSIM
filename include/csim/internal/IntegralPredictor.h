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

#ifndef CSIM_INTEGRALPREDICTOR_H_
#define CSIM_INTEGRALPREDICTOR_H_

#include "csim/internal/IntegralCorrector.h"

namespace csim
{
    class IntegralPredictor
    {
    public:
        IntegralPredictor() : m_order(0) {}
        virtual ~IntegralPredictor() {}

        virtual int setOrder(unsigned int order, const IntegralHistory *hsteps)
        {
            m_order = order;
            return 0;
        }
        virtual unsigned int getOrder() const
        {
            return m_order;
        }
        virtual int setStep(const IntegralHistory *hsteps) { return 0; }
        virtual double predict(const IntegralHistory *x, const IntegralHistory *hsteps) = 0;
        virtual double getTruncErrorCoeff() = 0;

    public:
        static IntegralPredictor *createInstance(const std::string &algorithm);

    protected:
        unsigned int m_order;
    };
}

#endif // CSIM_INTEGRALPREDICTOR_H_