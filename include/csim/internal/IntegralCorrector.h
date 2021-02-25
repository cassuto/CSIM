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

#ifndef CSIM_INTEGRALCORRECTOR_H_
#define CSIM_INTEGRALCORRECTOR_H_

#include <cassert>
#include "csim/internal/Complex.h"

namespace csim
{
    class IntegralHistory
    {
    public:
        IntegralHistory();

        double get(unsigned int delay) const;
        void push(double val);
        void set(unsigned int delay, double val);
        void setInitial(double val);

    public:
        static const unsigned int MaxNumHistoryShift = 3; /* 2^3 */
        static const unsigned int MaxNumHistoryNum = (1U << MaxNumHistoryShift);
        static const unsigned int MaxNumHistoryMask = MaxNumHistoryNum - 1;

    private:
        unsigned int m_queueHead;
        double m_queue[MaxNumHistoryNum];
    };

    class IntegralCorrector
    {
    public:
        IntegralCorrector() {}
        virtual ~IntegralCorrector() {}

        virtual void setStep(double step)
        {
            m_step = step;
        }
        virtual double getStep() const
        {
            return m_step;
        }
        virtual void setOrder(unsigned int order)
        {
            m_order = order;
        }
        virtual unsigned int getOrder() const
        {
            return m_order;
        }
        virtual void integrate(const IntegralHistory *x, IntegralHistory *y, double k, double *c0, double *c1) = 0;

    public:
        static IntegralCorrector *createInstance(const std::string &algorithm);

    protected:
        double m_step;
        unsigned int m_order;
    };

}

#endif // CSIM_INTEGRALCORRECTOR_H_