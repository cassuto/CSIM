/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#ifndef CSIM_SWEEP_H_
#define CSIM_SWEEP_H_

namespace csim
{

    class Sweep
    {
    public:
        virtual int init(double start, double stop, int points) = 0;
        virtual double next() = 0;
        virtual bool hasNext() = 0;

        static Sweep *createInstance(const char *name);
    };

}

#endif // CSIM_SWEEP_H_
