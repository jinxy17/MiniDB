/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "../queryparser/yacc.y"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "../queryparser/Stmt.h"
#include "../queryparser/Executer.h"
#include "../sysmgr/SysManager.h"
#include "../queryparser/DataOperater.h"
int yylex();
void yyerror(const char *);
class Stmt;
Executer * executer;

#line 86 "y.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KWNULL = 258,
    KWINT = 259,
    KWFLOAT = 260,
    KWDATE = 261,
    QUIT = 262,
    DATABASE = 263,
    DATABASES = 264,
    TABLE = 265,
    TABLES = 266,
    SHOW = 267,
    CREATE = 268,
    DROP = 269,
    USE = 270,
    PRIMARY = 271,
    KEY = 272,
    NOT = 273,
    INSERT = 274,
    INTO = 275,
    VALUES = 276,
    DELETE = 277,
    FROM = 278,
    WHERE = 279,
    UPDATE = 280,
    SET = 281,
    SELECT = 282,
    IS = 283,
    VARCHAR = 284,
    DEFAULT = 285,
    CONSTRAINT = 286,
    CHANGE = 287,
    ALTER = 288,
    ADD = 289,
    RENAME = 290,
    DESC = 291,
    INDEX = 292,
    AND = 293,
    FOREIGN = 294,
    REFERENCES = 295,
    ON = 296,
    TO = 297,
    EQ = 298,
    GT = 299,
    LT = 300,
    GE = 301,
    LE = 302,
    NE = 303,
    VALUE_INT = 304,
    VALUE_FLOAT = 305,
    VALUE_STRING = 306,
    IDENTIFIER = 307
  };
#endif
/* Tokens.  */
#define KWNULL 258
#define KWINT 259
#define KWFLOAT 260
#define KWDATE 261
#define QUIT 262
#define DATABASE 263
#define DATABASES 264
#define TABLE 265
#define TABLES 266
#define SHOW 267
#define CREATE 268
#define DROP 269
#define USE 270
#define PRIMARY 271
#define KEY 272
#define NOT 273
#define INSERT 274
#define INTO 275
#define VALUES 276
#define DELETE 277
#define FROM 278
#define WHERE 279
#define UPDATE 280
#define SET 281
#define SELECT 282
#define IS 283
#define VARCHAR 284
#define DEFAULT 285
#define CONSTRAINT 286
#define CHANGE 287
#define ALTER 288
#define ADD 289
#define RENAME 290
#define DESC 291
#define INDEX 292
#define AND 293
#define FOREIGN 294
#define REFERENCES 295
#define ON 296
#define TO 297
#define EQ 298
#define GT 299
#define LT 300
#define GE 301
#define LE 302
#define NE 303
#define VALUE_INT 304
#define VALUE_FLOAT 305
#define VALUE_STRING 306
#define IDENTIFIER 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "../queryparser/yacc.y"

    int ivalue;
    float fvalue;
    char * string;
    Stmt * ystmt;
    AttrInfo * attrinfo;
    DataType * datatype;
    Value* valuetype;
    Lists* infolist;
    Tcol* tcol;
    Relation* relation;
    CompOp compop;
    Assigns* assigns;

#line 257 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
typedef yytype_uint8 yy_state_t;

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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  207

#define YYUNDEFTOK  2
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      54,    55,    58,     2,    56,     2,    57,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    53,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    68,    68,    72,    79,    83,    84,    85,    86,    87,
      91,   100,   108,   116,   124,   133,   158,   165,   172,   181,
     190,   202,   219,   229,   236,   246,   257,   266,   274,   284,
     292,   299,   309,   317,   330,   341,   347,   354,   365,   376,
     388,   400,   406,   416,   420,   425,   430,   437,   442,   448,
     454,   462,   468,   476,   483,   490,   495,   502,   512,   522,
     532,   544,   549,   557,   561,   567,   568,   569,   570,   571,
     572,   575,   586,   600,   605,   610,   618,   623,   631,   636,
     643,   644,   645,   646,   647,   648
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KWNULL", "KWINT", "KWFLOAT", "KWDATE",
  "QUIT", "DATABASE", "DATABASES", "TABLE", "TABLES", "SHOW", "CREATE",
  "DROP", "USE", "PRIMARY", "KEY", "NOT", "INSERT", "INTO", "VALUES",
  "DELETE", "FROM", "WHERE", "UPDATE", "SET", "SELECT", "IS", "VARCHAR",
  "DEFAULT", "CONSTRAINT", "CHANGE", "ALTER", "ADD", "RENAME", "DESC",
  "INDEX", "AND", "FOREIGN", "REFERENCES", "ON", "TO", "EQ", "GT", "LT",
  "GE", "LE", "NE", "VALUE_INT", "VALUE_FLOAT", "VALUE_STRING",
  "IDENTIFIER", "';'", "'('", "')'", "','", "'.'", "'*'", "$accept",
  "program", "stmt", "sysStmt", "dbStmt", "tbStmt", "idxStmt", "alterStmt",
  "fieldList", "field", "type", "type_width", "valueLists", "valueList",
  "value", "whereClause", "whereClauses", "col", "op", "setClause",
  "selector", "tableList", "columnList", "dbName", "tbName", "colName",
  "idxName", "fkName", "pkName", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,    59,    40,    41,    44,    46,    42
};
# endif

