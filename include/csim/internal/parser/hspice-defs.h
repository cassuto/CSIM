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

#ifndef CSIM_PARSER_HSPICE_DEFS_H_
#define CSIM_PARSER_HSPICE_DEFS_H_

#include <string>
#include <vector>
#include <cstdio>
#include "csim/model/Variant.h"

namespace csim
{
    class HSPICE_KeyValueList
    {
    public:
        std::vector<std::pair<std::string, csimModel::Variant>> kvs;
    };

    class HSPICE_Model
    {
    public:
        HSPICE_Model() : params(nullptr)
        {
        }

    public:
        std::string name, type;
        HSPICE_KeyValueList *params; /* deleted by ~HSPICE_Block() */
    };

    class HSPICE_Nodes
    {
    public:
        std::vector<std::string> nodes;
    };

    class HSPICE_SubBlock;
    class HSPICE_Block
    {
    public:
        ~HSPICE_Block();

    public:
        std::vector<HSPICE_SubBlock *> subs;
        std::vector<HSPICE_Model> models;
    };

    class HSPICE_SubBlock
    {
    public:
        HSPICE_SubBlock(const char *name_)
            : name(name_),
              nodes(nullptr),
              block(nullptr)
        {
        }
        ~HSPICE_SubBlock();

    public:
        std::string name;
        HSPICE_Nodes *nodes;
        HSPICE_Block *block;
    };

    class HSPICE_AST
    {
    public:
        HSPICE_AST();
        ~HSPICE_AST();
        int parse(const char *filename);
        void dump();
        void dump(HSPICE_Block *block, int level);

        inline const char *getTitle() const
        {
            return title.c_str();
        }

    private:
        void dumpIndent(int level);

    public:
        std::string title;
        HSPICE_Block *blockRoot;
    };

    extern HSPICE_AST *hspice_ast;

    csimModel::Variant hspice_parseReal(const char *str);
}

extern FILE *hspice_in;
extern int hspice_lineno;
extern "C" int hspice_wrap(void);
extern void hspice_restart(FILE *input_file);
extern int hspice_lex(void);
extern int hspice_parse(void);

#endif // CSIM_PARSER_HSPICE_DEFS_H_
