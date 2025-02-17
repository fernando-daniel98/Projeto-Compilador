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

extern int yylex(void);
void yyerror(const char *s);

static TreeNode *savedTree;

void debug_print(const char* msg) {
    printf("DEBUG: %s\n", msg);
    fflush(stdout);
}


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
  YYSYMBOL_SEMICOL = 22,                   /* SEMICOL  */
  YYSYMBOL_COMMA = 23,                     /* COMMA  */
  YYSYMBOL_LPAREN = 24,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 25,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 26,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 27,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 28,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 29,                    /* RBRACE  */
  YYSYMBOL_LOWER_THAN_ELSE = 30,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UMINUS = 31,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_programa = 33,                  /* programa  */
  YYSYMBOL_declaracao_lista = 34,          /* declaracao_lista  */
  YYSYMBOL_declaracao = 35,                /* declaracao  */
  YYSYMBOL_var_declaracao = 36,            /* var_declaracao  */
  YYSYMBOL_tipo_especificador = 37,        /* tipo_especificador  */
  YYSYMBOL_fun_declaracao = 38,            /* fun_declaracao  */
  YYSYMBOL_params = 39,                    /* params  */
  YYSYMBOL_param_lista = 40,               /* param_lista  */
  YYSYMBOL_param = 41,                     /* param  */
  YYSYMBOL_composto_decl = 42,             /* composto_decl  */
  YYSYMBOL_local_declaracoes = 43,         /* local_declaracoes  */
  YYSYMBOL_statement_lista = 44,           /* statement_lista  */
  YYSYMBOL_statement = 45,                 /* statement  */
  YYSYMBOL_expressao_decl = 46,            /* expressao_decl  */
  YYSYMBOL_selecao_decl = 47,              /* selecao_decl  */
  YYSYMBOL_iteracao_decl = 48,             /* iteracao_decl  */
  YYSYMBOL_retorno_decl = 49,              /* retorno_decl  */
  YYSYMBOL_expressao = 50,                 /* expressao  */
  YYSYMBOL_var = 51,                       /* var  */
  YYSYMBOL_simples_expressao = 52,         /* simples_expressao  */
  YYSYMBOL_relacional = 53,                /* relacional  */
  YYSYMBOL_soma_expressao = 54,            /* soma_expressao  */
  YYSYMBOL_soma = 55,                      /* soma  */
  YYSYMBOL_termo = 56,                     /* termo  */
  YYSYMBOL_mult = 57,                      /* mult  */
  YYSYMBOL_fator = 58,                     /* fator  */
  YYSYMBOL_ativacao = 59,                  /* ativacao  */
  YYSYMBOL_args = 60,                      /* args  */
  YYSYMBOL_arg_lista = 61                  /* arg_lista  */
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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  106

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    70,    70,    73,    86,    90,    91,    95,   105,   118,
     119,   123,   137,   138,   147,   157,   161,   176,   193,   204,
     214,   218,   228,   229,   238,   239,   240,   241,   242,   246,
     247,   251,   260,   272,   283,   289,   299,   307,   311,   312,
     324,   331,   335,   337,   339,   341,   343,   345,   349,   356,
     360,   362,   366,   373,   377,   379,   383,   384,   385,   386,
     387,   397,   409,   410,   414,   424
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
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUM", "IF",
  "ELSE", "WHILE", "RETURN", "PLUS", "MINUS", "MULT", "DIV", "SMAL",
  "SMALEQ", "GREAT", "GREATEQ", "EQ", "DIFF", "ASSIGN", "INT", "VOID",
  "SEMICOL", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "LBRACE",
  "RBRACE", "LOWER_THAN_ELSE", "UMINUS", "$accept", "programa",
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

