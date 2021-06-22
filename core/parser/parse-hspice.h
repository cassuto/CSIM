/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_HSPICE_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_HSPICE_H_INCLUDED
# define YY_HSPICE_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_HSPICE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int hspice_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    K_Text = 258,
    K_Unsigned = 259,
    K_Number = 260,
    K_Nodes = 261,
    K_EOL = 262,
    K_Title = 263,
    K_Model = 264,
    K_ModelType = 265,
    K_Subckt = 266,
    K_Ends = 267,
    K_Equ = 268,
    K_Algebraic = 269,
    K_Param = 270,
    K_End = 271
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 42 "/cygdrive/e/FastCSIM/core/parser/parse-hspice.ypp" /* yacc.c:1909  */

    char *text;
    csim::HSPICE_KeyValueList *kvs;
    csim::HSPICE_Block *block;
    csim::HSPICE_SubBlock *subBlock;
    csim::HSPICE_Nodes *nodes;

#line 79 "/cygdrive/e/FastCSIM/core/parser/parse-hspice.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE hspice_lval;

int hspice_parse (void);

#endif /* !YY_HSPICE_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_HSPICE_H_INCLUDED  */
