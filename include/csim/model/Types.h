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

#ifndef CSIM_TYPES_H_
#define CSIM_TYPES_H_

#include <cstdint>
#include <complex>
#include <iostream>

namespace csimModel
{

    /*
     * Complex class restricted to model SDK
     * Note that this class is assumed to be `trivial typed`, i.e., no any virtual
     * function or non-trival type membership, which ensures `memset-safe`.
     */
    struct MComplex
    {
    public:
        MComplex();
        MComplex(double real);
        MComplex(double real, double imag);
        MComplex(const MComplex &src);

        double real() const;
        double imag() const;
        double *ptr();

        MComplex operator+(const MComplex &rval) const;
        MComplex operator+=(const MComplex &rval);
        MComplex operator-(const MComplex &rval) const;
        MComplex operator-=(const MComplex &rval);

        MComplex operator*(const MComplex &rval) const;
        MComplex operator*(double rval) const;
        MComplex operator*=(double rval);
        MComplex operator/(const MComplex &rval) const;
        MComplex operator/(double rval) const;
        MComplex operator/=(double rval);

        MComplex operator-() const;

        friend std::ostream& operator<<(std::ostream &out, const MComplex &src);

    public:
        double m_complex[2];
    };

    std::ostream& operator<<(std::ostream &out, const MComplex &src);

    class ModelBase;
    class PropertyMdl;
}

namespace csim
{
    class Circuit;
    class Algebraic;
}

namespace std
{
    double abs(const csimModel::MComplex &cplx);
    double norm(const csimModel::MComplex &cplx);
    double real(const csimModel::MComplex &cplx);
    double imag(const csimModel::MComplex &cplx);
    double arg(const csimModel::MComplex &cplx);
}

#endif // CSIM_TYPES_H_
