/**
 * @file Property container
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

#include <cassert>
#include <cstring>
#include "csim/utils/string.h"
#include "csim/model/PropertyBag.h"

namespace csimModel
{

    void PropertyBag::addProperty(const char *entry, const Variant &def, const char *desc, uint32_t flags)
    {
        std::string normEntry = csim::toUpper(entry);
        assert(!hasProperty(normEntry.c_str()));
        m_props[normEntry].desc = desc ? desc : normEntry;
        m_props[normEntry].value = Variant(def.getType());
        m_props[normEntry].value = def;
        m_props[normEntry].flags = flags;
        m_props[normEntry].given = false;
    }

    void PropertyBag::setProperty(const char *entry, const Variant &val)
    {
        std::string normEntry = csim::toUpper(entry);
        assert(hasProperty(normEntry.c_str()));
        m_props[normEntry].value = val;
        m_props[normEntry].given = true;
    }

    Variant &PropertyBag::getProperty(const char *entry)
    {
        std::string normEntry = csim::toUpper(entry);
        assert(hasProperty(normEntry.c_str()));
        return m_props.at(normEntry).value;
    }

    bool PropertyBag::hasProperty(const char *entry) const
    {
        std::string normEntry = csim::toUpper(entry);
        return (m_props.find(normEntry) != m_props.end());
    }

    bool PropertyBag::propertyGiven(const char *entry) const
    {
        std::string normEntry = csim::toUpper(entry);
        assert(m_props.find(normEntry) != m_props.end());
        return m_props.at(normEntry).given;
    }

    bool PropertyBag::missingRequired() const
    {
        for (std::map<std::string, Entry>::const_iterator it = m_props.begin(); it != m_props.end(); it++)
        {
            if ((it->second.flags & Required) && !it->second.given)
                return true;
        }
        return false;
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
     * @param [out] buff Where to store the text
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
        return (m_metaIter->second.flags & PropFlags::Required);
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