#define YYPACT_NINF (-43)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-23)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      10,   -43,   -43,     7,    10,   -43,   -43,     8,   -43,   -43,
     -43,    71,   -43,    33,    18,    11,    37,    23,    28,   -43,
      32,    36,    41,    10,    43,    49,   -43,   -43,   -43,   -43,
     -43,     5,    58,   -43,    93,    42,   -43,   -21,    17,   -43,
      74,    75,    13,    64,   -43,    64,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,    78,    83,   -43,    69,    61,   -43,   -43,
      64,    64,    64,    64,   -43,    79,   -43,   -43,    80,   -43,
      64,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,    64,
      64,   -43,   -43,    64,   -43,    81,    84,    76,    85,    86,
     -43,   -43,   -43,    82,    61,   -43,   -43,    64,   -43,    53,
      53,   -43,    98,   -43,    53,   -43
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     9,    10,     0,     2,     4,     5,     0,     6,     1,
       3,     0,     7,     0,     0,    10,     0,     0,    12,    15,
       0,    16,     0,     0,     0,     0,    20,    11,    14,     8,
      17,     0,     0,    19,     0,     0,    23,     0,    38,    59,
       0,     0,     0,     0,    30,     0,    18,    25,    21,    24,
      26,    27,    28,     0,    57,    37,    41,    49,    53,    58,
      63,     0,     0,     0,    34,     0,    57,    60,     0,    29,
       0,    50,    51,    42,    43,    44,    45,    46,    47,     0,
       0,    54,    55,     0,    65,     0,    62,     0,     0,     0,
      35,    56,    36,    40,    48,    52,    61,     0,    39,     0,
       0,    64,    31,    33,     0,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -43,   -43,   -43,   104,    87,     1,   -43,   -43,   -43,    89,
      91,   -43,   -43,   -10,   -43,   -43,   -43,   -43,   -42,   -41,
     -43,   -43,    30,   -43,    34,   -43,   -39,   -43,   -43,   -43
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,     6,     7,     8,    17,    18,    19,
      47,    31,    35,    48,    49,    50,    51,    52,    53,    54,
      55,    79,    56,    80,    57,    83,    58,    59,    85,    86
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      65,    12,    66,    68,    67,    14,    32,     9,   -22,   -22,
     -22,    11,   -22,   -22,    16,   -22,    38,    39,    84,    87,
      88,    89,    20,    43,    16,     1,     2,   -22,    92,   -22,
       1,     2,    34,   -22,   -22,    64,   -13,    45,    66,    66,
      21,    60,    66,    61,    95,    38,    39,    40,    22,    41,
      42,    23,    43,     1,    15,   101,    38,    39,    40,    24,
      41,    42,    25,    43,    44,    29,    45,    38,    39,    26,
      26,    46,    81,    82,    43,    44,    30,    45,    71,    72,
      36,    26,    73,    74,    75,    76,    77,    78,    45,   102,
     103,    71,    72,    12,   105,    13,    37,    14,    62,    63,
      69,    90,    70,    98,   104,    91,    96,    97,    10,    93,
      99,   100,    28,    27,    94,     0,     0,     0,    33
};

