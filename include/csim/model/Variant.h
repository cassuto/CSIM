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

#ifndef CSIM_VARIANT_H_
#define CSIM_VARIANT_H_

#include "csim/model/Types.h"
#include <string>

namespace csimModel
{

    class Variant
    {
    public:
        enum VariantType
        {
            VariantUnknown = 0,
            VariantUint8,
            VariantUint16,
            VariantUint32,
            VariantUint64,
            VariantInt8,
            VariantInt16,
            VariantInt32,
            VariantInt64,
            VariantDouble,
            VariantBoolean,
            VariantString,
        };

        Variant();
        explicit Variant(VariantType type);
        explicit Variant(const Variant &src);

        VariantType getType() const;

        Variant &setU8(uint8_t u8);
        Variant &setU16(uint16_t u16);
        Variant &setU32(uint32_t u32);
        Variant &setU64(uint64_t u64);
        Variant &setI8(int8_t i8);
        Variant &setI16(int16_t i16);
        Variant &setI32(int32_t i32);
        Variant &setI64(int64_t i64);
        Variant &setDouble(double d);
        Variant &setBoolean(bool b);
        Variant &setString(const char *str);

        uint8_t getU8() const;
        uint16_t getU16() const;
        uint32_t getU32() const;
        uint64_t getU64() const;
        int8_t getI8() const;
        int16_t getI16() const;
        int32_t getI32() const;
        int64_t getI64() const;
        double getDouble() const;
        bool getBoolean() const;
        const char *getString() const;

    private:
        VariantType m_type;
        union
        {
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;
            int8_t i8;
            int16_t i16;
            int32_t i32;
            int64_t i64;
            float f;
            double d;
            bool b;
        } m_dat;
        std::string m_str;
    };

}

#endif // CSIM_VARIANT_H_