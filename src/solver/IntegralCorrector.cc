/**
 * @file Numberic integral solver
 */

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

#include <cassert>
#include "IntegralCorrectorGear.h"
#include "csim/internal/IntegralCorrector.h"

namespace csim
{

    IntegralHistory::IntegralHistory()
        : m_queueHead(0)
    {
        for (int i = 0; i < MaxNumHistoryNum; ++i)
            pushX(0.0);
        assert(m_queueHead == MaxNumHistoryNum - 1);
    }

    double IntegralHistory::getX(unsigned int delay) const
    {
        assert(delay < MaxNumHistoryNum);
        return m_queue[(MaxNumHistoryNum + m_queueHead - 1 - delay) & MaxNumHistoryMask];
    }
    void IntegralHistory::pushX(double val)
    {
        m_queue[m_queueHead] = val;
        m_queueHead = (m_queueHead + 1) & MaxNumHistoryMask;
    }

    void IntegralHistory::setInitial(double val)
    {
        for (int i = 0; i < MaxNumHistoryNum; ++i)
            pushX(val);
    }

    /**
     * @brief Create instance of integral corrector by its algorithm name
     * @param algorithm Name of algorithm
     * @retval nullptr if failed
     * @retval Pointer to the integral corrector.
     */
    IntegralCorrector *IntegralCorrector::createInstance(const std::string &algorithm)
    {
        if (0 == algorithm.compare("gear"))
        {
            return new IntegralCorrectorGear();
        }
        else
        {
            return nullptr;
        }
    }
}