static const yytype_int8 yycheck[] =
{
      42,    22,    43,    45,    43,    26,     1,     0,     3,     4,
       5,     3,     7,     8,    13,    10,     3,     4,    60,    61,
      62,    63,     4,    10,    23,    20,    21,    22,    70,    24,
      20,    21,    31,    28,    29,    22,    25,    24,    79,    80,
       3,    24,    83,    26,    83,     3,     4,     5,    25,     7,
       8,    23,    10,    20,    21,    97,     3,     4,     5,    27,
       7,     8,    26,    10,    22,    22,    24,     3,     4,    28,
      28,    29,    11,    12,    10,    22,    27,    24,     9,    10,
      22,    28,    13,    14,    15,    16,    17,    18,    24,    99,
     100,     9,    10,    22,   104,    24,     3,    26,    24,    24,
      22,    22,    19,    27,     6,    25,    25,    23,     4,    79,
      25,    25,    23,    22,    80,    -1,    -1,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    21,    33,    34,    35,    36,    37,    38,     0,
      35,     3,    22,    24,    26,    21,    37,    39,    40,    41,
       4,     3,    25,    23,    27,    26,    28,    42,    41,    22,
      27,    43,     1,    36,    37,    44,    22,     3,     3,     4,
       5,     7,     8,    10,    22,    24,    29,    42,    45,    46,
      47,    48,    49,    50,    51,    52,    54,    56,    58,    59,
      24,    26,    24,    24,    22,    50,    51,    58,    50,    22,
      19,     9,    10,    13,    14,    15,    16,    17,    18,    53,
      55,    11,    12,    57,    50,    60,    61,    50,    50,    50,
      22,    25,    50,    54,    56,    58,    25,    23,    27,    25,
      25,    50,    45,    45,     6,    45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    36,    36,    37,
      37,    38,    39,    39,    40,    40,    41,    41,    42,    43,
      43,    44,    44,    44,    45,    45,    45,    45,    45,    46,
      46,    47,    47,    48,    49,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    53,    53,    54,    54,
      55,    55,    56,    56,    57,    57,    58,    58,    58,    58,
      58,    59,    60,    60,    61,    61
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     6,     1,
       1,     6,     1,     1,     3,     1,     2,     4,     4,     2,
       0,     2,     0,     2,     1,     1,     1,     1,     1,     2,
       1,     5,     7,     5,     2,     3,     3,     1,     1,     4,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       2,     4,     1,     0,     3,     1
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
  case 2: /* programa: declaracao_lista  */
#line 70 "./parser/parser.y"
                           { savedTree = (yyvsp[0].node);}
#line 1216 "./build/parser.tab.c"
    break;

  case 3: /* declaracao_lista: declaracao_lista declaracao  */
#line 74 "./parser/parser.y"
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
#line 1233 "./build/parser.tab.c"
    break;

  case 4: /* declaracao_lista: declaracao  */
#line 86 "./parser/parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1239 "./build/parser.tab.c"
    break;

  case 5: /* declaracao: var_declaracao  */
#line 90 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node);}
#line 1245 "./build/parser.tab.c"
    break;

  case 6: /* declaracao: fun_declaracao  */
#line 91 "./parser/parser.y"
                     { (yyval.node) = (yyvsp[0].node);}
#line 1251 "./build/parser.tab.c"
    break;

  case 7: /* var_declaracao: tipo_especificador ID SEMICOL  */
#line 96 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VarDeclK;
        t->attr.name = (yyvsp[-1].node)->attr.name;  // Copy the ID name
        t->type = (yyvsp[-2].type);  // Cast back to ExpType
        free((yyvsp[-1].node));  // Free the ID node since we copied its data
        (yyval.node) = t;
    }
#line 1264 "./build/parser.tab.c"
    break;

  case 8: /* var_declaracao: tipo_especificador ID LBRACKET NUM RBRACKET SEMICOL  */
#line 106 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = VetDeclK;       // Vector declaration
        t->attr.name = (yyvsp[-4].node)->attr.name;  // Copy array name
        t->type = (yyvsp[-5].type);         // Cast back to ExpType
        t->child[0] = (yyvsp[-2].node);             // Store size of array (NUM node)
        free((yyvsp[-4].node));                      // Free the ID node
        (yyval.node) = t;
    }
#line 1278 "./build/parser.tab.c"
    break;

  case 9: /* tipo_especificador: INT  */
#line 118 "./parser/parser.y"
         { (yyval.type) = Integer; }
#line 1284 "./build/parser.tab.c"
    break;

  case 10: /* tipo_especificador: VOID  */
#line 119 "./parser/parser.y"
           { (yyval.type) = Void; }
#line 1290 "./build/parser.tab.c"
    break;

  case 11: /* fun_declaracao: tipo_especificador ID LPAREN params RPAREN composto_decl  */
