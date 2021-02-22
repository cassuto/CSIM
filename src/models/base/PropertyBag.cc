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
#include <cstring>
#include "csim/model/PropertyBag.h"

namespace csimModel
{

    void PropertyBag::addProperty(const char *entry, const Variant &def, bool required)
    {
        assert(!hasProperty(entry));
        m_props[entry].value = Variant(def.getType());
        m_props[entry].value = def;
        m_props[entry].required = required;
    }

    void PropertyBag::setProperty(const char *entry, const Variant &val)
    {
        assert(hasProperty(entry));
        m_props[entry].value = val;
    }

    const Variant &PropertyBag::getProperty(const char *entry) const
    {
        assert(hasProperty(entry));
        return m_props.at(entry).value;
    }

    bool PropertyBag::hasProperty(const char *entry) const
    {
        return (m_props.find(entry) != m_props.end());
    }

    /**
     * @brief Begin the meta data iterator
     */
    void PropertyBag::metaBegin()
    {
        m_metaIter = m_props.begin();
    }

    /**
     * @brief Get the name of property at the current cursor.
     * @param buff Where to store the text
     * @param maxsize Maximum length that can be written to the buffer
     * @return Size of the actual text.
     */
    size_t PropertyBag::metaGetPropertyName(char *buff, size_t maxsize) const
    {
        strncpy(buff, m_metaIter->first.c_str(), maxsize);
        return strlen(m_metaIter->first.c_str());
    }

    /**
     * @brief Get whether the current property is required.
     * @return Boolean
     */
    bool PropertyBag::metaGetPropertyRequired() const
    {
        return m_metaIter->second.required;
    }

    /**
     * @brief Move the meta data cursor to the next position.
     * @retval true If there are successors
     * @retval false If there are no more elements next
     */
    bool PropertyBag::metaNext()
    {
        ++m_metaIter;
        return (m_metaIter == m_props.end());
    }
}
