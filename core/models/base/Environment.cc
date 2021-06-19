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
#include <cstdarg>
#include <iostream>
#include <algorithm>
#include "csim/internal/Circuit.h"
#include "csim/model/Environment.h"

namespace csimModel
{
    Environment::Environment(csim::Circuit *circuit)
        : m_circuit(circuit),
          m_msgBuf(new char[INITIAL_MSG_BUF_SIZE]),
          m_msgBufSize(INITIAL_MSG_BUF_SIZE),
          m_VepsMax(1e-6), m_VepsrMax(0.001),
          m_IepsMax(1e-12), m_IepsrMax(0.001),
          m_ChargeEpsMax(1e-14),
          m_Gmin(1e-12),
          m_tTOEF(7),
          m_temperature(300.15),
          m_normTemperature(300.15),
          m_enableModelBypass(true)
    {
    }

    Environment::~Environment()
    {
        delete m_msgBuf;
    }

    /**
     * @brief Parameter `TEMP` in SPICE (Full compatible)
     */
    double Environment::getTemp() const
    {
        return m_temperature;
    }

    /**
     * @brief Parameter `TNOM` in SPICE (Full compatible)
     */
    double Environment::getNormTemp() const
    {
        return m_normTemperature;
    }

    /**
     * @brief Parameter `ABSTOL` in SPICE (Full compatible)
     */
    double Environment::getAbsTol() const
    {
        return m_IepsMax;
    }

    /**
     * @brief Parameter `RELTOL` in SPICE (Full compatible)
     */
    double Environment::getRelTol() const
    {
        return std::min(m_VepsrMax, m_IepsrMax);
    }

    /**
     * @brief Parameter `VNTOL` in SPICE (Full compatible)
     */
    double Environment::getVoltTol() const
    {
        return m_VepsMax;
    }

    /**
     * @brief Parameter `CHGTOL` in SPICE (Full compatible)
     */
    double Environment::getChargeTol() const
    {
        return m_ChargeEpsMax;
    }

    /**
     * @brief Parameter `TRTOL` in SPICE (Full compatible)
     */
    double Environment::getTranTol() const
    {
        return m_tTOEF;
    }

    /**
     * @brief Parameter `GMIN` in SPICE (Full compatible)
     */
    double Environment::getGmin() const
    {
        return m_Gmin;
    }

    /**
     * @brief `CKTmode` in SPICE
     */
    long Environment::getCKTmode() const
    {
        return m_circuit->spiceCompatible()->getMode();
    }

    bool Environment::getBypassEnabled() const
    {
        return m_enableModelBypass;
    }

    void Environment::setTemp(double temp)
    {
        m_temperature = temp;
    }

    const char *Environment::getLevelMsg(LogLevel level) const
    {
        switch (level)
        {
        case INFO:
            return "[INFO]";
        case ERR:
            return "[ERR]";
        case FATAL:
            return "[FATAL]";
        default:
            return "";
        }
    }

    void Environment::logging(LogLevel level, const char *format, ...)
    {
        va_list va;
        va_start(va, format);
        for (;;)
        {
            int ret = vsnprintf(m_msgBuf, m_msgBufSize, format, va);
            if (ret < 0)
                break;
            else if (ret >= (int)m_msgBufSize)
            {
                delete m_msgBuf;
                m_msgBufSize = (size_t)ret;
                m_msgBuf = new char[m_msgBufSize];
            }
            else
                break;
        }
        va_end(va);
        std::cout << getLevelMsg(level) << " " << m_msgBuf;
    }
}
