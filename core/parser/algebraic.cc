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
#include <cmath>
#include <cassert>
#include <algorithm>
#include <cctype>
#include "csim/utils/string.h"
#include "csim/utils/errors.h"
#include "csim/internal/parser/algebraic-defs.h"

namespace csim
{
    /* Globals */
    AlgebraicScope *algebraic_currentScope;
    double algebraic_result;
    bool algebraic_err;
    const char *algebraic_input;
    int algebraic_input_size;

    double algebraic_parseReal(const char *str)
    {
        size_t idx;
        double dbl = std::stod(str, &idx);
        /* Parse SACLE defined in `lex-hspice.lpp` */
        switch (std::tolower(str[idx]))
        {
        case '\0':
            break;
        case 'f':
            dbl *= 1e-15;
            break;
        case 'p':
            dbl *= 1e-12;
            break;
        case 'n':
            dbl *= 1e-9;
            break;
        case 'u':
            dbl *= 1e-6;
            break;
        case 'm':
            switch (std::tolower(str[idx + 1]))
            {
            case 'i': /* mil */
                assert(std::tolower(str[idx + 2]) == 'l');
                dbl *= 25.4e-6;
                break;

            case 'e': /* meg */
                assert(std::tolower(str[idx + 2]) == 'g');
                dbl *= 1e6;
                break;

            default: /* m */
                dbl *= 1e-3;
                break;
            }
            break;
        case 'k':
            dbl *= 1e3;
            break;
        case 'g':
            dbl *= 1e9;
            break;
        case 't':
            dbl *= 1e12;
            break;
        default:
            assert(0);
        }
        return dbl;
    }

    AlgebraicScope::AlgebraicScope() : m_parent(nullptr)
    {
    }
    AlgebraicScope::AlgebraicScope(AlgebraicScope *parent)
        : m_parent(parent)
    {
    }

    AlgebraicScope::~AlgebraicScope()
    {
        for (auto &kv : m_params)
        {
            ParamEntry &entry = kv.second;
            if (entry.unsolved)
                delete entry.u.alg;
        }
    }

    /**
     * @brief Add a parameter to the scope
     * If the specified identifier is existing, then take no actions,
     * because the reduction of syntax tree is bottom-top, the parameter definitions
     * in later lines will be handled early.
     */
    void AlgebraicScope::addParam(const char *identifier, double value)
    {
        std::string id = toUpper(identifier);
        if (m_params.find(id) != m_params.end())
            return;
        m_params[id].u.real = value;
        m_params[id].unsolved = false;
    }
    void AlgebraicScope::addParam(const char *identifier, Algebraic *alg)
    {
        std::string id = toUpper(identifier);
        if (m_params.find(id) != m_params.end())
            return;
        m_params[id].u.alg = alg; /* for lazzy evaluation later */
        m_params[id].unsolved = true;
    }

    /**
     * @brief Get the value of a parameter by its identifier
     * If the specified identifier is not existing in this scope,
     * then it will look at its parent scope recursively (if any).
     */
    int AlgebraicScope::getValue(const char *identifier, double *out)
    {
        std::string id = toUpper(identifier);
        AlgebraicScope *cur = this;
        int rc;
        while (cur)
        {
            if (cur->m_params.find(id) != cur->m_params.end())
            {
                ParamEntry &entry = cur->m_params.at(id);
                if (entry.unsolved)
                {
                    /* Lazzy evaluation */
                    rc = entry.u.alg->evaluate(cur, out);
                    if (CSIM_OK(rc))
                    {
                        entry.unsolved = false;
                        delete entry.u.alg;
                        entry.u.real = *out;
                    }
                    return rc;
                }
                else
                {
                    *out = entry.u.real;
                    return 0;
                }
                return 0;
            }
            cur = cur->m_parent;
        }
        return CERR_IDENTIFIER_UNDEFINED;
    }

    double algebraic_trunc(double x)
    {
        return (x < 0) ? std::ceil(x) : std::floor(x);
    }

    int algebraic_yyinput(char *buf, int max_size)
    {
        int toread = std::min(max_size, algebraic_input_size);
        if (toread > 0)
        {
            memcpy(buf, algebraic_input, toread);
            algebraic_input += toread;
            algebraic_input_size -= toread;
        }
        return toread;
    }

    Algebraic::Algebraic(const char *statement)
        : m_statement(statement)
    {
    }

    int Algebraic::evaluate(AlgebraicScope *scope, double *out)
    {
        int rc = 0;
        /*TODO: global lock */
        algebraic_input = m_statement.c_str();
        algebraic_input_size = (int)m_statement.size();
        algebraic_currentScope = scope;
        algebraic_restart(nullptr);
        algebraic_wrap();
        algebraic_lineno = 1;
        algebraic_err = false;
        if (algebraic_parse())
            rc = CERR_PARSE_ALGEBRAIC_EXPRESSION;
        if (algebraic_err)
            rc = CERR_PARSE_ALGEBRAIC_EXPRESSION;
        algebraic_input = nullptr;
        *out = algebraic_result;
        /*TODO: global unlock */
        return rc;
    }

}
