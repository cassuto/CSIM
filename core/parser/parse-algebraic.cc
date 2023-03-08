/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         algebraic_parse
#define yylex           algebraic_lex
#define yyerror         algebraic_error
#define yydebug         algebraic_debug
#define yynerrs         algebraic_nerrs
#define yylval          algebraic_lval
#define yychar          algebraic_char

/* First part of user prologue.  */
#line 1 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"

/**
 * @file Lexer of HSPICE-compatible circuit description
 */

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
#include <limits>
#include <cmath>
#include <cstring>
#include "csim/utils/string.h"
#include "csim/internal/parser/algebraic-defs.h"
#include "csim/internal/parser/hspice-defs.h"
#include "parse-algebraic.h"
static void yyerror(char const *msg) {
    std::cerr<<"Error: "<<msg<<std::endl;
    algebraic_wrap();
    csim::algebraic_err = true;
}

#line 109 "/home/ain/CSIM/core/parser/parse-algebraic.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parse-algebraic.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_K_Text = 3,                     /* K_Text  */
  YYSYMBOL_K_Unsigned = 4,                 /* K_Unsigned  */
  YYSYMBOL_K_Number = 5,                   /* K_Number  */
  YYSYMBOL_K_ExprNot = 6,                  /* K_ExprNot  */
  YYSYMBOL_K_ExprPow = 7,                  /* K_ExprPow  */
  YYSYMBOL_K_ExprMul = 8,                  /* K_ExprMul  */
  YYSYMBOL_K_ExprMod = 9,                  /* K_ExprMod  */
  YYSYMBOL_K_ExprDiv = 10,                 /* K_ExprDiv  */
  YYSYMBOL_K_ExprIntDiv = 11,              /* K_ExprIntDiv  */
  YYSYMBOL_K_ExprPlus = 12,                /* K_ExprPlus  */
  YYSYMBOL_K_ExprMinus = 13,               /* K_ExprMinus  */
  YYSYMBOL_K_ExprEq = 14,                  /* K_ExprEq  */
  YYSYMBOL_K_ExprNe = 15,                  /* K_ExprNe  */
  YYSYMBOL_K_ExprLe = 16,                  /* K_ExprLe  */
  YYSYMBOL_K_ExprGe = 17,                  /* K_ExprGe  */
  YYSYMBOL_K_ExprLt = 18,                  /* K_ExprLt  */
  YYSYMBOL_K_ExprGt = 19,                  /* K_ExprGt  */
  YYSYMBOL_K_ExprAnd = 20,                 /* K_ExprAnd  */
  YYSYMBOL_K_ExprOr = 21,                  /* K_ExprOr  */
  YYSYMBOL_K_ExprSel = 22,                 /* K_ExprSel  */
  YYSYMBOL_K_ExprColon = 23,               /* K_ExprColon  */
  YYSYMBOL_K_ExprLPar = 24,                /* K_ExprLPar  */
  YYSYMBOL_K_ExprRPar = 25,                /* K_ExprRPar  */
  YYSYMBOL_K_ExprId = 26,                  /* K_ExprId  */
  YYSYMBOL_K_ExprComma = 27,               /* K_ExprComma  */
  YYSYMBOL_YYACCEPT = 28,                  /* $accept  */
  YYSYMBOL_S = 29,                         /* S  */
  YYSYMBOL_Expr = 30,                      /* Expr  */
  YYSYMBOL_ExprOr = 31,                    /* ExprOr  */
  YYSYMBOL_ExprAnd = 32,                   /* ExprAnd  */
  YYSYMBOL_ExprEQ = 33,                    /* ExprEQ  */
  YYSYMBOL_ExprRelation = 34,              /* ExprRelation  */
  YYSYMBOL_ExprTerm = 35,                  /* ExprTerm  */
  YYSYMBOL_ExprFactor = 36,                /* ExprFactor  */
  YYSYMBOL_ExprPow = 37,                   /* ExprPow  */
  YYSYMBOL_ExprUnary = 38,                 /* ExprUnary  */
  YYSYMBOL_ExprFunc = 39,                  /* ExprFunc  */
  YYSYMBOL_Atom = 40                       /* Atom  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   56

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  37
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  66

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    54,    54,    59,    62,    68,    71,    77,    80,    86,
      89,    92,    98,   101,   104,   107,   110,   116,   119,   122,
     128,   131,   134,   137,   140,   147,   150,   157,   160,   163,
     166,   172,   175,   233,   250,   258,   262,   266
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "K_Text", "K_Unsigned",
  "K_Number", "K_ExprNot", "K_ExprPow", "K_ExprMul", "K_ExprMod",
  "K_ExprDiv", "K_ExprIntDiv", "K_ExprPlus", "K_ExprMinus", "K_ExprEq",
  "K_ExprNe", "K_ExprLe", "K_ExprGe", "K_ExprLt", "K_ExprGt", "K_ExprAnd",
  "K_ExprOr", "K_ExprSel", "K_ExprColon", "K_ExprLPar", "K_ExprRPar",
  "K_ExprId", "K_ExprComma", "$accept", "S", "Expr", "ExprOr", "ExprAnd",
  "ExprEQ", "ExprRelation", "ExprTerm", "ExprFactor", "ExprPow",
  "ExprUnary", "ExprFunc", "Atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-24)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -2,   -24,   -24,     1,     1,     1,    -2,   -23,    16,   -24,
      21,     3,    30,    -4,    34,    20,    13,   -24,   -24,   -24,
     -24,   -24,   -24,     7,    -2,   -24,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,   -24,    -8,     3,    18,    30,    -4,    -4,    34,
      34,    34,    34,    20,    20,    13,    13,    13,    13,   -24,
     -24,    -2,    -2,    27,   -24,   -24
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    35,    36,     0,     0,     0,     0,    34,     0,     2,
       3,     5,     7,     9,    12,    17,    20,    25,    27,    31,
      30,    28,    29,     0,     0,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     6,     0,     8,    10,    11,    15,
      16,    13,    14,    18,    19,    21,    24,    22,    23,    26,
      32,     0,     0,     0,     4,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -24,   -24,    -6,   -24,    28,    25,    19,     2,    15,     0,
     -15,     4,   -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      23,    24,     1,     2,     3,     1,     2,    20,    21,    22,
       4,     5,    31,    32,    33,    34,    25,    60,    43,    61,
      41,    45,     6,    28,     7,     6,    59,     7,    37,    38,
      39,    40,    42,    49,    50,    51,    52,    55,    56,    57,
      58,    62,    26,    27,    29,    30,    35,    36,    47,    48,
      53,    54,    65,    46,    44,    63,    64
};

static const yytype_int8 yycheck[] =
{
       6,    24,     4,     5,     6,     4,     5,     3,     4,     5,
      12,    13,    16,    17,    18,    19,     0,    25,    24,    27,
       7,    27,    24,    20,    26,    24,    41,    26,     8,     9,
      10,    11,    25,    31,    32,    33,    34,    37,    38,    39,
      40,    23,    21,    22,    14,    15,    12,    13,    29,    30,
      35,    36,    25,    28,    26,    61,    62
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     6,    12,    13,    24,    26,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      39,    39,    39,    30,    24,     0,    21,    22,    20,    14,
      15,    16,    17,    18,    19,    12,    13,     8,     9,    10,
      11,     7,    25,    30,    32,    30,    33,    34,    34,    35,
      35,    35,    35,    36,    36,    37,    37,    37,    37,    38,
      25,    27,    23,    30,    30,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    28,    29,    30,    30,    31,    31,    32,    32,    33,
      33,    33,    34,    34,    34,    34,    34,    35,    35,    35,
      36,    36,    36,    36,    36,    37,    37,    38,    38,    38,
      38,    39,    39,    39,    40,    40,    40,    40
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     5,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     1,     2,     2,
       2,     1,     4,     6,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* S: Expr  */
