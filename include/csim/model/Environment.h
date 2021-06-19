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

#ifndef CSIM_MODELBASE_ENVIRONMENT_H_
#define CSIM_MODELBASE_ENVIRONMENT_H_

#define INITIAL_MSG_BUF_SIZE 2048

namespace csim
{
    class Circuit;
}

namespace csimModel
{
    class Environment
    {
    public:
        enum LogLevel
        {
            INFO = 0,
            WARN = 1,
            ERR,
            FATAL
        };
        Environment(csim::Circuit *circuit);
        ~Environment();

        double getTemp() const;
        double getNormTemp() const;
        bool getBypassEnabled() const;
        double getAbsTol() const;
        double getRelTol() const;
        double getVoltTol() const;
        double getChargeTol() const;
        double getTranTol() const;
        double getGmin() const;
        long getCKTmode() const;

        void setTemp(double temp);

        void logging(LogLevel level, const char *format, ...);

    private:
        const char *getLevelMsg(LogLevel level) const;

    private:
        csim::Circuit *m_circuit;
        char *m_msgBuf;
        size_t m_msgBufSize;

    protected:
        double m_VepsMax, m_VepsrMax;
        double m_IepsMax, m_IepsrMax;
        double m_ChargeEpsMax;
        double m_Gmin;
        double m_tTOEF;
        double m_temperature;
        double m_normTemperature;
        bool m_enableModelBypass;
        friend csim::Circuit;
    };
}

#endif // CSIM_MODELBASE_ENVIRONMENT_H_
