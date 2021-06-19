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

#ifndef CSIM_GMINOPTIMIZER_H_
#define CSIM_GMINOPTIMIZER_H_

#include "csim/model/Types.h"
#include "csim/internal/LinearSolverOptimizer.h"

namespace csim
{
    class Netlist;
    class GminOptimizer : public LinearSolverOptimizer
    {
    public:
        GminOptimizer(Netlist *netlist);
        void reset();
        int singularDiag(csimModel::MComplex *A, unsigned curRow, unsigned int nRows);

        inline void enableGmin(bool enabled)
        {
            m_gminEnabled = enabled;
        }
        inline bool getGminEnabled() const
        {
            return m_gminEnabled;
        }
        inline void setGmin(double g)
        {
            m_gmin = g;
        }

    private:
        Netlist *m_netlist;
        bool m_gminEnabled;
        double m_gmin;
        unsigned int m_lastFixRow;
    };
}

#endif // CSIM_GMINOPTIMIZER_H_