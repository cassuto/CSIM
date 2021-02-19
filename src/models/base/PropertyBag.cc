/**
 * @file Property container
 */

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

#include <cassert>
#include "csim/model/PropertyBag.h"

namespace csimModel
{

    void PropertyBag::addProperty(const char *entry, const Variant &def, bool required)
    {
        if (m_props.find(entry) == m_props.end())
        {
            m_props[entry].value = Variant(def.getType());
            m_props[entry].value = def;
            m_props[entry].required = required;
        }
    }

    void PropertyBag::setProperty(const char *entry, const Variant &val)
    {
        assert(m_props.find(entry) != m_props.end());
        m_props[entry].value = val;
    }

    const Variant &PropertyBag::getProperty(const char *entry) const
    {
        assert(m_props.find(entry) != m_props.end());
        return m_props.at(entry).value;
    }
}
