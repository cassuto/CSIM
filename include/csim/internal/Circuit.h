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

#include <vector>
#include "csim/internal/Complex.h"

namespace csimModel
{
    class ModelBase;
}

namespace csim
{

    class LinearSolver;

    class Circuit
    {
    public:
        Circuit();
        ~Circuit();

        /* MNA matrices */
        const Complex &getA(int row, int col) const;
        const Complex &getX(int row) const;
        const Complex &getZ(int row) const;

        void setA(int row, int col, const Complex &val);
        void setX(int row, const Complex &val);
        void setZ(int row, const Complex &val);

        inline int getNumNodes() const
        {
            return m_numNodes;
        }
        inline int getNumVS() const
        {
            return m_numVS;
        }

        void addComponent(csimModel::ModelBase *model);
        int solveMNA();

    private:
        void createMatrix(int numNodes, int numVS);

    private:
        int m_numNodes;
        int m_numVS;
        int m_matrixRows;
        Complex *m_A, *m_x, *m_z;
        LinearSolver *m_linearSolver;
        std::vector<csimModel::ModelBase *> m_models;
    };

}

#endif // CSIM_CIRCUIT_H_
