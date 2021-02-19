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

#include <vector>
#include "csim/SubCircuit.h"
#include "csim/PropertyBag.h"

namespace csim
{

    class ModelBase : public SubCircuit
    {
    public:
        ModelBase(int terminals);
        virtual ~ModelBase();

        inline PropertyBag &property()
        {
            return m_props;
        }

    protected:
    public:
        virtual void prepareDC() = 0;
        virtual void prepareAC() = 0;
        virtual void prepareTR() = 0;

    private:
        PropertyBag m_props;
    };

}

#endif // CSIM_MODELBASE_H_