#line 123 "./parser/parser.y"
                                                             {
        TreeNode* t = newNode(StatementK);

        t->kind.stmt = FunDeclK;
        t->attr.name = (yyvsp[-4].node)->attr.name;  // Copy the function name
        t->type = (yyvsp[-5].type);  // Store the return type (Integer or Void) from tipo_especificador
        t->child[0] = (yyvsp[-2].node);  // Store the parameters
        t->child[1] = (yyvsp[0].node);  // Store the compound statement
        free((yyvsp[-4].node));  // Free the ID node since we copied its data
        (yyval.node) = t;
    }
#line 1306 "./build/parser.tab.c"
    break;

  case 12: /* params: param_lista  */
#line 137 "./parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1312 "./build/parser.tab.c"
    break;

  case 13: /* params: VOID  */
#line 139 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ParamVoid;
        (yyval.node) = t;
    }
#line 1322 "./build/parser.tab.c"
    break;

  case 14: /* param_lista: param_lista COMMA param  */
#line 148 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1336 "./build/parser.tab.c"
    break;

  case 15: /* param_lista: param  */
#line 157 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1342 "./build/parser.tab.c"
    break;

  case 16: /* param: tipo_especificador ID  */
#line 162 "./parser/parser.y"
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
#line 1361 "./build/parser.tab.c"
    break;

  case 17: /* param: tipo_especificador ID LBRACKET RBRACKET  */
#line 177 "./parser/parser.y"
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
#line 1379 "./build/parser.tab.c"
    break;

  case 18: /* composto_decl: LBRACE local_declaracoes statement_lista RBRACE  */
#line 194 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = NuloDecl;
        t->child[0] = (yyvsp[-2].node);  // local declarations
        t->child[1] = (yyvsp[-1].node);  // statements
        (yyval.node) = t;
    }
#line 1391 "./build/parser.tab.c"
    break;

  case 19: /* local_declaracoes: local_declaracoes var_declaracao  */
#line 205 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1405 "./build/parser.tab.c"
    break;

  case 20: /* local_declaracoes: %empty  */
#line 214 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 1411 "./build/parser.tab.c"
    break;

  case 21: /* statement_lista: statement_lista statement  */
#line 219 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1425 "./build/parser.tab.c"
    break;

  case 22: /* statement_lista: %empty  */
#line 228 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 1431 "./build/parser.tab.c"
    break;

  case 23: /* statement_lista: error SEMICOL  */
#line 230 "./parser/parser.y"
        {
            yyerrok;
            printf("Erro recuperado. Continuando...\n");
            (yyval.node) = NULL;
        }
#line 1441 "./build/parser.tab.c"
    break;

  case 24: /* statement: expressao_decl  */
#line 238 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1447 "./build/parser.tab.c"
    break;

  case 25: /* statement: composto_decl  */
#line 239 "./parser/parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1453 "./build/parser.tab.c"
    break;

  case 26: /* statement: selecao_decl  */
#line 240 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1459 "./build/parser.tab.c"
    break;

  case 27: /* statement: iteracao_decl  */
#line 241 "./parser/parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1465 "./build/parser.tab.c"
    break;

  case 28: /* statement: retorno_decl  */
#line 242 "./parser/parser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1471 "./build/parser.tab.c"
    break;

  case 29: /* expressao_decl: expressao SEMICOL  */
#line 246 "./parser/parser.y"
                      { (yyval.node) = (yyvsp[-1].node); }
#line 1477 "./build/parser.tab.c"
    break;

  case 30: /* expressao_decl: SEMICOL  */
#line 247 "./parser/parser.y"
              { (yyval.node) = NULL; }
#line 1483 "./build/parser.tab.c"
    break;

  case 31: /* selecao_decl: IF LPAREN expressao RPAREN statement  */
