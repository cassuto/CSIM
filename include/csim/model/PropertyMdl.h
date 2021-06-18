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

#ifndef CSIM_PROPERTYMDL_H_
#define CSIM_PROPERTYMDL_H_

#include <vector>
#include <string>
#include "csim/model/Types.h"
#include "csim/model/Variant.h"
#include "csim/model/Environment.h"

namespace csimModel
{

    class PropertyMdl
    {
    public:
        PropertyMdl() {}
        virtual ~PropertyMdl() {}

        /* Meta operations */
        virtual int getPropertyNum() const=0;
        virtual const char *getPropertyName(int id) const=0;
        virtual const char *getPropertyDesc(int id) const=0;
        virtual Variant::VariantType getPropertyType(int id) const=0;
        virtual uint32_t getPropertyFlags(int id) const=0;

        /* Setter */
        virtual const char *name() const=0;
        virtual void setProperty(int id, const Variant &value)=0;
        virtual int setup(Environment *env)=0;
        virtual int setTemp(Environment *env)=0;
        virtual int apply(ModelBase *devModel, Environment *env)=0;
    };

}

#endif // CSIM_PROPERTYMDL_H_