#define YYPACT_NINF (-142)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-86)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     121,   -52,    25,    74,    89,   -35,    23,    12,    40,   -38,
      37,    40,    76,  -142,    -3,    47,    52,    55,    57,  -142,
    -142,  -142,   -35,    40,    61,   -35,    40,    61,  -142,  -142,
      40,    40,  -142,    42,    58,  -142,  -142,   -15,    70,  -142,
      40,  -142,  -142,  -142,  -142,  -142,  -142,  -142,  -142,  -142,
      68,  -142,    91,  -142,  -142,  -142,   109,   115,    85,    40,
      92,    85,   131,   -10,    40,    87,    92,  -142,   -11,   104,
       6,  -142,  -142,  -142,    77,    85,    15,   114,   134,   147,
       9,  -142,   113,   125,    30,   112,  -142,   138,   127,    92,
      85,    30,    92,    40,   152,    61,   163,  -142,   -10,   129,
      61,  -142,    40,   128,   132,  -142,   -10,   133,  -142,  -142,
     133,    10,    85,  -142,  -142,  -142,  -142,    16,  -142,   135,
      92,     7,  -142,  -142,  -142,  -142,  -142,  -142,     8,   138,
     140,  -142,   138,  -142,   136,  -142,   139,  -142,   169,   151,
     177,   141,  -142,    85,    85,  -142,   145,  -142,  -142,   193,
      30,    31,  -142,  -142,    30,    30,  -142,  -142,   194,  -142,
    -142,    30,  -142,  -142,  -142,  -142,   181,   182,    85,    65,
     146,   148,   170,  -142,  -142,    85,  -142,    69,  -142,  -142,
     150,   153,    94,  -142,   162,  -142,    30,  -142,  -142,    85,
      85,  -142,    40,  -142,   103,   105,   154,   165,  -142,    85,
      40,   155,   157,  -142,    85,   122,  -142
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     4,
      10,    14,     0,     0,     0,     0,     0,     0,    80,    13,
       0,     0,    81,     0,    82,    73,    74,     0,     0,    63,
       0,    17,     1,     3,     5,     6,     7,     8,     9,    11,
       0,    83,     0,    12,    16,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,    76,    75,    64,     0,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    18,    61,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    26,     0,     0,     0,    15,     0,    48,    46,    45,
      48,    37,     0,    56,    53,    54,    55,     0,    51,     0,
       0,     0,    65,    66,    67,    68,    69,    70,     0,    20,
       0,    71,    21,    77,    30,    25,     0,    28,    84,     0,
       0,     0,    29,     0,     0,    36,     0,    43,    44,     0,
       0,     0,    78,    49,     0,     0,    62,    60,     0,    57,
      58,     0,    85,    32,    84,    34,     0,     0,     0,     0,
       0,     0,    38,    39,    22,     0,    52,     0,    59,    72,
       0,     0,     0,    41,     0,    47,     0,    79,    50,     0,
       0,    24,     0,    40,     0,     0,     0,     0,    31,     0,
       0,     0,     0,    42,     0,     0,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -142,  -142,   197,  -142,  -142,  -142,  -142,  -142,  -142,    -2,
    -142,    96,  -142,    59,   -84,    93,   -80,    -5,  -142,  -142,
    -142,  -142,  -141,    -6,    -8,   -37,   -22,    79,    78
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,    80,    81,
     111,   147,    85,   117,   118,    86,    87,    88,   128,    68,
      37,    70,   151,    29,    38,    39,    52,   139,   140
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,    19,   169,    41,    36,    55,    78,   131,    59,   129,
     157,   113,   132,    89,    34,    50,    49,    28,    54,    53,
      35,    69,    56,    57,    73,   158,    82,   182,   149,    79,
      92,    78,    62,   113,    20,    31,    21,    97,    98,    82,
     150,    60,    67,    30,   159,    90,    99,    40,   194,   195,
      44,    71,   100,   130,    79,    72,    83,   114,   115,   116,
      34,    82,    93,   205,   105,   106,   173,    67,    58,    82,
     176,   153,   154,   135,   101,   152,    42,   179,   141,   114,
     115,   116,    22,     1,    23,   133,   174,   175,     2,     3,
       4,     5,    32,    94,   142,     6,   137,    25,     7,    26,
      45,     8,   193,     9,   145,    46,   152,   170,    47,    10,
      48,    24,    11,    51,    95,   -81,    96,   107,   108,   109,
     183,   175,    63,   160,   188,   154,    27,    61,     1,    67,
      65,   152,    64,     2,     3,     4,     5,    67,   187,    66,
       6,    84,   110,     7,    34,    74,     8,    91,     9,   191,
     175,   103,   152,   152,    10,   121,   102,    11,   197,   175,
     198,   175,   201,    75,   104,    76,    77,   152,   119,   134,
     122,   123,   124,   125,   126,   127,   120,   206,   175,   112,
     136,   138,   143,   161,   196,   -85,   144,   146,   162,   155,
     166,   164,   202,   167,   171,   168,   172,   178,   180,   181,
     186,   184,   192,   185,   189,   200,   148,   190,   199,    43,
     203,   204,   163,   156,   177,   165
};

