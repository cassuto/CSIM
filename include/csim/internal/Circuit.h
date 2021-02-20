/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#ifndef CSIM_CIRCUIT_H_
#define CSIM_CIRCUIT_H_

#include <vector>
#include "csim/internal/Complex.h"

namespace csimModel
{
    class ModelBase;
}

namespace csim
{

    class LinearSolver;
    class Netlist;

    class Circuit
    {
    public:
        Circuit();
        ~Circuit();

        /* MNA matrices */
        const Complex &getA(unsigned int row, unsigned int col) const;
        const Complex &getX(unsigned int row) const;
        const Complex &getZ(unsigned int row) const;

        void setA(unsigned int row, unsigned int col, const Complex &val);
        void setX(unsigned int row, const Complex &val);
        void setZ(unsigned int row, const Complex &val);

        inline Netlist *netlist() const
        {
            return m_netlist;
        }

        int analyseDC();
        int analyseAC();
        int startTR();

        Complex getNodeVolt(unsigned int node);

    private:
        void createMatrix(unsigned int numNodes, unsigned int numVS);
        int initMNA();
        int solveMNA();
        bool converged();

    private:
        unsigned int m_matrixRows;
        Complex *m_A, *m_x, *m_x_1, *m_z, *m_z_1;
        LinearSolver *m_linearSolver;
        Netlist *m_netlist;
        unsigned int m_maxIterations;
        double m_VepsMax, m_VepsrMax;
        double m_IepsMax, m_IepsrMax;
    };

}

#endif // CSIM_CIRCUIT_H_
