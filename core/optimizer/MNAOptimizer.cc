/**
 * @file Optimizer for linear equations solver
 */

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

#include <iostream>
#include <cstring>
#include "csim/internal/Netlist.h"
#include "csim/utils/errors.h"
#include "csim/internal/MNAOptimizer.h"

namespace csim
{

    MNAOptimizer::MNAOptimizer(Netlist *netlist)
        : m_netlist(netlist),
          m_gminEnabled(true),
          m_gmin(1e-12)
    {
    }

    void MNAOptimizer::reset()
    {
        m_lastFixRow = (unsigned int)-1;
    }

    int MNAOptimizer::singularRow(csimModel::MComplex *A, unsigned curRow, unsigned int nRows)
    {
        int ret = CERR_SINGULAR_MATRIX;
        if (getGminEnabled())
        {
            /* Insert a small resistor to ground */
            A[curRow * nRows + curRow] = m_gmin;
            ret = 0;

            if (curRow != m_lastFixRow)
            {
                if (curRow < m_netlist->getNumNodes())
                    std::cout << "A large resistor is inserted between the node" << curRow << " and ground." << std::endl;
                else
                    std::cout << "A tiny resistor is inserted at branch" << curRow << std::endl;
                m_lastFixRow = curRow;
            }
        }

        return ret;
    }
}