static const yytype_uint8 yycheck[] =
{
       8,    53,   143,    11,     9,    27,    16,    91,    23,    89,
       3,     3,    92,    24,    52,    23,    22,    52,    26,    25,
      58,    58,    30,    31,    61,    18,    63,   168,    18,    39,
      24,    16,    40,     3,     9,    23,    11,    74,    75,    76,
      30,    56,    52,    20,   128,    56,    31,    10,   189,   190,
      53,    59,    37,    90,    39,    60,    64,    49,    50,    51,
      52,    98,    56,   204,    55,    56,   150,    52,    26,   106,
     154,    55,    56,    95,    76,   112,     0,   161,   100,    49,
      50,    51,     8,     7,    10,    93,    55,    56,    12,    13,
      14,    15,    52,    16,   102,    19,    98,     8,    22,    10,
      53,    25,   186,    27,   106,    53,   143,   144,    53,    33,
      53,    37,    36,    52,    37,    57,    39,     4,     5,     6,
      55,    56,    54,   128,    55,    56,    37,    57,     7,    52,
      21,   168,    41,    12,    13,    14,    15,    52,   175,    24,
      19,    54,    29,    22,    52,    14,    25,    43,    27,    55,
      56,    17,   189,   190,    33,    28,    42,    36,    55,    56,
      55,    56,   199,    32,    17,    34,    35,   204,    56,    17,
      43,    44,    45,    46,    47,    48,    38,    55,    56,    54,
      17,    52,    54,    43,   192,    16,    54,    54,    52,    54,
      39,    52,   200,    16,    49,    54,     3,     3,    17,    17,
      30,    55,    40,    55,    54,    40,   110,    54,    54,    12,
      55,    54,   134,   120,   155,   136
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    12,    13,    14,    15,    19,    22,    25,    27,
      33,    36,    60,    61,    62,    63,    64,    65,    66,    53,
       9,    11,     8,    10,    37,     8,    10,    37,    52,    82,
      20,    23,    52,    83,    52,    58,    76,    79,    83,    84,
      10,    83,     0,    61,    53,    53,    53,    53,    53,    82,
      83,    52,    85,    82,    83,    85,    83,    83,    26,    23,
      56,    57,    83,    54,    41,    21,    24,    52,    78,    84,
      80,    83,    76,    84,    14,    32,    34,    35,    16,    39,
      67,    68,    84,    83,    54,    71,    74,    75,    76,    24,
      56,    43,    24,    56,    16,    37,    39,    84,    84,    31,
      37,    68,    42,    17,    17,    55,    56,     4,     5,     6,
      29,    69,    54,     3,    49,    50,    51,    72,    73,    56,
      38,    28,    43,    44,    45,    46,    47,    48,    77,    75,
      84,    73,    75,    83,    17,    85,    17,    68,    52,    86,
      87,    85,    83,    54,    54,    68,    54,    70,    70,    18,
      30,    81,    84,    55,    56,    54,    74,     3,    18,    73,
      76,    43,    52,    87,    52,    86,    39,    16,    54,    81,
      84,    49,     3,    73,    55,    56,    73,    72,     3,    73,
      17,    17,    81,    55,    55,    55,    30,    84,    55,    54,
      54,    55,    40,    73,    81,    81,    83,    55,    55,    54,
      40,    84,    83,    55,    54,    81,    55
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    61,    61,    61,    61,
      62,    63,    63,    63,    63,    64,    64,    64,    64,    64,
      64,    64,    65,    65,    65,    65,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    67,    67,    68,    68,    68,
      68,    68,    68,    69,    69,    69,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    73,    73,    74,    74,    74,
      74,    75,    75,    76,    76,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    79,    79,    80,    80,    81,    81,
      82,    83,    84,    85,    86,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     2,     6,     3,     2,     5,     5,
       6,     6,     8,     3,     9,     6,     5,     5,     6,     6,
       6,    11,     7,    16,     7,     1,     3,     2,     4,     4,
       6,     5,    10,     2,     2,     1,     1,     3,     0,     3,
       5,     1,     3,     1,     1,     1,     1,     3,     3,     4,
       3,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     3,     5,     1,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
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
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
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
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
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
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 2:
#line 69 "../queryparser/yacc.y"
       {

       }
#line 1572 "y.tab.c"
    break;

  case 3:
#line 73 "../queryparser/yacc.y"
       {

       }
#line 1580 "y.tab.c"
    break;

  case 4:
#line 80 "../queryparser/yacc.y"
       {
         YYACCEPT;
       }
#line 1588 "y.tab.c"
    break;

  case 5:
#line 83 "../queryparser/yacc.y"
                     { }
#line 1594 "y.tab.c"
    break;

  case 6:
#line 84 "../queryparser/yacc.y"
                     { }
#line 1600 "y.tab.c"
    break;

  case 7:
#line 85 "../queryparser/yacc.y"
                     { }
#line 1606 "y.tab.c"
    break;

  case 8:
#line 86 "../queryparser/yacc.y"
                     { }
#line 1612 "y.tab.c"
    break;

  case 9:
#line 87 "../queryparser/yacc.y"
                     { }
#line 1618 "y.tab.c"
    break;

  case 10:
#line 92 "../queryparser/yacc.y"
       {
          sysStmt* stmt = new sysStmt();
          executer->execSysStmt(stmt);
          delete stmt;
       }
#line 1628 "y.tab.c"
    break;

  case 11:
#line 101 "../queryparser/yacc.y"
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_CREATE);
          stmt->dbName = (yyvsp[0].string);
          executer->execDbStmt(stmt);
          delete (yyvsp[0].string);
          delete stmt;
       }
