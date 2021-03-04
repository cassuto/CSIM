/**
 * @file PN-junction model
 */

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

#include <algorithm>
#include "Constants.h"
#include "PN.h"

namespace csimModel
{

    PN::PN(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          m_Is(0.0), m_N(0.0), m_Isr(0.0), m_Nr(0.0), m_Temp(0.0), m_Ibv(0.0),
          m_Bv(0.0), m_Bv_set(false),
          m_Ut(0.0),
          m_Uth(0.0),
          m_Ud_1(0.0)
    {
        property().addProperty("Is", Variant(Variant::VariantDouble).setDouble(1e-14), false);
        property().addProperty("N", Variant(Variant::VariantDouble).setDouble(1.0), false);
        property().addProperty("Isr", Variant(Variant::VariantDouble).setDouble(0.0), false);
        property().addProperty("Nr", Variant(Variant::VariantDouble).setDouble(2.0), false);
        property().addProperty("Temp", Variant(Variant::VariantDouble).setDouble(27.0), false);
        property().addProperty("Ibv", Variant(Variant::VariantDouble).setDouble(1e-3), false);
        property().addProperty("Bv", Variant(Variant::VariantDouble).setDouble(40.0), false);
        property().addProperty("Bv_set", Variant(Variant::VariantBoolean).setBoolean(true), false);
        property().addProperty("Area", Variant(Variant::VariantDouble).setDouble(1.0), false);
    }

    PN::~PN()
    {
    }

    int PN::configure()
    {
        m_Is = property().getProperty("Is").getDouble();
        m_N = property().getProperty("N").getDouble();
        m_Isr = property().getProperty("Isr").getDouble();
        m_Nr = property().getProperty("Nr").getDouble();
        m_Temp = property().getProperty("Temp").getDouble();
        m_Ibv = property().getProperty("Ibv").getDouble();
        m_Bv = property().getProperty("Bv").getDouble();
        m_Bv_set = property().getProperty("Bv_set").getBoolean();
        double area = property().getProperty("Area").getDouble();

        /* Apply the ratio of junction area to the reference junction area */
        m_Is *= area;
        m_Isr *= area;

        /* Temperature voltage equivalent */
        m_Ut = kBoltzmann * (m_Temp - kKelvin) / qElement;

        if (m_Bv_set)
        {
            /*
             * Fit the breakdown voltage from Isr to Is
             * 
             * The principle is shown as follows:
             * Let the equations of breakdown current about Is and Isr be equal, i.e.
             * \f[ -Ibv \cdot e^{\frac{-(Bv+Ud)}{N \cdot Ut}} = -Is \cdot e^{\frac{-(Bv'+Ud)}{N \cdot Ut}} \f]
             * 
             * Take logarithms on both sides of the equation to get:
             * \f[ ln(\frac{Ibv}{Is})+\frac{-(Bv-Ud)}{N \cdot Ut} = \frac{-(Bv'-Ud)}{N \cdot Ut} $$ \f]
             * 
             * Finally, Rewrite the above equation and get the following results:
             * \f[ Bv' = Bv - N \cdot Ut \cdot ln(\frac{Ibv}{Is}) $$ \f]
             */
            m_Bv = m_Bv - m_N * m_Ut * std::log(m_Ibv / m_Is);
        }

        /*
         * Compute critical voltage
         * 
         * The principle is shown as follows:
         * 
         * Critical voltage Uth is defined as the point in V/I curve which makes the curve radius minimum.
         * The curve radius of V/I curve can be written as:
         * \f[ R=\left|\frac{{\left(1+{(\frac{\partial I}{\partial U})}^2\right)}^{\frac{3}{2}}}{\frac{\partial^2 I}{\partial U^2}}\right| \f]
         * 
         * Finding the minimum by making the derivative zero:
         * \f[ \frac{\partial R}{\partial U} = 0 \f]
         * 
         * Finally, we can find out the Uth which meets the above condition:
         * \f[ Uth = N \cdot Ut \cdot ln(\frac{N \cdot Ut}{\sqrt{2} \cdot Is}) \f]
         */
        m_Uth = m_N * m_Ut * std::log(m_N * m_Ut / (sqrt_2 * m_Is));

        resizeModel(2, 0, 0);

        return 0;
    }

    int PN::prepareDC()
    {
        m_Ud_1 = (getU(getNode(0)) - getU(getNode(1))).real();
        return 0;
    }
    int PN::prepareAC()
    {
        return prepareDC();
    }
    int PN::prepareTR()
    {
        return prepareDC();
    }

    int PN::iterateDC()
    {
        double Ud = (getU(getNode(0)) - getU(getNode(1))).real();
        Ud = vlimit(Ud);
        m_Ud_1 = Ud;

        double geq, Id;
        const double Ute = m_N * m_Ut;
        const double Uter = m_Nr * m_Ut;

        if (m_Bv_set && Ud < -m_Bv) /* breakdown */
        {
            double e = std::exp(-(m_Bv + Ud) / Ute);
            Id = -m_Is * e;
            geq = m_Is * e / Ute;
        }
        else
        {
            double e = std::exp(Ud / Ute);
            geq = m_Is * e / Ute;
            Id = m_Is * (e - 1.0);

            /* recombination current */
            e = std::exp(Ud / Uter);
            geq += m_Isr * e / Uter;
            Id += m_Isr * (e - 1.0);
        }

        double Ieq = Id - Ud * geq;

        addY(getNode(0), getNode(0), +geq), addY(getNode(0), getNode(1), -geq);
        addY(getNode(1), getNode(0), -geq), addY(getNode(1), getNode(1), +geq);
        addI(getNode(0), -Ieq);
        addI(getNode(1), +Ieq);

        return 0;
    }
    int PN::iterateAC(double omega)
    {
        return 0;
    }
    int PN::iterateTR(double tTime)
    {
        return iterateDC();
    }

    /**
     * @brief Limit the new junction voltage to avoid numerical overflow
     * @param Ud Junction voltage in (k+1)-th iteration
     * @param Ud_1 Junction voltage in k-th iteration
     * @return Adjusted junction voltage
     */
    double PN::vlimit(double Ud)
    {
        bool flag = false;
        double Ud_0, Ud_1, Ud_f;
        const double Ute = m_N * m_Ut;

        /* Fix the very small voltage */
        if (m_Bv_set && Ud < std::min(0.0, -m_Bv + 10 * Ute))
        {
            Ud_0 = -(Ud + m_Bv);
            Ud_1 = -(m_Ud_1 + m_Bv);
            flag = true;
        }
        else
        {
            Ud_0 = Ud;
            Ud_1 = m_Ud_1;
        }

        /* This code comes from SPICE3F5. Authors: 1985 Thomas L. Quarles, 2000 Alan Gillespie. */
        if (Ud_0 > m_Uth && std::abs(Ud_0 - Ud_1) > 2 * Ute)
        {
            if (Ud_1 > 0)
            {
                double arg = (Ud_0 - Ud_1) / Ute;
                if (arg > 0)
                    Ud_f = Ud_1 + Ute * (2 + std::log(arg - 2));
                else
                    Ud_f = Ud_1 - Ute * (2 + std::log(2 - arg));
            }
            else
                Ud_f = Ute * std::log(Ud_0 / Ute);
        }
        else
        {
            Ud_f = Ud_0; /* Near zero */
            if (Ud_0 < 0)
            {
                double arg = Ud_1 > 0 ? -1 - Ud_1 : 2 * Ud_1 - 1;
                if (Ud_0 < arg)
                    Ud_f = arg;
            }
        }

        if (flag)
        {
            return -(Ud_f + m_Bv);
        }
        else
        {
            return Ud_f;
        }
    }

    void PN::stepChangedTR(double tTime, double nstep)
    {
        m_Ud_1 = (getU(getNode(0)) - getU(getNode(1))).real();
    }
}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::PN(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

const ModelDescriptor descriptor = {
    /* id */
    "PN",
    /* description */
    "Generic PN-junction"};
