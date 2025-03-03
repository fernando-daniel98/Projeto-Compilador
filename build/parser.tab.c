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




/* First part of user prologue.  */
#line 1 "./parser/parser.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

extern int lineNum;
extern int inFunctionScope;

extern int yylex(void);
void yyerror(const char *s);

int syntax_errors = 0;
extern int lexical_errors;
int semantic_errors = 0;

static TreeNode *savedTree;


#line 96 "./build/parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_NUM = 4,                        /* NUM  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_WHILE = 7,                      /* WHILE  */
  YYSYMBOL_RETURN = 8,                     /* RETURN  */
  YYSYMBOL_PLUS = 9,                       /* PLUS  */
  YYSYMBOL_MINUS = 10,                     /* MINUS  */
  YYSYMBOL_MULT = 11,                      /* MULT  */
  YYSYMBOL_DIV = 12,                       /* DIV  */
  YYSYMBOL_SMAL = 13,                      /* SMAL  */
  YYSYMBOL_SMALEQ = 14,                    /* SMALEQ  */
  YYSYMBOL_GREAT = 15,                     /* GREAT  */
  YYSYMBOL_GREATEQ = 16,                   /* GREATEQ  */
  YYSYMBOL_EQ = 17,                        /* EQ  */
  YYSYMBOL_DIFF = 18,                      /* DIFF  */
  YYSYMBOL_ASSIGN = 19,                    /* ASSIGN  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_VOID = 21,                      /* VOID  */
  YYSYMBOL_ERROR = 22,                     /* ERROR  */
  YYSYMBOL_SEMICOL = 23,                   /* SEMICOL  */
  YYSYMBOL_COMMA = 24,                     /* COMMA  */
  YYSYMBOL_LPAREN = 25,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 26,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 27,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 28,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 29,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 30,                    /* RBRACE  */
  YYSYMBOL_LOWER_THAN_ELSE = 31,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UMINUS = 32,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 33,                  /* $accept  */
  YYSYMBOL_programa = 34,                  /* programa  */
  YYSYMBOL_declaracao_lista = 35,          /* declaracao_lista  */
  YYSYMBOL_declaracao = 36,                /* declaracao  */
  YYSYMBOL_var_declaracao = 37,            /* var_declaracao  */
  YYSYMBOL_tipo_especificador = 38,        /* tipo_especificador  */
  YYSYMBOL_fun_declaracao = 39,            /* fun_declaracao  */
  YYSYMBOL_params = 40,                    /* params  */
  YYSYMBOL_param_lista = 41,               /* param_lista  */
  YYSYMBOL_param = 42,                     /* param  */
  YYSYMBOL_composto_decl = 43,             /* composto_decl  */
  YYSYMBOL_local_declaracoes = 44,         /* local_declaracoes  */
  YYSYMBOL_statement_lista = 45,           /* statement_lista  */
  YYSYMBOL_statement = 46,                 /* statement  */
  YYSYMBOL_expressao_decl = 47,            /* expressao_decl  */
  YYSYMBOL_selecao_decl = 48,              /* selecao_decl  */
  YYSYMBOL_iteracao_decl = 49,             /* iteracao_decl  */
  YYSYMBOL_retorno_decl = 50,              /* retorno_decl  */
  YYSYMBOL_expressao = 51,                 /* expressao  */
  YYSYMBOL_var = 52,                       /* var  */
  YYSYMBOL_simples_expressao = 53,         /* simples_expressao  */
  YYSYMBOL_relacional = 54,                /* relacional  */
  YYSYMBOL_soma_expressao = 55,            /* soma_expressao  */
  YYSYMBOL_soma = 56,                      /* soma  */
  YYSYMBOL_termo = 57,                     /* termo  */
  YYSYMBOL_mult = 58,                      /* mult  */
  YYSYMBOL_fator = 59,                     /* fator  */
  YYSYMBOL_ativacao = 60,                  /* ativacao  */
  YYSYMBOL_args = 61,                      /* args  */
  YYSYMBOL_arg_lista = 62                  /* arg_lista  */
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