#line 54 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
        {
    csim::algebraic_result = (yyvsp[0].real);
}
#line 1172 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 3: /* Expr: ExprOr  */
#line 59 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
           {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1180 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 4: /* Expr: ExprOr K_ExprSel Expr K_ExprColon Expr  */
#line 62 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                                 {
        (yyval.real) = (std::fabs((yyvsp[-4].real))>std::numeric_limits<float>::epsilon()) ? (yyvsp[-2].real) : (yyvsp[0].real);
    }
#line 1188 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 5: /* ExprOr: ExprAnd  */
#line 68 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
            {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1196 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 6: /* ExprOr: ExprOr K_ExprOr ExprAnd  */
#line 71 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                  {
        (yyval.real) = ((std::fabs((yyvsp[-2].real))>std::numeric_limits<float>::epsilon()) || (std::fabs((yyvsp[0].real))>std::numeric_limits<float>::epsilon())) ? 1.0 : 0.0;
    }
#line 1204 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 7: /* ExprAnd: ExprEQ  */
#line 77 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
           {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1212 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 8: /* ExprAnd: ExprAnd K_ExprAnd ExprEQ  */
#line 80 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                   {
        (yyval.real) = ((std::fabs((yyvsp[-2].real))<=std::numeric_limits<float>::epsilon()) && (std::fabs((yyvsp[0].real))<=std::numeric_limits<float>::epsilon())) ? 1.0 : 0.0;
    }
