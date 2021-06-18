/**
 * @file Dataset
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

#include <cassert>
#include "csim/utils/errors.h"
#include "csim/internal/Dataset.h"

namespace csim
{

    Variable::Variable(const std::string &name)
        : m_name(name)
    {
    }
    void Variable::addValue(const csimModel::MComplex &value)
    {
        m_values.push_back(value);
    }
    void Variable::addIndependVar(const Variable &var)
    {
        m_independentVars.push_back(var.getName());
    }

    void Variable::clearValues()
    {
        m_values.clear();
    }
    void Variable::clearIndependVars()
    {
        m_independentVars.clear();
    }

    Variable &Dataset::addIndependentVar(const std::string &name)
    {
        assert(m_independentVars.find(name) == m_independentVars.end());
        return (m_independentVars[name] = Variable(name));
    }

    void Dataset::addGroup(const std::string &group)
    {
        assert(m_groups.find(group) == m_groups.end());
        m_groups[group] = DataGroup();
    }
    Variable &Dataset::addDependentVar(const std::string &group, const std::string &name)
    {
        assert(m_groups[group].vars.find(name) == m_groups[group].vars.end());
        return (m_groups[group].vars[name] = Variable(name));
    }

    Variable &Dataset::getIndependentVar(const std::string &name)
    {
        assert(m_independentVars.find(name) != m_independentVars.end());
        return m_independentVars.at(name);
    }
    Variable &Dataset::getDependentVar(const std::string &group, const std::string &name)
    {
        assert(m_groups.find(group) != m_groups.end());
        assert(m_groups[group].vars.find(name) != m_groups[group].vars.end());
        return m_groups.at(group).vars.at(name);
    }

    void Dataset::clear()
    {
        m_independentVars.clear();
        m_groups.clear();
    }

}