#if 1

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
#endif /* 1 */

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   111

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  110

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   287


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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    72,    72,    75,    88,    92,    93,    97,   106,   111,
     121,   129,   130,   134,   148,   149,   158,   168,   172,   187,
     204,   215,   225,   229,   239,   243,   244,   245,   246,   247,
     251,   252,   253,   261,   270,   282,   293,   299,   309,   317,
     321,   322,   334,   341,   345,   347,   349,   351,   353,   355,
     359,   366,   370,   372,   376,   383,   387,   389,   393,   394,
     395,   396,   397,   407,   419,   420,   424,   434
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUM", "IF",
  "ELSE", "WHILE", "RETURN", "PLUS", "MINUS", "MULT", "DIV", "SMAL",
  "SMALEQ", "GREAT", "GREATEQ", "EQ", "DIFF", "ASSIGN", "INT", "VOID",
  "ERROR", "SEMICOL", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET",
  "LBRACE", "RBRACE", "LOWER_THAN_ELSE", "UMINUS", "$accept", "programa",
  "declaracao_lista", "declaracao", "var_declaracao", "tipo_especificador",
  "fun_declaracao", "params", "param_lista", "param", "composto_decl",
  "local_declaracoes", "statement_lista", "statement", "expressao_decl",
  "selecao_decl", "iteracao_decl", "retorno_decl", "expressao", "var",
  "simples_expressao", "relacional", "soma_expressao", "soma", "termo",
  "mult", "fator", "ativacao", "args", "arg_lista", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-16)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      21,   -72,   -72,    11,    21,   -72,   -72,    24,   -72,   -72,
     -72,    29,    35,   -72,     1,   -72,    50,    10,   -72,    23,
      28,    33,    40,   -72,    47,    60,    59,    21,    66,    63,
     -72,   -72,   -72,   -72,   -72,    21,   -72,    62,     5,   -22,
      67,    42,   -72,    68,    69,    13,    51,   -72,    51,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,    73,    78,   -72,    64,
      72,   -72,   -72,   -72,    51,    51,    51,    51,   -72,    75,
     -72,   -72,    74,   -72,    51,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,    51,    51,   -72,   -72,    51,   -72,    77,
      71,    79,    80,    82,   -72,   -72,   -72,    76,    72,   -72,
     -72,    51,   -72,    43,    43,   -72,    86,   -72,    43,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    11,    12,     0,     2,     4,     5,     0,     6,     1,
       3,     0,     0,     8,     0,     7,     0,     0,    10,    12,
       0,     0,    14,    17,     0,    18,     0,     0,     0,     0,
      22,    13,    16,     9,    19,    24,    21,     0,     0,     0,
       0,    40,    61,     0,     0,     0,     0,    31,     0,    20,
      26,    23,    25,    27,    28,    29,     0,    59,    39,    43,
      51,    55,    60,    32,    65,     0,     0,     0,    36,     0,
      59,    62,     0,    30,     0,    52,    53,    44,    45,    46,
      47,    48,    49,     0,     0,    56,    57,     0,    67,     0,
      64,     0,     0,     0,    37,    58,    38,    42,    50,    54,
      63,     0,    41,     0,     0,    66,    33,    35,     0,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,   -72,    95,    70,    -9,   -72,   -72,   -72,    83,
      85,   -72,   -72,   -71,   -72,   -72,   -72,   -72,   -45,   -44,
     -72,   -72,    18,   -72,    20,   -72,   -42,   -72,   -72,   -72
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,     6,     7,     8,    21,    22,    23,
      50,    35,    38,    51,    52,    53,    54,    55,    56,    57,
      58,    83,    59,    84,    60,    87,    61,    62,    89,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      69,    15,    70,    72,    71,    17,    40,    20,    41,    42,
      43,     9,    44,    45,    24,    46,    41,    42,    20,    88,
      91,    92,    93,    46,    18,    11,    37,    12,    47,    96,
      48,    25,   106,   107,    30,    49,    68,   109,    48,    70,
      70,     1,     2,    70,    40,    99,    41,    42,    43,   -15,
      44,    45,    13,    46,    41,    42,   105,    14,    15,    26,
      16,    46,    17,    11,    27,    39,    47,    64,    48,    65,
       1,    19,    30,    75,    76,    28,    48,    77,    78,    79,
      80,    81,    82,    85,    86,    75,    76,    29,    30,    33,
      63,    34,   108,    66,    67,   101,    73,    74,    94,    10,
      95,    97,     0,   100,    98,    36,   103,   102,   104,     0,
      32,    31
};

