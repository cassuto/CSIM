/**
 * @file Complex type restricted to model SDK
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

#include "csim/model/Types.h"

namespace csimModel
{
    MComplex::MComplex() : m_real(0.0), m_imag(0.0)
    {
    }
    MComplex::MComplex(double real) : m_real(real), m_imag(0.0)
    {
    }
    MComplex::MComplex(double real, double imag) : m_real(real), m_imag(imag)
    {
    }

    double MComplex::real() const
    {
        return m_real;
    }
    double MComplex::imag() const
    {
        return m_imag;
    }

    MComplex MComplex::operator+(const MComplex &rval)
    {
        return MComplex(m_real + rval.m_real, m_imag + rval.m_imag);
    }
    MComplex MComplex::operator+=(const MComplex &rval)
    {
        m_real += rval.m_real;
        m_imag += rval.m_imag;
        return (*this);
    }
    MComplex MComplex::operator-(const MComplex &rval)
    {
        return MComplex(m_real - rval.m_real, m_imag - rval.m_imag);
    }
    MComplex MComplex::operator-=(const MComplex &rval)
    {
        m_real -= rval.m_real;
        m_imag -= rval.m_imag;
        return (*this);
    }

    MComplex MComplex::operator*(double rval)
    {
        return MComplex(m_real * rval, m_imag * rval);
    }
    MComplex MComplex::operator*=(double rval)
    {
        m_real *= rval;
        m_imag *= rval;
        return (*this);
    }
    MComplex MComplex::operator/(double rval)
    {
        return MComplex(m_real / rval, m_imag / rval);
    }
    MComplex MComplex::operator/=(double rval)
    {
        m_real /= rval;
        m_imag /= rval;
        return (*this);
    }
}