#line 1640 "y.tab.c"
    break;

  case 12:
#line 109 "../queryparser/yacc.y"
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_DROP);
          stmt->dbName = (yyvsp[0].string);
          executer->execDbStmt(stmt);
          delete (yyvsp[0].string);
          delete stmt;
       }
#line 1652 "y.tab.c"
    break;

  case 13:
#line 117 "../queryparser/yacc.y"
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_USE);
          stmt->dbName = (yyvsp[0].string);
          executer->execDbStmt(stmt);
          delete (yyvsp[0].string);
          delete stmt;
       }
#line 1664 "y.tab.c"
    break;

  case 14:
#line 125 "../queryparser/yacc.y"
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_SHOW);
          executer->execDbStmt(stmt);
          delete stmt;
       }
#line 1674 "y.tab.c"
    break;

  case 15:
#line 134 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_CREATE);
          stmt->tbName = (yyvsp[-3].string);

          stmt->tableInfo = new TableInfo();
			 stmt->tableInfo->tableName = stmt->tbName;
          stmt->tableInfo->recordSize = 0;
          stmt->tableInfo->attrs.clear();
          stmt->pk = nullptr;
          stmt->fks.clear();
          //TODO: 添加对PK和FK的处理
          for(int i = 0;i < (yyvsp[-1].infolist)->attrinfos.size();i++) {
             if((yyvsp[-1].infolist)->attrinfos[i]->createPk == false && (yyvsp[-1].infolist)->attrinfos[i]->createFk == false)
               stmt->tableInfo->attrs.push_back(*(yyvsp[-1].infolist)->attrinfos[i]);
             else if((yyvsp[-1].infolist)->attrinfos[i]->createPk == true)
               stmt->pk = (yyvsp[-1].infolist)->attrinfos[i];
             else
               stmt->fks.push_back((yyvsp[-1].infolist)->attrinfos[i]);
          }
          stmt->tableInfo->attrNum = stmt->tableInfo->attrs.size();
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1703 "y.tab.c"
    break;

  case 16:
