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

#ifndef CSIM_INTEGRALHISTORY_H_
#define CSIM_INTEGRALHISTORY_H_

namespace csim
{
    class IntegralHistory
    {
    public:
        IntegralHistory();

        double get(unsigned int delay) const;
        void push();
        void set(unsigned int delay, double val);
        void setInitial(double val);

    public:
        static const unsigned int MaxNumHistoryShift = 3; /* 2^3 */
        static const unsigned int MaxNumHistoryNum = (1U << MaxNumHistoryShift);
        static const unsigned int MaxNumHistoryMask = MaxNumHistoryNum - 1;

        //private:
    public:
        unsigned int m_queueHead;
        double m_queue[MaxNumHistoryNum];
    };

}

#endif // CSIM_INTEGRALHISTORY_H_