#line 252 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = (yyvsp[-2].node);  /* condition */
        t->child[1] = (yyvsp[0].node);  /* if-part */
        t->child[2] = NULL; /* else-part is null */
        (yyval.node) = t;
    }
#line 1496 "./build/parser.tab.c"
    break;

  case 32: /* selecao_decl: IF LPAREN expressao RPAREN statement ELSE statement  */
#line 261 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = IfK;
        t->child[0] = (yyvsp[-4].node);  /* condition */
        t->child[1] = (yyvsp[-2].node);  /* if-part */
        t->child[2] = (yyvsp[0].node);  /* else-part */
        (yyval.node) = t;
    }
#line 1509 "./build/parser.tab.c"
    break;

  case 33: /* iteracao_decl: WHILE LPAREN expressao RPAREN statement  */
#line 273 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = WhileK;
        t->child[0] = (yyvsp[-2].node);  /* condition */
        t->child[1] = (yyvsp[0].node);  /* body */
        (yyval.node) = t;
    }
#line 1521 "./build/parser.tab.c"
    break;

  case 34: /* retorno_decl: RETURN SEMICOL  */
#line 284 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnVOID;
        (yyval.node) = t;
    }
#line 1531 "./build/parser.tab.c"
    break;

  case 35: /* retorno_decl: RETURN expressao SEMICOL  */
#line 290 "./parser/parser.y"
    {
        TreeNode* t = newNode(StatementK);
        t->kind.stmt = ReturnINT;
        t->child[0] = (yyvsp[-1].node);  /* return value */
        (yyval.node) = t;
    }
#line 1542 "./build/parser.tab.c"
    break;

  case 36: /* expressao: var ASSIGN expressao  */
#line 300 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AssignK;
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1554 "./build/parser.tab.c"
    break;

  case 37: /* expressao: simples_expressao  */
#line 307 "./parser/parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1560 "./build/parser.tab.c"
    break;

  case 38: /* var: ID  */
#line 311 "./parser/parser.y"
       { (yyval.node) = (yyvsp[0].node); }
#line 1566 "./build/parser.tab.c"
    break;

  case 39: /* var: ID LBRACKET expressao RBRACKET  */
#line 313 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = VetorK;
        t->attr.name = (yyvsp[-3].node)->attr.name;
        t->child[0] = (yyvsp[-1].node);  // index expression
        free((yyvsp[-3].node));
        (yyval.node) = t;
    }
#line 1579 "./build/parser.tab.c"
    break;

  case 40: /* simples_expressao: soma_expressao relacional soma_expressao  */
#line 325 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);  // reuse relational operator node
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1590 "./build/parser.tab.c"
    break;

  case 41: /* simples_expressao: soma_expressao  */
#line 331 "./parser/parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1596 "./build/parser.tab.c"
    break;

  case 42: /* relacional: SMAL  */
#line 335 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1602 "./build/parser.tab.c"
    break;

  case 43: /* relacional: SMALEQ  */
#line 337 "./parser/parser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1608 "./build/parser.tab.c"
    break;

  case 44: /* relacional: GREAT  */
#line 339 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1614 "./build/parser.tab.c"
    break;

  case 45: /* relacional: GREATEQ  */
#line 341 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1620 "./build/parser.tab.c"
    break;

  case 46: /* relacional: EQ  */
#line 343 "./parser/parser.y"
       { (yyval.node) = (yyvsp[0].node); }
#line 1626 "./build/parser.tab.c"
    break;

  case 47: /* relacional: DIFF  */
#line 345 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1632 "./build/parser.tab.c"
    break;

  case 48: /* soma_expressao: soma_expressao soma termo  */
#line 350 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);  // reuse operator node
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1643 "./build/parser.tab.c"
    break;

  case 49: /* soma_expressao: termo  */
#line 356 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1649 "./build/parser.tab.c"
    break;

  case 50: /* soma: PLUS  */