static const yytype_int8 yycheck[] =
{
      45,    23,    46,    48,    46,    27,     1,    16,     3,     4,
       5,     0,     7,     8,     4,    10,     3,     4,    27,    64,
      65,    66,    67,    10,    23,     1,    35,     3,    23,    74,
      25,     3,   103,   104,    29,    30,    23,   108,    25,    83,
      84,    20,    21,    87,     1,    87,     3,     4,     5,    26,
       7,     8,    23,    10,     3,     4,   101,    28,    23,    26,
      25,    10,    27,     1,    24,     3,    23,    25,    25,    27,
      20,    21,    29,     9,    10,    28,    25,    13,    14,    15,
      16,    17,    18,    11,    12,     9,    10,    27,    29,    23,
      23,    28,     6,    25,    25,    24,    23,    19,    23,     4,
      26,    83,    -1,    26,    84,    35,    26,    28,    26,    -1,
      27,    26
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    21,    34,    35,    36,    37,    38,    39,     0,
      36,     1,     3,    23,    28,    23,    25,    27,    23,    21,
      38,    40,    41,    42,     4,     3,    26,    24,    28,    27,
      29,    43,    42,    23,    28,    44,    37,    38,    45,     3,
       1,     3,     4,     5,     7,     8,    10,    23,    25,    30,
      43,    46,    47,    48,    49,    50,    51,    52,    53,    55,
      57,    59,    60,    23,    25,    27,    25,    25,    23,    51,
      52,    59,    51,    23,    19,     9,    10,    13,    14,    15,
      16,    17,    18,    54,    56,    11,    12,    58,    51,    61,
      62,    51,    51,    51,    23,    26,    51,    55,    57,    59,
      26,    24,    28,    26,    26,    51,    46,    46,     6,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    37,    37,    37,
      37,    38,    38,    39,    40,    40,    41,    41,    42,    42,
      43,    44,    44,    45,    45,    46,    46,    46,    46,    46,
      47,    47,    47,    48,    48,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    54,    54,    54,    54,    54,    54,
      55,    55,    56,    56,    57,    57,    58,    58,    59,    59,
      59,    59,    59,    60,    61,    61,    62,    62
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     3,     6,
       4,     1,     1,     6,     1,     1,     3,     1,     2,     4,
       4,     2,     0,     2,     0,     1,     1,     1,     1,     1,
       2,     1,     2,     5,     7,     5,     2,     3,     3,     1,
       1,     4,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     2,     4,     1,     0,     3,     1
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


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

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
  case 2: /* programa: declaracao_lista  */
#line 72 "./parser/parser.y"
                           { savedTree = (yyvsp[0].node);}
#line 1487 "./build/parser.tab.c"
    break;

  case 3: /* declaracao_lista: declaracao_lista declaracao  */
#line 76 "./parser/parser.y"
        { 
            TreeNode* t = (yyvsp[-1].node);
            if (t != NULL)
            { 
                while (t->sibling != NULL) {
                    t = t->sibling;
                }
                t->sibling = (yyvsp[0].node);
                (yyval.node) = (yyvsp[-1].node);
            }
            else (yyval.node) = (yyvsp[0].node);
        }
#line 1504 "./build/parser.tab.c"
    break;

  case 4: /* declaracao_lista: declaracao  */
#line 88 "./parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1510 "./build/parser.tab.c"
    break;

  case 5: /* declaracao: var_declaracao  */
#line 92 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node);}
#line 1516 "./build/parser.tab.c"
    break;

  case 6: /* declaracao: fun_declaracao  */
#line 93 "./parser/parser.y"
                     { (yyval.node) = (yyvsp[0].node);}
#line 1522 "./build/parser.tab.c"
    break;

  case 7: /* var_declaracao: tipo_especificador ID SEMICOL  */
#line 98 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VarDeclK;
        t->attr.name = (yyvsp[-1].node)->attr.name;
        t->type = (yyvsp[-2].type);
        free((yyvsp[-1].node));
        (yyval.node) = t;
    }
#line 1535 "./build/parser.tab.c"
    break;

  case 8: /* var_declaracao: tipo_especificador error SEMICOL  */
#line 106 "./parser/parser.y"
                                       {
        yyerrok;
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração inválida\n");
        (yyval.node) = NULL;
    }
#line 1545 "./build/parser.tab.c"
    break;

  case 9: /* var_declaracao: tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL  */
#line 112 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetDeclK;
        t->attr.name = (yyvsp[-4].node)->attr.name;
        t->type = (yyvsp[-5].type);
        t->child[0] = (yyvsp[-2].node);
        free((yyvsp[-4].node));
        (yyval.node) = t;
    }
#line 1559 "./build/parser.tab.c"
    break;

  case 10: /* var_declaracao: tipo_especificador error RBRACKET SEMICOL  */
#line 121 "./parser/parser.y"
                                                {
        yyerrok;
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Ignorando declaração de vetor inválida\n");
        (yyval.node) = NULL;
    }
