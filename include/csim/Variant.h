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

#ifndef MODELBASE_H_
#define MODELBASE_H_

#include <cstdint>
#include <string>

namespace csim
{

    class Variant
    {
    public:
        enum VariantType
        {
            VariantUint8 = 0,
            VariantUint16,
            VariantUint32,
            VariantUint64,
            VariantInt8,
            VariantInt16,
            VariantInt32,
            VariantInt64,
            VariantFloat,
            VariantDouble,
            VariantBoolean,
            VariantString,
        };

        explicit Variant(VariantType type);
        void setU8(uint8_t u8);
        void setU16(uint16_t u16);
        void setU32(uint32_t u32);
        void setU64(uint64_t u64);
        void setI8(int8_t i8);
        void setI16(int16_t i16);
        void setI32(int32_t i32);
        void setI64(int64_t i64);
        void setFloat(float f);
        void setDouble(double d);
        void setBoolean(bool b);
        void setString(const char *str);

        uint8_t getU8();
        uint16_t getU16();
        uint32_t getU32();
        uint64_t getU64();
        int8_t getI8();
        int16_t getI16();
        int32_t getI32();
        int64_t getI64();
        float getFloat();
        double getDouble();
        bool getBoolean();
        const char *getString();
        
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

#endif // MODELBASE_H_