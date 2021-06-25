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

#ifndef CSIM_PARSER_ALGEBRAIC_DEFS_H_
#define CSIM_PARSER_ALGEBRAIC_DEFS_H_

#include <map>

namespace csim
{
    class Algebraic;
    class AlgebraicScope
    {
    public:
        AlgebraicScope();
        ~AlgebraicScope();
        AlgebraicScope(AlgebraicScope *parent);

        int getValue(const char *identifier, double *out);
        void addParam(const char *identifier, double value);
        void addParam(const char *identifier, Algebraic *alg);

        inline void setParent(AlgebraicScope *parent)
        {
            m_parent = parent;
        }

    private:
        class ParamEntry
        {
        public:
            bool unsolved;
            union
            {
                double real;
                Algebraic *alg;
            } u;
        };
        AlgebraicScope *m_parent;
        std::map<std::string, ParamEntry> m_params;
    };

    class Algebraic
    {
    public:
        Algebraic(const char *statement);

        int evaluate(AlgebraicScope *scope, double *out);

    private:
        std::string m_statement;
    };

    double algebraic_parseReal(const char *str);
    double algebraic_trunc(double x);

    int algebraic_yyinput(char *buf, int max_size);

    extern AlgebraicScope *algebraic_currentScope;
    extern double algebraic_result;
    extern bool algebraic_err;
    extern const char *algebraic_input;
}

extern void algebraic_restart(FILE *input_file);
extern "C" int algebraic_wrap(void);
extern int algebraic_lex(void);
extern int algebraic_parse(void);
extern int algebraic_lineno;
extern int algebraic_debug;

#endif // CSIM_PARSER_ALGEBRAIC_DEFS_H_