#line 1220 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 9: /* ExprEQ: ExprRelation  */
#line 86 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                 {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1228 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 10: /* ExprEQ: ExprEQ K_ExprEq ExprRelation  */
#line 89 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                       {
        (yyval.real) = ((yyvsp[-2].real) == (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1236 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 11: /* ExprEQ: ExprEQ K_ExprNe ExprRelation  */
#line 92 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                       {
        (yyval.real) = ((yyvsp[-2].real) != (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1244 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 12: /* ExprRelation: ExprTerm  */
#line 98 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
             {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1252 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 13: /* ExprRelation: ExprRelation K_ExprLt ExprTerm  */
#line 101 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                         {
        (yyval.real) = ((yyvsp[-2].real) < (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1260 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 14: /* ExprRelation: ExprRelation K_ExprGt ExprTerm  */
#line 104 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                         {
        (yyval.real) = ((yyvsp[-2].real) > (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1268 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 15: /* ExprRelation: ExprRelation K_ExprLe ExprTerm  */
#line 107 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                         {
        (yyval.real) = ((yyvsp[-2].real) <= (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1276 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 16: /* ExprRelation: ExprRelation K_ExprGe ExprTerm  */
#line 110 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                         {
        (yyval.real) = ((yyvsp[-2].real) >= (yyvsp[0].real)) ? 1.0 : 0.0;
    }
#line 1284 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 17: /* ExprTerm: ExprFactor  */
#line 116 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
               {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1292 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 18: /* ExprTerm: ExprTerm K_ExprPlus ExprFactor  */
#line 119 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                         {
        (yyval.real) = (yyvsp[-2].real) + (yyvsp[0].real);
    }
#line 1300 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 19: /* ExprTerm: ExprTerm K_ExprMinus ExprFactor  */
#line 122 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                          {
        (yyval.real) = (yyvsp[-2].real) - (yyvsp[0].real);
    }
#line 1308 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 20: /* ExprFactor: ExprPow  */
#line 128 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
            {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1316 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 21: /* ExprFactor: ExprFactor K_ExprMul ExprPow  */
#line 131 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                       {
        (yyval.real) = (yyvsp[-2].real) * (yyvsp[0].real);
    }
#line 1324 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 22: /* ExprFactor: ExprFactor K_ExprDiv ExprPow  */
#line 134 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                       {
        (yyval.real) = (yyvsp[-2].real) / (yyvsp[0].real);
    }
#line 1332 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 23: /* ExprFactor: ExprFactor K_ExprIntDiv ExprPow  */
#line 137 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                      {
        (yyval.real) = csim::algebraic_trunc(std::fabs((yyvsp[-2].real) / (yyvsp[0].real)));
    }
#line 1340 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 24: /* ExprFactor: ExprFactor K_ExprMod ExprPow  */
#line 140 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                       {
        double t = csim::algebraic_trunc((yyvsp[-2].real) / (yyvsp[0].real));
        (yyval.real) = (yyvsp[-2].real) - (yyvsp[0].real) * t;
    }
#line 1349 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 25: /* ExprPow: ExprUnary  */
#line 147 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
              {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1357 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 26: /* ExprPow: ExprPow K_ExprPow ExprUnary  */
#line 150 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                  {
        (yyval.real) = std::pow(std::fabs((yyvsp[-2].real)),(yyvsp[0].real));
    }
#line 1365 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 27: /* ExprUnary: ExprFunc  */
#line 157 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
             {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1373 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 28: /* ExprUnary: K_ExprPlus ExprFunc  */
#line 160 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                          {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1381 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 29: /* ExprUnary: K_ExprMinus ExprFunc  */
#line 163 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                               {
        (yyval.real) = -(yyvsp[0].real);
    }
#line 1389 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 30: /* ExprUnary: K_ExprNot ExprFunc  */
#line 166 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                         {
        (yyval.real) = ((yyvsp[0].real)<=std::numeric_limits<float>::epsilon()) ? 1.0 : 0.0;
    }
#line 1397 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 31: /* ExprFunc: Atom  */
#line 172 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
         {
        (yyval.real) = (yyvsp[0].real);
    }
#line 1405 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 32: /* ExprFunc: K_ExprId K_ExprLPar Expr K_ExprRPar  */
#line 175 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                          {
        if (0==csim::strcmp_nocase((yyvsp[-3].text), "sqr"))
            (yyval.real) = (yyvsp[-1].real) * (yyvsp[-1].real);
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "sqrt"))
            (yyval.real) = std::sqrt((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "sin"))
            (yyval.real) = std::sin((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "cos"))
            (yyval.real) = std::cos((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "tan"))
            (yyval.real) = std::tan((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "sinh"))
            (yyval.real) = std::sinh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "cosh"))
            (yyval.real) = std::cosh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "tanh"))
            (yyval.real) = std::tanh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "asin"))
            (yyval.real) = std::asin((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "acos"))
            (yyval.real) = std::acos((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "atan"))
            (yyval.real) = std::atan((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "asinh"))
            (yyval.real) = std::asinh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "acosh"))
            (yyval.real) = std::acosh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "atanh"))
            (yyval.real) = std::atanh((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "arctan"))
            (yyval.real) = std::atan((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "exp"))
            (yyval.real) = std::exp((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "ln"))
            (yyval.real) = std::log((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "log"))
            (yyval.real) = std::log((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "log10"))
            (yyval.real) = std::log10((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "abs"))
            (yyval.real) = std::fabs((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "nint"))
            (yyval.real) = std::nearbyint((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "nint"))
            (yyval.real) = std::nearbyint((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "int"))
            (yyval.real) = csim::algebraic_trunc((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "floor"))
            (yyval.real) = std::floor((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "ceil"))
            (yyval.real) = std::ceil((yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-3].text), "sgn"))
            (yyval.real) = ((yyvsp[-1].real) > 0.0) ? 1.0 : ((yyvsp[-1].real) < 0.0) ? -1.0 : 0.0;
        else {
            algebraic_error("Unknown function");
        }
        free((yyvsp[-3].text));
    }