#line 159 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DROP);
          stmt->tbName = (yyvsp[0].string);
          executer->execTbStmt(stmt);
          delete stmt;
       }
#line 1714 "y.tab.c"
    break;

  case 17:
#line 166 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DESC);
          stmt->tbName = (yyvsp[0].string);
          executer->execTbStmt(stmt);
          delete stmt;
       }
#line 1725 "y.tab.c"
    break;

  case 18:
#line 173 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_INSERT);
          stmt->tbName = (yyvsp[-2].string);
          stmt->datas = (yyvsp[0].infolist)->valuelists;
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[0].infolist);
       }
#line 1738 "y.tab.c"
    break;

  case 19:
#line 182 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DELETE);
          stmt->tbName = (yyvsp[-2].string);
          stmt->relations.assign((yyvsp[0].infolist)->relations.begin(),(yyvsp[0].infolist)->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[0].infolist);
       }
#line 1751 "y.tab.c"
    break;

  case 20:
#line 191 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_UPDATE);
          stmt->tbName = (yyvsp[-4].string);
          stmt->assigns = (yyvsp[-2].assigns);
          stmt->assigns->table = (yyvsp[-4].string);
          stmt->relations.assign((yyvsp[0].infolist)->relations.begin(),(yyvsp[0].infolist)->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[-2].assigns);
          delete (yyvsp[0].infolist);
       }
#line 1767 "y.tab.c"
    break;

  case 21:
#line 203 "../queryparser/yacc.y"
       {
          // TODO:三张以上表的连接
          tbStmt* stmt = new tbStmt(tbStmt::TB_SELECT);
          //printf("select: cols num:%ld,tables num:%ld,whereclauses num:%ld\n",$2->collist.size(),$4->namelist.size(),$6->relations.size());
          stmt->collist.assign((yyvsp[-4].infolist)->collist.begin(),(yyvsp[-4].infolist)->collist.end());
          stmt->tablelist.assign((yyvsp[-2].infolist)->namelist.begin(),(yyvsp[-2].infolist)->namelist.end());
          stmt->relations.assign((yyvsp[0].infolist)->relations.begin(),(yyvsp[0].infolist)->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[-4].infolist);
          delete (yyvsp[-2].infolist);
          delete (yyvsp[0].infolist);
       }
#line 1785 "y.tab.c"
    break;

  case 22:
#line 220 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_CREATE);
          stmt->tbName = (yyvsp[-3].string);
          stmt->idxName = (yyvsp[-5].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1799 "y.tab.c"
    break;

  case 23:
#line 230 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_DROP);
          executer->execIdxStmt(stmt);
          stmt->idxName = (yyvsp[0].string);
          delete stmt;
       }
#line 1810 "y.tab.c"
    break;

  case 24:
#line 237 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_ADD);
          stmt->tbName = (yyvsp[-6].string);
          stmt->idxName = (yyvsp[-3].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1824 "y.tab.c"
    break;

  case 25:
#line 247 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_DROP);
          stmt->tbName = (yyvsp[-3].string);
          stmt->idxName = (yyvsp[0].string);
          executer->execIdxStmt(stmt);
          delete stmt;
       }
#line 1836 "y.tab.c"
    break;

  case 26:
#line 258 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD);
          stmt->tbName = (yyvsp[-2].string);
          stmt->field = (yyvsp[0].attrinfo);
          executer->execAlterStmt(stmt);
         //  delete stmt;
         //  delete $5;
       }
