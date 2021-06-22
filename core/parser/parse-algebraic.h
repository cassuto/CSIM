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

#ifndef YY_ALGEBRAIC_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_ALGEBRAIC_H_INCLUDED
# define YY_ALGEBRAIC_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_ALGEBRAIC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int algebraic_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    K_Text = 258,
    K_Unsigned = 259,
    K_Number = 260,
    K_ExprBegin = 261,
    K_ExprEnd = 262,
    K_ExprNot = 263,
    K_ExprPow = 264,
    K_ExprMul = 265,
    K_ExprMod = 266,
    K_ExprDiv = 267,
    K_ExprIntDiv = 268,
    K_ExprPlus = 269,
    K_ExprMinus = 270,
    K_ExprEq = 271,
    K_ExprNe = 272,
    K_ExprLe = 273,
    K_ExprGe = 274,
    K_ExprLt = 275,
    K_ExprGt = 276,
    K_ExprAnd = 277,
    K_ExprOr = 278,
    K_ExprSel = 279,
    K_ExprColon = 280,
    K_ExprLPar = 281,
    K_ExprRPar = 282,
    K_ExprId = 283,
    K_ExprComma = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 42 "/cygdrive/e/FastCSIM/core/parser/parse-algebraic.ypp" /* yacc.c:1909  */

    char *text;
    double real;

#line 89 "/cygdrive/e/FastCSIM/core/parser/parse-algebraic.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE algebraic_lval;

int algebraic_parse (void);

#endif /* !YY_ALGEBRAIC_CYGDRIVE_E_FASTCSIM_CORE_PARSER_PARSE_ALGEBRAIC_H_INCLUDED  */
