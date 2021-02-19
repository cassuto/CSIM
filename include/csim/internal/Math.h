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

#ifndef CSIM_MATH_H_
#define CSIM_MATH_H_

#include <complex>
#include "csim/model/Types.h"

namespace csim
{

    /*
     * Fast Complex restricted to the core
     */
    class Complex : public std::complex<double>
    {
    public:
        Complex() : std::complex<double>(0, 0) {}
        Complex(const csimModel::MComplex &src) : std::complex<double>(src.real(), src.imag()) {}
        inline double real() const
        {
            return std::real(*this);
        }
        inline double imag() const
        {
            return std::imag(*this);
        }
        inline csimModel::MComplex toMComplex() const
        {
            return csimModel::MComplex(real(), imag());
        }
    };
}

#endif // CSIM_MATH_H_
