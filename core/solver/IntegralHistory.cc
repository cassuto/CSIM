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

#include <cassert>
#include "csim/internal/IntegralHistory.h"

namespace csim
{
    IntegralHistory::IntegralHistory()
        : m_queueHead(0)
    {
        for (unsigned int i = 0; i < MaxNumHistoryNum; ++i)
        {
            set(0, 0.0);
            push();
        }
        assert(m_queueHead == 0);
    }

    double IntegralHistory::get(unsigned int delay) const
    {
        assert(delay < MaxNumHistoryNum);
        return m_queue[(MaxNumHistoryNum + m_queueHead - delay) & MaxNumHistoryMask];
    }
    void IntegralHistory::push()
    {
        m_queueHead = (m_queueHead + 1) & MaxNumHistoryMask;
        m_queue[m_queueHead]=0.0;
    }
    void IntegralHistory::set(unsigned int delay, double val)
    {
        assert(delay < MaxNumHistoryNum);
        m_queue[(MaxNumHistoryNum + m_queueHead - delay) & MaxNumHistoryMask] = val;
    }

    void IntegralHistory::setInitial(double val)
    {
        m_queueHead = 0;
        for (unsigned int i = 0; i < MaxNumHistoryNum; ++i)
        {
            m_queue[i] = val;
        }
    }
}
