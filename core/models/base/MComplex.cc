/**
 * @file Complex type restricted to model SDK
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

#include "csim/model/Types.h"

namespace csimModel
{
    MComplex::MComplex()
    {
        m_complex[0] = m_complex[1] = 0.0;
    }
    MComplex::MComplex(double real)
    {
        m_complex[0] = real;
        m_complex[1] = 0.0;
    }
    MComplex::MComplex(double real, double imag)
    {
        m_complex[0] = real;
        m_complex[1] = imag;
    }
    MComplex::MComplex(const MComplex &src)
    {
        m_complex[0] = src.m_complex[0];
        m_complex[1] = src.m_complex[1];
    }

    double MComplex::real() const
    {
        return m_complex[0];
    }
    double MComplex::imag() const
    {
        return m_complex[1];
    }

    /**
     * @brief
     * *(ptr() + 0) is real
     * *(ptr() + 1) is imag
     */
    double *MComplex::ptr()
    {
        return m_complex;
    }

    MComplex MComplex::operator+(const MComplex &rval) const
    {
        return MComplex(m_complex[0] + rval.m_complex[0], m_complex[1] + rval.m_complex[1]);
    }
    MComplex MComplex::operator+=(const MComplex &rval)
    {
        m_complex[0] += rval.m_complex[0];
        m_complex[1] += rval.m_complex[1];
        return (*this);
    }
    MComplex MComplex::operator-(const MComplex &rval) const
    {
        return MComplex(m_complex[0] - rval.m_complex[0], m_complex[1] - rval.m_complex[1]);
    }
    MComplex MComplex::operator-=(const MComplex &rval)
    {
        m_complex[0] -= rval.m_complex[0];
        m_complex[1] -= rval.m_complex[1];
        return (*this);
    }

    MComplex MComplex::operator*(double rval) const
    {
        return MComplex(m_complex[0] * rval, m_complex[1] * rval);
    }
    MComplex MComplex::operator*=(double rval)
    {
        m_complex[0] *= rval;
        m_complex[1] *= rval;
        return (*this);
    }
    MComplex MComplex::operator/(double rval) const
    {
        return MComplex(m_complex[0] / rval, m_complex[1] / rval);
    }
    MComplex MComplex::operator/=(double rval)
    {
        m_complex[0] /= rval;
        m_complex[1] /= rval;
        return (*this);
    }

    MComplex MComplex::operator*(const MComplex &rval) const
    {
        return MComplex(m_complex[0] * rval.m_complex[0] - m_complex[1] * rval.m_complex[1], m_complex[0] * rval.m_complex[1] + m_complex[1] * rval.m_complex[0]);
    }
    MComplex MComplex::operator/(const MComplex &rval) const
    {
        double k = rval.m_complex[0] * rval.m_complex[0] + rval.m_complex[1] * rval.m_complex[1];
        return MComplex((m_complex[0] * rval.m_complex[0] + m_complex[1] * rval.m_complex[1]) / k, (m_complex[1] * rval.m_complex[0] - m_complex[0] * rval.m_complex[1]) / k);
    }

    MComplex MComplex::operator-() const
    {
        return MComplex(-m_complex[0], -m_complex[1]);
    }

    std::ostream& operator<<(std::ostream &out, const MComplex &src)
    {
        out<<src.real()<<"+"<<src.imag()<<"*j";
        return out;
    }
}

namespace std
{
    double abs(const csimModel::MComplex &cplx)
    {
        return sqrt(cplx.m_complex[0] * cplx.m_complex[0] + cplx.m_complex[1] * cplx.m_complex[1]);
    }
    double norm(const csimModel::MComplex &cplx)
    {
        return cplx.m_complex[0] * cplx.m_complex[0] + cplx.m_complex[1] * cplx.m_complex[1];
    }
    double real(const csimModel::MComplex &cplx)
    {
        return cplx.m_complex[0];
    }
    double imag(const csimModel::MComplex &cplx)
    {
        return cplx.m_complex[1];
    }
    double arg(const csimModel::MComplex &cplx)
    {
        return std::atan2(cplx.m_complex[1], cplx.m_complex[0]);
    }
}