#line 1569 "./build/parser.tab.c"
    break;

  case 11: /* tipo_especificador: INT  */
#line 129 "./parser/parser.y"
         { (yyval.type) = Integer; }
#line 1575 "./build/parser.tab.c"
    break;

  case 12: /* tipo_especificador: VOID  */
#line 130 "./parser/parser.y"
           { (yyval.type) = Void; }
#line 1581 "./build/parser.tab.c"
    break;

  case 13: /* fun_declaracao: tipo_especificador ID LPAREN params RPAREN composto_decl  */
#line 134 "./parser/parser.y"
                                                             {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = FunDeclK;
        t->attr.name = (yyvsp[-4].node)->attr.name;
        t->type = (yyvsp[-5].type);
        t->child[0] = (yyvsp[-2].node);  // params
        t->child[1] = (yyvsp[0].node);  // compound statement
        t->lineno = (yyvsp[-4].node)->lineno;  // Set line number from the ID token
        free((yyvsp[-4].node));
        (yyval.node) = t;
    }
#line 1597 "./build/parser.tab.c"
    break;

  case 14: /* params: param_lista  */
#line 148 "./parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1603 "./build/parser.tab.c"
    break;

  case 15: /* params: VOID  */
#line 150 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ParamVoid;
        (yyval.node) = t;
    }
#line 1613 "./build/parser.tab.c"
    break;

  case 16: /* param_lista: param_lista COMMA param  */
#line 159 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1627 "./build/parser.tab.c"
    break;

  case 17: /* param_lista: param  */
#line 168 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1633 "./build/parser.tab.c"
    break;

  case 18: /* param: tipo_especificador ID  */
#line 173 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VarParamK;

        if ((yyvsp[0].node) && (yyvsp[0].node)->attr.name) {
            t->attr.name = strdup((yyvsp[0].node)->attr.name);
        }

        t->type = (yyvsp[-1].type);
        
        free((yyvsp[0].node));
        
        (yyval.node) = t;
    }
#line 1652 "./build/parser.tab.c"
    break;

  case 19: /* param: tipo_especificador ID LBRACKET RBRACKET  */
#line 188 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetParamK;
        
        if ((yyvsp[-2].node) && (yyvsp[-2].node)->attr.name) {
            t->attr.name = strdup((yyvsp[-2].node)->attr.name);
        } 
        
        t->type = (yyvsp[-3].type);
        
        free((yyvsp[-2].node));
        (yyval.node) = t;
    }
#line 1670 "./build/parser.tab.c"
    break;

  case 20: /* composto_decl: LBRACE local_declaracoes statement_lista RBRACE  */
#line 205 "./parser/parser.y"
        {
            TreeNode* t = newNode(StatementK);
            t->kind.stmt = NuloDecl;
            t->child[0] = (yyvsp[-2].node);  // local declarations
            t->child[1] = (yyvsp[-1].node);  // statements
            (yyval.node) = t;
        }
#line 1682 "./build/parser.tab.c"
    break;

  case 21: /* local_declaracoes: local_declaracoes var_declaracao  */
#line 216 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1696 "./build/parser.tab.c"
    break;

  case 22: /* local_declaracoes: %empty  */
#line 225 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 1702 "./build/parser.tab.c"
    break;

  case 23: /* statement_lista: statement_lista statement  */
#line 230 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1716 "./build/parser.tab.c"
    break;

  case 24: /* statement_lista: %empty  */
#line 239 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 1722 "./build/parser.tab.c"
    break;

  case 25: /* statement: expressao_decl  */
#line 243 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1728 "./build/parser.tab.c"
    break;

  case 26: /* statement: composto_decl  */
#line 244 "./parser/parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1734 "./build/parser.tab.c"
    break;

  case 27: /* statement: selecao_decl  */
#line 245 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1740 "./build/parser.tab.c"
    break;

  case 28: /* statement: iteracao_decl  */
#line 246 "./parser/parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1746 "./build/parser.tab.c"
    break;

  case 29: /* statement: retorno_decl  */
#line 247 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1752 "./build/parser.tab.c"
    break;

  case 30: /* expressao_decl: expressao SEMICOL  */
#line 251 "./parser/parser.y"
                      { (yyval.node) = (yyvsp[-1].node); }
#line 1758 "./build/parser.tab.c"
    break;

  case 31: /* expressao_decl: SEMICOL  */
#line 252 "./parser/parser.y"
              { (yyval.node) = NULL; }