#line 1849 "y.tab.c"
    break;

  case 27:
#line 267 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP);
          stmt->tbName = (yyvsp[-2].string);
          stmt->colName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1861 "y.tab.c"
    break;

  case 28:
#line 275 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_CHANGE);
          stmt->tbName = (yyvsp[-3].string);
          stmt->colName = (yyvsp[-1].string);
          stmt->field = (yyvsp[0].attrinfo);
          executer->execAlterStmt(stmt);
          delete stmt;
          delete (yyvsp[0].attrinfo);
       }
#line 1875 "y.tab.c"
    break;

  case 29:
#line 285 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_RENAME);
          stmt->tbName = (yyvsp[-3].string);
          stmt->newtbName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1887 "y.tab.c"
    break;

  case 30:
#line 293 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY);
          stmt->tbName = (yyvsp[-3].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1898 "y.tab.c"
    break;

  case 31:
#line 300 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD_PRIMARY);
          stmt->tbName = (yyvsp[-8].string);
          stmt->pkName = (yyvsp[-5].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execAlterStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1912 "y.tab.c"
    break;

  case 32:
#line 310 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY_WNAME);
          stmt->tbName = (yyvsp[-4].string);
          stmt->pkName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1924 "y.tab.c"
    break;

  case 33:
#line 318 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD_FOREIGN);
          stmt->tbName = (yyvsp[-13].string);
          stmt->fkName = (yyvsp[-10].string);
          stmt->collist.assign((yyvsp[-6].infolist)->namelist.begin(),(yyvsp[-6].infolist)->namelist.end());
          stmt->fktbName = (yyvsp[-3].string);
          stmt->fkcollist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execAlterStmt(stmt);
          delete stmt;
          delete (yyvsp[-6].infolist);
          delete (yyvsp[-1].infolist);
       }
#line 1941 "y.tab.c"
    break;

  case 34:
#line 331 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_FOREIGN_WNAME);
          stmt->tbName = (yyvsp[-4].string);
          stmt->fkName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1953 "y.tab.c"
    break;

  case 35:
#line 342 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->attrinfos.clear();
          (yyval.infolist)->attrinfos.push_back((yyvsp[0].attrinfo));
       }
#line 1963 "y.tab.c"
    break;

  case 36:
#line 348 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->attrinfos.push_back((yyvsp[0].attrinfo));
       }
#line 1972 "y.tab.c"
    break;

  case 37:
#line 355 "../queryparser/yacc.y"
       {
          (yyval.attrinfo) = new AttrInfo();
          (yyval.attrinfo)->attrName = (yyvsp[-1].string);
	       (yyval.attrinfo)->attrType = (yyvsp[0].datatype)->dtype;
          (yyval.attrinfo)->attrLength = (yyvsp[0].datatype)->setlength;
	       (yyval.attrinfo)->notNull = false;
	       (yyval.attrinfo)->primary = false;
	       (yyval.attrinfo)->haveIndex = false;
          delete (yyvsp[0].datatype);
       }
#line 1987 "y.tab.c"
    break;

  case 38:
#line 366 "../queryparser/yacc.y"
       {
          (yyval.attrinfo) = new AttrInfo();
          (yyval.attrinfo)->attrName = (yyvsp[-3].string);
	       (yyval.attrinfo)->attrType = (yyvsp[-2].datatype)->dtype;
          (yyval.attrinfo)->attrLength = (yyvsp[-2].datatype)->setlength;
	       (yyval.attrinfo)->notNull = true;
	       (yyval.attrinfo)->primary = false;
	       (yyval.attrinfo)->haveIndex = false;
          delete (yyvsp[-2].datatype);
       }
#line 2002 "y.tab.c"
    break;

  case 39:
#line 377 "../queryparser/yacc.y"
       {
          (yyval.attrinfo) = new AttrInfo();
          (yyval.attrinfo)->attrName = (yyvsp[-3].string);
	       (yyval.attrinfo)->attrType = (yyvsp[-2].datatype)->dtype;
          (yyval.attrinfo)->attrLength = (yyvsp[-2].datatype)->setlength;
	       (yyval.attrinfo)->notNull = false;
	       (yyval.attrinfo)->primary = false;
	       (yyval.attrinfo)->haveIndex = false;
          //TODO：sys模块需要添加对int转float,date转int的处理
          (yyval.attrinfo)->defaultValue = (yyvsp[0].valuetype)->data;
       }
