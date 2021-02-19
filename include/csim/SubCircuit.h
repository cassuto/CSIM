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

#ifndef CSIM_CIRCUIT_H_
#define CSIM_CIRCUIT_H_

#include <cassert>
#include "csim/Types.h"

namespace csim
{

    class SubCircuit
    {
    public:
        SubCircuit(int numNodes);
        ~SubCircuit();

        /* MNA matrices */
        inline const Complex &getY(int row, int col) const
        {
            assert(row < m_numNodes && col < m_numNodes);
            return m_Y[row * m_numNodes + col];
        }
        inline const Complex &getB(int row, int col) const
        {
            assert(row < m_numNodes && col < m_numVS);
            return m_B[row * m_numVS + col];
        }
        inline const Complex &getC(int row, int col) const
        {
            assert(row < m_numVS && col < m_numNodes);
            return m_C[row * m_numNodes + col];
        }
        inline const Complex &getD(int row, int col) const
        {
            assert(row < m_numVS && col < m_numVS);
            return m_D[row * m_numVS + col];
        }
        inline const Complex &getI(int row) const
        {
            assert(row < m_numNodes);
            return m_I[row];
        }
        inline const Complex &getE(int row) const
        {
            assert(row < m_numVS);
            return m_E[row];
        }
        inline const Complex &getU(int row) const
        {
            assert(row < m_numNodes);
            return m_U[row];
        }
        inline const Complex &getJ(int row) const
        {
            assert(row < m_numVS);
            return m_J[row];
        }

    protected:
        inline void setNumVS(int num)
        {
            m_numVS = num;
        }

        void createMatrix();

        /* MNA matrices */
        inline void setY(int row, int col, const Complex &val)
        {
            assert(row < m_numNodes && col < m_numNodes);
            m_Y[row * m_numNodes + col] = val;
        }
        inline void setB(int row, int col, const Complex &val)
        {
            assert(row < m_numNodes && col < m_numVS);
            m_B[row * m_numVS + col] = val;
        }
        inline void setC(int row, int col, const Complex &val)
        {
            assert(row < m_numVS && col < m_numNodes);
            m_C[row * m_numNodes + col] = val;
        }
        inline void setD(int row, int col, const Complex &val)
        {
            assert(row < m_numVS && col < m_numVS);
            m_D[row * m_numVS + col] = val;
        }
        inline void setI(int row, const Complex &val)
        {
            assert(row < m_numNodes);
            m_I[row] = val;
        }
        inline void setE(int row, const Complex &val)
        {
            assert(row < m_numVS);
            m_E[row] = val;
        }
        inline const Complex &setU(int row, const Complex &val) const
        {
            assert(row < m_numNodes);
            m_U[row] = val;
        }
        inline const Complex &setJ(int row, const Complex &val) const
        {
            assert(row < m_numVS);
            m_J[row] = val;
        }

        /* Basic Elements */
        void setVS(int k, int nodeP, int nodeN, double volt);

    private:
        int m_numNodes;
        int m_numVS;
        Complex *m_Y, *m_B, *m_C, *m_D, *m_I, *m_E, *m_U, *m_J;
    };

}

#endif // CSIM_CIRCUIT_H_