#line 1764 "./build/parser.tab.c"
    break;

  case 32: /* expressao_decl: error SEMICOL  */
#line 253 "./parser/parser.y"
                    { 
        yyerrok; 
        fprintf(stderr, ANSI_COLOR_GREEN "RECUPERAÇÃO DE ERRO: " ANSI_COLOR_RESET "Sincronizando em ';'\n"); 
        (yyval.node) = NULL; 
    }
#line 1774 "./build/parser.tab.c"
    break;

  case 33: /* selecao_decl: IF LPAREN expressao RPAREN statement  */
#line 262 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = (yyvsp[-2].node);  /* condition */
        t->child[1] = (yyvsp[0].node);  /* if-part */
        t->child[2] = NULL; /* else-part is null */
        (yyval.node) = t;
    }
#line 1787 "./build/parser.tab.c"
    break;

  case 34: /* selecao_decl: IF LPAREN expressao RPAREN statement ELSE statement  */
#line 271 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = (yyvsp[-4].node);  /* condition */
        t->child[1] = (yyvsp[-2].node);  /* if-part */
        t->child[2] = (yyvsp[0].node);  /* else-part */
        (yyval.node) = t;
    }
#line 1800 "./build/parser.tab.c"
    break;

  case 35: /* iteracao_decl: WHILE LPAREN expressao RPAREN statement  */
#line 283 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = WhileK;
        t->child[0] = (yyvsp[-2].node);  /* condition */
        t->child[1] = (yyvsp[0].node);  /* body */
        (yyval.node) = t;
    }
#line 1812 "./build/parser.tab.c"
    break;

  case 36: /* retorno_decl: RETURN SEMICOL  */
#line 294 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnVOID;
        (yyval.node) = t;
    }
#line 1822 "./build/parser.tab.c"
    break;

  case 37: /* retorno_decl: RETURN expressao SEMICOL  */
#line 300 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnINT;
        t->child[0] = (yyvsp[-1].node);  /* return value */
        (yyval.node) = t;
    }
#line 1833 "./build/parser.tab.c"
    break;

  case 38: /* expressao: var ASSIGN expressao  */
#line 310 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AssignK;
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1845 "./build/parser.tab.c"
    break;

  case 39: /* expressao: simples_expressao  */
#line 317 "./parser/parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1851 "./build/parser.tab.c"
    break;

  case 40: /* var: ID  */
#line 321 "./parser/parser.y"
       { (yyval.node) = (yyvsp[0].node); }
#line 1857 "./build/parser.tab.c"
    break;

  case 41: /* var: ID LBRACKET expressao RBRACKET  */
#line 323 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = VetorK;
        t->attr.name = (yyvsp[-3].node)->attr.name;
        t->child[0] = (yyvsp[-1].node);  // index expression
        free((yyvsp[-3].node));
        (yyval.node) = t;
    }
#line 1870 "./build/parser.tab.c"
    break;

  case 42: /* simples_expressao: soma_expressao relacional soma_expressao  */
#line 335 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);  // reuse relational operator node
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1881 "./build/parser.tab.c"
    break;

  case 43: /* simples_expressao: soma_expressao  */
#line 341 "./parser/parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1887 "./build/parser.tab.c"
    break;

  case 44: /* relacional: SMAL  */
#line 345 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1893 "./build/parser.tab.c"
    break;

  case 45: /* relacional: SMALEQ  */
#line 347 "./parser/parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1899 "./build/parser.tab.c"
    break;

  case 46: /* relacional: GREAT  */
#line 349 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1905 "./build/parser.tab.c"
    break;

  case 47: /* relacional: GREATEQ  */
#line 351 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1911 "./build/parser.tab.c"
    break;

  case 48: /* relacional: EQ  */
#line 353 "./parser/parser.y"
       { (yyval.node) = (yyvsp[0].node); }
#line 1917 "./build/parser.tab.c"
    break;

  case 49: /* relacional: DIFF  */
#line 355 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1923 "./build/parser.tab.c"
    break;

  case 50: /* soma_expressao: soma_expressao soma termo  */
#line 360 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);  // reuse operator node
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1934 "./build/parser.tab.c"
    break;

  case 51: /* soma_expressao: termo  */
#line 366 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1940 "./build/parser.tab.c"
    break;

  case 52: /* soma: PLUS  */
#line 370 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1946 "./build/parser.tab.c"
    break;

  case 53: /* soma: MINUS  */
#line 372 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1952 "./build/parser.tab.c"
    break;

  case 54: /* termo: termo mult fator  */