#line 360 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1655 "./build/parser.tab.c"
    break;

  case 51: /* soma: MINUS  */
#line 362 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1661 "./build/parser.tab.c"
    break;

  case 52: /* termo: termo mult fator  */
#line 367 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-1].node);
        t->child[0] = (yyvsp[-2].node);
        t->child[1] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1672 "./build/parser.tab.c"
    break;

  case 53: /* termo: fator  */
#line 373 "./parser/parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1678 "./build/parser.tab.c"
    break;

  case 54: /* mult: MULT  */
#line 377 "./parser/parser.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1684 "./build/parser.tab.c"
    break;

  case 55: /* mult: DIV  */
#line 379 "./parser/parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1690 "./build/parser.tab.c"
    break;

  case 56: /* fator: LPAREN expressao RPAREN  */
#line 383 "./parser/parser.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1696 "./build/parser.tab.c"
    break;

  case 57: /* fator: var  */
#line 384 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1702 "./build/parser.tab.c"
    break;

  case 58: /* fator: ativacao  */
#line 385 "./parser/parser.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1708 "./build/parser.tab.c"
    break;

  case 59: /* fator: NUM  */
#line 386 "./parser/parser.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1714 "./build/parser.tab.c"
    break;

  case 60: /* fator: MINUS fator  */
#line 388 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = OpK;
        t->child[0] = (yyvsp[0].node);
        (yyval.node) = t;
    }
#line 1725 "./build/parser.tab.c"
    break;

  case 61: /* ativacao: ID LPAREN args RPAREN  */
#line 398 "./parser/parser.y"
    {
        TreeNode* t = newNode(ExpressionK);
        t->kind.exp = AtivK;
        t->attr.name = (yyvsp[-3].node)->attr.name;
        t->child[0] = (yyvsp[-1].node);  // arguments
        free((yyvsp[-3].node));
        (yyval.node) = t;
    }
#line 1738 "./build/parser.tab.c"
    break;

  case 62: /* args: arg_lista  */
#line 409 "./parser/parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1744 "./build/parser.tab.c"
    break;

  case 63: /* args: %empty  */
#line 410 "./parser/parser.y"
                  { (yyval.node) = NULL; }
#line 1750 "./build/parser.tab.c"
    break;

  case 64: /* arg_lista: arg_lista COMMA expressao  */
#line 415 "./parser/parser.y"
    {
        TreeNode* t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL) t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
        else (yyval.node) = (yyvsp[0].node);
    }
#line 1764 "./build/parser.tab.c"
    break;

  case 65: /* arg_lista: expressao  */
#line 424 "./parser/parser.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1770 "./build/parser.tab.c"
    break;


#line 1774 "./build/parser.tab.c"

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

#line 427 "./parser/parser.y"


void yyerror(const char *s) {
    printf( "ERRO SINTÁTICO: \"%s\" ", yytext);
    printf( "LINHA: %d\n" , lineNum);
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

    if (parseResult != 0) {
        fprintf(stderr, "Parsing failed!\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    printf("Checking syntax tree...\n");
    fflush(stdout);

    if (savedTree != NULL) {        
        printf("\nBuilding symbol table...\n");
        buildSymTabFromTree(savedTree);
        
        printf("\nSymbol Table Contents:\n");
        mostraTabelaSimbolos(symbolTable);
        
        deleteSymTab();

        printf("\nSyntax tree created successfully!\n");
        printTree(savedTree);
        
        /* freeTree(savedTree); */
    } else {
        fprintf(stderr, "Empty syntax tree!\n");
        if (yyin != stdin) fclose(yyin);
        if (yyout != stdout) fclose(yyout);
        return 1;
    }

    printf("Cleaning up...\n");
    fflush(stdout);

    if (yyin != stdin) fclose(yyin);
    if (yyout != stdout) fclose(yyout);

    printf("Program completed successfully\n");
    fflush(stdout);
    return 0;
}
