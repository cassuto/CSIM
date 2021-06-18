/**
 * @file Variant Data type
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
#include "csim/model/Variant.h"

namespace csimModel
{

    Variant::Variant() : m_type(VariantUnknown) {}

    Variant::Variant(VariantType type)
    {
        m_type = type;
    }

    Variant::Variant(const Variant &src)
    {
        m_type = src.m_type;
        switch (src.m_type)
        {
        case VariantUnknown:
            break;
        case VariantUint8:
        case VariantUint16:
        case VariantUint32:
        case VariantUint64:
        case VariantInt8:
        case VariantInt16:
        case VariantInt32:
        case VariantInt64:
        case VariantDouble:
        case VariantBoolean:
        case VariantPropertyModel:
            m_dat = src.m_dat;
            break;
        case VariantString:
            m_str = src.m_str;
            break;
        default:
            assert(0);
        }
    }

    Variant::VariantType Variant::getType() const
    {
        return m_type;
    }

    /*
     * Setters
     * Return this object 
     */
    Variant &Variant::setUint8(uint8_t u8)
    {
        assert(m_type == VariantUint8);
        m_dat.u8 = u8;
        return (*this);
    }
    Variant &Variant::setUint16(uint16_t u16)
    {
        assert(m_type == VariantUint16);
        m_dat.u16 = u16;
        return (*this);
    }
    Variant &Variant::setUint32(uint32_t u32)
    {
        assert(m_type == VariantUint32);
        m_dat.u32 = u32;
        return (*this);
    }
    Variant &Variant::setUint64(uint64_t u64)
    {
        assert(m_type == VariantUint64);
        m_dat.u64 = u64;
        return (*this);
    }
    Variant &Variant::setInt8(int8_t i8)
    {
        assert(m_type == VariantInt8);
        m_dat.i8 = i8;
        return (*this);
    }
    Variant &Variant::setInt16(int16_t i16)
    {
        assert(m_type == VariantInt16);
        m_dat.i16 = i16;
        return (*this);
    }
    Variant &Variant::setInt32(int32_t i32)
    {
        assert(m_type == VariantInt32);
        m_dat.i32 = i32;
        return (*this);
    }
    Variant &Variant::setInt64(int64_t i64)
    {
        assert(m_type == VariantInt64);
        m_dat.i64 = i64;
        return (*this);
    }
    Variant &Variant::setDouble(double d)
    {
        assert(m_type == VariantDouble);
        m_dat.d = d;
        return (*this);
    }
    Variant &Variant::setBoolean(bool b)
    {
        assert(m_type == VariantBoolean);
        m_dat.b = b;
        return (*this);
    }
    Variant &Variant::setString(const char *str)
    {
        assert(m_type == VariantString);
        m_str = str;
        return (*this);
    }
    Variant &Variant::setPropertyModel(PropertyMdl *mdl)
    {
        assert(m_type == VariantPropertyModel);
        m_dat.mdl = mdl;
        return (*this);
    }

    /*
     * Getters
     */
    uint8_t Variant::getUint8() const
    {
        assert(m_type == VariantUint8);
        return m_dat.u8;
    }
    uint16_t Variant::getUint16() const
    {
        assert(m_type == VariantUint16);
        return m_dat.u16;
    }
    uint32_t Variant::getUint32() const
    {
        assert(m_type == VariantUint32);
        return m_dat.u32;
    }
    uint64_t Variant::getUint64() const
    {
        assert(m_type == VariantUint64);
        return m_dat.u64;
    }
    int8_t Variant::getInt8() const
    {
        assert(m_type == VariantInt8);
        return m_dat.i8;
    }
    int16_t Variant::getInt16() const
    {
        assert(m_type == VariantInt16);
        return m_dat.i16;
    }
    int32_t Variant::getInt32() const
    {
        assert(m_type == VariantInt32);
        return m_dat.i32;
    }
    int64_t Variant::getInt64() const
    {
        assert(m_type == VariantInt64);
        return m_dat.i64;
    }
    double Variant::getDouble() const
    {
        assert(m_type == VariantDouble);
        return m_dat.d;
    }
    bool Variant::getBoolean() const
    {
        assert(m_type == VariantBoolean);
        return m_dat.b;
    }
    const char *Variant::getString() const
    {
        assert(m_type == VariantString);
        return m_str.c_str();
    }
    PropertyMdl *Variant::getPropertyModel() const
    {
        assert(m_type == VariantPropertyModel);
        return m_dat.mdl;
    }

}