#line 2018 "y.tab.c"
    break;

  case 40:
#line 389 "../queryparser/yacc.y"
       {
          (yyval.attrinfo) = new AttrInfo();
          (yyval.attrinfo)->attrName = (yyvsp[-5].string);
	       (yyval.attrinfo)->attrType = (yyvsp[-4].datatype)->dtype;
          (yyval.attrinfo)->attrLength = (yyvsp[-4].datatype)->setlength;
	       (yyval.attrinfo)->notNull = true;
	       (yyval.attrinfo)->primary = false;
	       (yyval.attrinfo)->haveIndex = false;
          //TODO：sys模块需要添加对int转float,date转int的处理
          (yyval.attrinfo)->defaultValue = (yyvsp[0].valuetype)->data;
       }
#line 2034 "y.tab.c"
    break;

  case 41:
#line 401 "../queryparser/yacc.y"
       {
         (yyval.attrinfo) = new AttrInfo();
         (yyval.attrinfo)->namelist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
         (yyval.attrinfo)->createPk = true;
       }
#line 2044 "y.tab.c"
    break;

  case 42:
#line 407 "../queryparser/yacc.y"
       {
         (yyval.attrinfo) = new AttrInfo();
         (yyval.attrinfo)->attrName = (yyvsp[-6].string);
	      (yyval.attrinfo)->reference = (yyvsp[-3].string);
         (yyval.attrinfo)->foreignKeyName = (yyvsp[-1].string); 
       }
#line 2055 "y.tab.c"
    break;

  case 43:
#line 417 "../queryparser/yacc.y"
       {
         (yyval.datatype) = new DataType(INT,(yyvsp[0].ivalue));
       }
#line 2063 "y.tab.c"
    break;

  case 44:
#line 421 "../queryparser/yacc.y"
       {
          printf("type_width:%d\n",(yyvsp[0].ivalue));
          (yyval.datatype) = new DataType(STRING,(yyvsp[0].ivalue));
       }
#line 2072 "y.tab.c"
    break;

  case 45:
#line 426 "../queryparser/yacc.y"
       {
          //DATE直接用String表示似乎更方便快捷
          (yyval.datatype) = new DataType(STRING,12);
       }
#line 2081 "y.tab.c"
    break;

  case 46:
#line 431 "../queryparser/yacc.y"
       {
          (yyval.datatype) = new DataType(FLOAT,8);
       }
#line 2089 "y.tab.c"
    break;

  case 47:
#line 438 "../queryparser/yacc.y"
       {
          (yyval.ivalue) = (yyvsp[-1].ivalue);
       }
#line 2097 "y.tab.c"
    break;

  case 48:
#line 442 "../queryparser/yacc.y"
       {
          (yyval.ivalue) = 0;
       }
#line 2105 "y.tab.c"
    break;

  case 49:
#line 449 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->valuelists.clear();
          (yyval.infolist)->valuelists.push_back((yyvsp[-1].infolist)->valuelist);
       }
#line 2115 "y.tab.c"
    break;

  case 50:
#line 455 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-4].infolist);
          (yyval.infolist)->valuelists.push_back((yyvsp[-1].infolist)->valuelist);
       }
#line 2124 "y.tab.c"
    break;

  case 51:
#line 463 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->valuelist.clear();
          (yyval.infolist)->valuelist.push_back((yyvsp[0].valuetype));
       }
#line 2134 "y.tab.c"
    break;

  case 52:
#line 469 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->valuelist.push_back((yyvsp[0].valuetype));
       }
#line 2143 "y.tab.c"
    break;

  case 53:
#line 477 "../queryparser/yacc.y"
       {
         //printf("value int:%d \n",$1);
         int *d = new int;
         *d = (yyvsp[0].ivalue);
         (yyval.valuetype) = new Value(INT,(BufType)d);
       }
#line 2154 "y.tab.c"
    break;

  case 54:
#line 484 "../queryparser/yacc.y"
       {
          //printf("value float:%f \n",$1);
          double *f = new double;
          *f = (yyvsp[0].fvalue);
			 (yyval.valuetype) = new Value(FLOAT,(BufType)f);
       }
#line 2165 "y.tab.c"
    break;

  case 55:
