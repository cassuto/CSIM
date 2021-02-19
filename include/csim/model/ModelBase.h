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

#ifndef CSIM_MODELBASE_H_
#define CSIM_MODELBASE_H_

#include "csim/model/Types.h"
#include "csim/model/PropertyBag.h"

namespace csimModel
{

    class ModelBase
    {
    public:
        ModelBase();
        virtual ~ModelBase();

    public:
        virtual void configure() = 0;
        virtual void prepareDC() = 0;
        virtual void prepareAC() = 0;
        virtual void prepareTR() = 0;

    public:
        PropertyBag &property();

        /* MNA matrices */
        const MComplex &getY(int row, int col) const;
        const MComplex &getB(int row, int col) const;
        const MComplex &getC(int row, int col) const;
        const MComplex &getD(int row, int col) const;
        const MComplex &getI(int row) const;
        const MComplex &getE(int row) const;
        const MComplex &getU(int row) const;
        const MComplex &getJ(int row) const;

    protected:
        void createMatrix(int numNodes, int numVS);

        /* MNA matrices */
        void setY(int row, int col, const MComplex &val);
        void setB(int row, int col, const MComplex &val);
        void setC(int row, int col, const MComplex &val);
        void setD(int row, int col, const MComplex &val);
        void setI(int row, const MComplex &val);
        void setE(int row, const MComplex &val);
        void setU(int row, const MComplex &val);
        void setJ(int row, const MComplex &val);

        /* Basic Elements */
        void setVS(int k, int nodeP, int nodeN, double volt);

    private:
        PropertyBag m_props;
        int m_numNodes;
        int m_numVS;
        MComplex *m_Y, *m_B, *m_C, *m_D, *m_I, *m_E, *m_U, *m_J;
    };

}

#endif // CSIM_MODELBASE_H_
