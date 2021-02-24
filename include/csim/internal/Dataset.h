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

#ifndef CSIM_DATASET_H_
#define CSIM_DATASET_H_

#include <vector>
#include <map>
#include <string>
#include "csim/internal/Complex.h"

namespace csim
{
    class Variable
    {
    public:
        Variable() {}
        Variable(const std::string &name);
        void addValue(const Complex &value);
        void addIndependVar(const Variable &var);
        void clearValues();
        void clearIndependVars();
        inline unsigned int getNumValues() const
        {
            return m_values.size();
        }
        inline unsigned int getNumIndependVars() const
        {
            return m_independentVars.size();
        }
        inline const Complex &at(unsigned int index) const
        {
            return m_values.at(index);
        }
        inline const std::string getName() const
        {
            return m_name;
        }

    private:
        std::string m_name;
        std::vector<std::string> m_independentVars;
        std::vector<Complex> m_values;
    };

    class Dataset
    {
    public:
        Variable &addIndependentVar(const std::string &name);
        void addGroup(const std::string &group);
        Variable &addDependentVar(const std::string &group, const std::string &name);
        Variable &getIndependentVar(const std::string &name);
        Variable &getDependentVar(const std::string &group, const std::string &name);
        void clear();

        inline void setName(const std::string &name)
        {
            m_name = name;
        }
        inline const std::string &getName() const
        {
            return m_name;
        }

    private:
        class DataGroup
        {
        public:
            std::map<std::string, Variable> vars;
        };
        std::string m_name;
        std::map<std::string, Variable> m_independentVars;
        std::map<std::string, DataGroup> m_groups;
    };
}

#endif // CSIM_DATASET_H_