#line 491 "../queryparser/yacc.y"
       {
          printf("string:%s\n",(yyvsp[0].string));
          (yyval.valuetype) = new Value(STRING,(BufType)(yyvsp[0].string));
       }
#line 2174 "y.tab.c"
    break;

  case 56:
#line 496 "../queryparser/yacc.y"
       {
          (yyval.valuetype) = new Value(DNULL,nullptr);
       }
#line 2182 "y.tab.c"
    break;

  case 57:
#line 503 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = (yyvsp[0].valuetype);
          (yyval.relation)->op = (yyvsp[-1].compop);
       }
#line 2196 "y.tab.c"
    break;

  case 58:
#line 513 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = (yyvsp[0].tcol)->tablename;
	       (yyval.relation)->attr2 = (yyvsp[0].tcol)->colname;
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = (yyvsp[-1].compop);
       }
#line 2210 "y.tab.c"
    break;

  case 59:
#line 523 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-3].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-3].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = CompOp::IS_NOT_NULL;
       }
#line 2224 "y.tab.c"
    break;

  case 60:
#line 533 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = CompOp::IS_NULL;
       }
#line 2238 "y.tab.c"
    break;

  case 61:
#line 545 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->relations.push_back((yyvsp[0].relation));
       }
#line 2247 "y.tab.c"
    break;

  case 62:
#line 550 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->relations.push_back((yyvsp[0].relation));
       }
#line 2256 "y.tab.c"
    break;

  case 63:
#line 558 "../queryparser/yacc.y"
       {
          (yyval.tcol) = new Tcol("",(yyvsp[0].string));
       }
#line 2264 "y.tab.c"
    break;

  case 64:
#line 562 "../queryparser/yacc.y"
       {
          (yyval.tcol) = new Tcol((yyvsp[-2].string),(yyvsp[0].string));
       }
#line 2272 "y.tab.c"
    break;

  case 65:
#line 567 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::EQ_OP;}
#line 2278 "y.tab.c"
    break;

  case 66:
#line 568 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::GT_OP;}
#line 2284 "y.tab.c"
    break;

  case 67:
#line 569 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::LT_OP;}
#line 2290 "y.tab.c"
    break;

  case 68:
#line 570 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::GE_OP;}
#line 2296 "y.tab.c"
    break;

  case 69:
#line 571 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::LE_OP;}
#line 2302 "y.tab.c"
    break;

  case 70:
#line 572 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::NE_OP;}
#line 2308 "y.tab.c"
    break;

  case 71:
#line 576 "../queryparser/yacc.y"
       {
          (yyval.assigns) = new Assigns();
          (yyval.assigns)->attrs.push_back((yyvsp[-2].string));
          (yyval.assigns)->values.push_back((yyvsp[0].valuetype));
          if((yyvsp[0].valuetype)->datatype == DNULL){
             (yyval.assigns)->assignnull.push_back(true);
          }else{
             (yyval.assigns)->assignnull.push_back(false);
          }
       }
#line 2323 "y.tab.c"
    break;

  case 72:
#line 587 "../queryparser/yacc.y"
       {
          (yyval.assigns) = (yyvsp[-4].assigns);
          (yyval.assigns)->attrs.push_back((yyvsp[-2].string));
          (yyval.assigns)->values.push_back((yyvsp[0].valuetype));
          if((yyvsp[0].valuetype)->datatype == DNULL){
             (yyval.assigns)->assignnull.push_back(true);
          }else{
             (yyval.assigns)->assignnull.push_back(false);
          }
       }
#line 2338 "y.tab.c"
    break;

  case 73:
#line 601 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->collist.push_back(new Tcol("","*"));
       }
#line 2347 "y.tab.c"
    break;

  case 74:
#line 606 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->collist.push_back((yyvsp[0].tcol));
       }
#line 2356 "y.tab.c"
    break;

  case 75:
#line 611 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->collist.push_back((yyvsp[0].tcol));
       }
#line 2365 "y.tab.c"
    break;

  case 76:
#line 619 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2374 "y.tab.c"
    break;

  case 77:
#line 624 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2383 "y.tab.c"
    break;

  case 78:
#line 632 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2392 "y.tab.c"
    break;

  case 79:
#line 637 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2401 "y.tab.c"
    break;


#line 2405 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 649 "../queryparser/yacc.y"


void yyerror(const char *msg) {
   printf("YACC error: %s\n", msg);
}