#line 377 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1963 "./build/parser.tab.c"
    break;

  case 55: /* termo: fator  */
#line 383 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1969 "./build/parser.tab.c"
    break;

  case 56: /* mult: MULT  */
#line 387 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1975 "./build/parser.tab.c"
    break;

  case 57: /* mult: DIV  */
#line 389 "./parser/parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1981 "./build/parser.tab.c"
    break;

  case 58: /* fator: LPAREN expressao RPAREN  */
#line 393 "./parser/parser.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1987 "./build/parser.tab.c"
    break;

  case 59: /* fator: var  */
#line 394 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1993 "./build/parser.tab.c"
    break;

  case 60: /* fator: ativacao  */
#line 395 "./parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1999 "./build/parser.tab.c"
    break;

  case 61: /* fator: NUM  */
#line 396 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2005 "./build/parser.tab.c"
    break;

  case 62: /* fator: MINUS fator  */
#line 398 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = OpK;
        t->child[0] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 2016 "./build/parser.tab.c"
    break;

  case 63: /* ativacao: ID LPAREN args RPAREN  */
#line 408 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AtivK;
        t->attr.name = (yyvsp[-3].node)->attr.name;
        t->child[0] = (yyvsp[-1].node);  // arguments
        free((yyvsp[-3].node));
        (yyval.node) = t;
    }
#line 2029 "./build/parser.tab.c"
    break;

  case 64: /* args: arg_lista  */
#line 419 "./parser/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2035 "./build/parser.tab.c"
    break;

  case 65: /* args: %empty  */
#line 420 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 2041 "./build/parser.tab.c"
    break;

  case 66: /* arg_lista: arg_lista COMMA expressao  */
#line 425 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 2055 "./build/parser.tab.c"
    break;

  case 67: /* arg_lista: expressao  */
#line 434 "./parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2061 "./build/parser.tab.c"
    break;


#line 2065 "./build/parser.tab.c"

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 437 "./parser/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, ANSI_COLOR_YELLOW "ERRO SINTÁTICO: " ANSI_COLOR_RESET ANSI_COLOR_WHITE "\"%s\" ", yytext);
    fprintf(stderr, ANSI_COLOR_YELLOW "LINHA: " ANSI_COLOR_WHITE "%d" ANSI_COLOR_RESET " | %s\n", lineNum, s);
    
    syntax_errors++;
}

int formaEntrada(int argc, char **argv) {
    if (argc == 1) {
        yyin = stdin;
        yyout = stdout;
    } else if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyout = stdout;
    } else if (argc == 3) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
            return 1;
        }
        yyout = fopen(argv[2], "w");
        if (yyout == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
            fclose(yyin);
            return 1;
        }
    } else {
        fprintf(stderr, "Usage: %s [input_file] [output_file]\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    printf("Starting program...\n");
    fflush(stdout);  // Force print

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    printf("Opening file: %s\n", argv[1]);
    fflush(stdout);

    // Check if formaEntrada is successful
    if (formaEntrada(argc, argv) != 0) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    // Verify file was opened successfully
    if (yyin == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }

    printf("File opened successfully\n");
    fflush(stdout);
    
    printf("Starting parse...\n");
    fflush(stdout);

    int parseResult = yyparse();
    
    printf("Parse completed with result: %d\n", parseResult);
    fflush(stdout);

    if (savedTree != NULL) {        
        // Inicializações para o gerenciamento correto de escopo
        if (symbolTable == NULL) {
            symbolTable = inicializaTabela();
        }
        strcpy(currentScope, "global");
        inFunctionScope = 0;
        
        printf("\nBuilding symbol table...\n");
        buildSymTabFromTree(savedTree);
        
        printf("\nSymbol Table Contents:\n");
        mostraTabelaSimbolos(symbolTable);

        printf("\nSyntax tree created successfully!\n");
        printTree(savedTree);
        
    } else {
        fprintf(stderr, "Empty syntax tree!\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    printf("\nCleaning up...\n");
    fflush(stdout);

    if(savedTree != NULL)
        freeTree(savedTree);
        
    if(symbolTable != NULL)
        deleteSymTab();

    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout) fclose(yyout);

    printf("Program completed successfully\n");
    fflush(stdout);

    fprintf(stderr, "Erros léxicos: %d\n", lexical_errors);
    fprintf(stderr, "Erros sintáticos: %d\n", syntax_errors);
    fprintf(stderr, "Erros semânticos: %d\n", semantic_errors);
    return 0;
}
