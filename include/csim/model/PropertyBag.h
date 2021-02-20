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

#ifndef CSIM_PROPERTYBAG_H_
#define CSIM_PROPERTYBAG_H_

#include <map>
#include <string>
#include "csim/model/Variant.h"

namespace csimModel
{

    class PropertyBag
    {
    public:
        void addProperty(const char *entry, const Variant &def, bool required);
        void setProperty(const char *entry, const Variant &val);
        const Variant &getProperty(const char *entry) const;
        bool hasProperty(const char *entry) const;

        /* Meta data iterator */
        void metaBegin();
        size_t metaGetPropertyName(char *buff, size_t maxsize) const;
        bool metaGetPropertyRequired() const;
        bool metaNext();

    private:
        struct Entry
        {
            Variant value;
            bool required;
        };
        std::map<std::string, Entry> m_props;
        std::map<std::string, Entry>::iterator m_metaIter;
    };

}

#endif // CSIM_PROPERTYBAG_H_