#line 1468 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 33: /* ExprFunc: K_ExprId K_ExprLPar Expr K_ExprComma Expr K_ExprRPar  */
#line 233 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                                           {
        if (0==csim::strcmp_nocase((yyvsp[-5].text), "pow"))
            (yyval.real) = std::pow((yyvsp[-3].real), (yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-5].text), "pwr"))
            (yyval.real) = std::pow(std::fabs((yyvsp[-3].real)), (yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-5].text), "min"))
            (yyval.real) = std::min((yyvsp[-3].real), (yyvsp[-1].real));
        else if (0==csim::strcmp_nocase((yyvsp[-5].text), "max"))
            (yyval.real) = std::max((yyvsp[-3].real), (yyvsp[-1].real));
        else {
            algebraic_error("Unknown function");
        }
        free((yyvsp[-5].text));
    }
#line 1487 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 34: /* Atom: K_ExprId  */
#line 250 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                 {
        double val;
        if (csim::algebraic_currentScope->getValue((yyvsp[0].text),&val)) {
            yyerror("Variable undefined");
        }
        (yyval.real) = val;
        free((yyvsp[0].text));
    }
#line 1500 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 35: /* Atom: K_Unsigned  */
#line 258 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                     {
        (yyval.real) = csim::algebraic_parseReal((yyvsp[0].text));
        free((yyvsp[0].text));
    }
#line 1509 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 36: /* Atom: K_Number  */
#line 262 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                   {
        (yyval.real) = csim::algebraic_parseReal((yyvsp[0].text));
        free((yyvsp[0].text));
    }
#line 1518 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;

  case 37: /* Atom: K_ExprLPar Expr K_ExprRPar  */
#line 266 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"
                                     {
        (yyval.real) = (yyvsp[-1].real);
    }
#line 1526 "/home/ain/CSIM/core/parser/parse-algebraic.cc"
    break;


#line 1530 "/home/ain/CSIM/core/parser/parse-algebraic.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 271 "/home/ain/CSIM/core/parser/parse-algebraic.ypp"


/**/
