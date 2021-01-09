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
    VALUE_DATE = 307,
    IDENTIFIER = 308
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
#define VALUE_DATE 307
#define IDENTIFIER 308

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

#line 259 "y.tab.c"

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
#define YYLAST   217

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  208

#define YYUNDEFTOK  2
#define YYMAXUTOK   308


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
      55,    56,    59,     2,    57,     2,    58,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    54,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    68,    68,    72,    79,    83,    84,    85,    86,    87,
      91,   100,   108,   116,   124,   133,   151,   158,   162,   171,
     180,   192,   209,   219,   226,   236,   247,   256,   264,   274,
     282,   289,   299,   307,   320,   331,   337,   344,   355,   366,
     378,   390,   396,   406,   410,   414,   418,   425,   430,   436,
     442,   450,   456,   464,   471,   478,   482,   486,   493,   503,
     513,   523,   535,   540,   548,   552,   558,   559,   560,   561,
     562,   563,   566,   577,   591,   596,   601,   609,   614,   622,
     627,   634,   635,   636,   637,   638,   639
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
  "VALUE_DATE", "IDENTIFIER", "';'", "'('", "')'", "','", "'.'", "'*'",
  "$accept", "program", "stmt", "sysStmt", "dbStmt", "tbStmt", "idxStmt",
  "alterStmt", "fieldList", "field", "type", "type_width", "valueLists",
  "valueList", "value", "whereClause", "whereClauses", "col", "op",
  "setClause", "selector", "tableList", "columnList", "dbName", "tbName",
  "colName", "idxName", "fkName", "pkName", YY_NULLPTR
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
     305,   306,   307,   308,    59,    40,    41,    44,    46,    42
};
# endif

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-87)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     125,   -48,    91,     9,    52,   -42,    11,    24,    15,   -29,
      61,    15,    92,  -137,    39,    49,    58,    64,    72,  -137,
    -137,  -137,   -42,    15,    26,   -42,    15,    26,  -137,  -137,
      15,    15,  -137,    87,    63,  -137,  -137,   -19,    85,  -137,
      15,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
      76,  -137,    95,  -137,  -137,  -137,   128,   131,   109,    15,
     121,   109,    56,    48,    15,   102,   121,  -137,   -17,   130,
     -14,  -137,  -137,  -137,    13,   109,     5,   133,   159,   160,
      59,  -137,   104,   123,    25,   122,  -137,   142,   120,   121,
     109,    25,   121,    15,   164,    26,   165,  -137,    48,   134,
      26,  -137,    15,   129,   135,  -137,    48,   136,  -137,  -137,
     136,    68,   109,  -137,  -137,  -137,  -137,  -137,    67,  -137,
     137,   121,    17,  -137,  -137,  -137,  -137,  -137,  -137,    31,
     142,   140,  -137,   142,  -137,   141,  -137,   143,  -137,   170,
     149,   173,   144,  -137,   109,   109,  -137,   146,  -137,  -137,
     194,    25,    73,  -137,  -137,    25,    25,  -137,  -137,   195,
    -137,  -137,    25,  -137,  -137,  -137,  -137,   183,   184,   109,
      78,   147,   148,   172,  -137,  -137,   109,  -137,    89,  -137,
    -137,   150,   151,    97,  -137,   167,  -137,    25,  -137,  -137,
     109,   109,  -137,    15,  -137,   103,   113,   153,   169,  -137,
     109,    15,   154,   156,  -137,   109,   115,  -137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     4,
      10,    14,     0,     0,     0,     0,     0,     0,    81,    13,
       0,     0,    82,     0,    83,    74,    75,     0,     0,    64,
       0,    17,     1,     3,     5,     6,     7,     8,     9,    11,
       0,    84,     0,    12,    16,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,     0,
       0,    77,    76,    65,     0,     0,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    18,    62,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    26,     0,     0,     0,    15,     0,    48,    46,    45,
      48,    37,     0,    57,    53,    54,    56,    55,     0,    51,
       0,     0,     0,    66,    67,    68,    69,    70,    71,     0,
      20,     0,    72,    21,    78,    30,    25,     0,    28,    85,
       0,     0,     0,    29,     0,     0,    36,     0,    43,    44,
       0,     0,     0,    79,    49,     0,     0,    63,    61,     0,
      58,    59,     0,    86,    32,    85,    34,     0,     0,     0,
       0,     0,     0,    38,    39,    22,     0,    52,     0,    60,
      73,     0,     0,     0,    41,     0,    47,     0,    80,    50,
       0,     0,    24,     0,    40,     0,     0,     0,     0,    31,
       0,     0,     0,     0,    42,     0,     0,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,   200,  -137,  -137,  -137,  -137,  -137,  -137,   -39,
    -137,   105,  -137,    57,   -90,    93,   -76,    -7,  -137,  -137,
    -137,  -137,  -136,    23,    -8,   -49,   -22,    79,    82
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,    80,    81,
     111,   148,    85,   118,   119,    86,    87,    88,   129,    68,
      37,    70,   152,    29,    38,    39,    52,   140,   141
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      33,   132,    36,    41,    59,    55,    19,    89,   170,    69,
      92,    28,    73,   130,    82,    50,   133,    22,    54,    23,
     158,    78,    56,    57,    34,    97,    98,    82,   113,    94,
      35,    30,    62,   183,   113,   159,    99,   101,    60,   160,
      90,   131,   100,    93,    79,    49,    24,    31,    53,    82,
      95,    71,    96,    72,   195,   196,    83,    82,    67,   138,
      25,   174,    26,   153,    78,   177,    67,   146,    32,   206,
      74,    40,   180,   136,   114,   115,   116,   117,   142,    51,
     114,   115,   116,   117,    34,   134,   150,    79,    75,    27,
      76,    77,    42,    44,   143,   153,   171,   194,   151,     1,
      20,    67,    21,    45,     2,     3,     4,     5,   107,   108,
     109,     6,    46,    58,     7,   105,   106,     8,    47,     9,
     153,   -82,   161,   154,   155,    10,    48,   188,    11,   175,
     176,    63,     1,   110,   184,   176,    64,     2,     3,     4,
       5,   153,   153,    61,     6,   189,   155,     7,   122,    65,
       8,   202,     9,   192,   176,    66,   153,    84,    10,   198,
     176,    11,    67,   123,   124,   125,   126,   127,   128,   199,
     176,   207,   176,    91,    34,   102,   103,   104,   112,   120,
     121,   135,   137,   162,   144,   197,   -86,   139,   167,   168,
     145,   147,   156,   203,   163,   172,   165,   173,   179,   169,
     181,   182,   187,   185,   186,   190,   191,   193,   200,   201,
     204,   205,    43,   178,   157,   149,   166,   164
};

static const yytype_uint8 yycheck[] =
{
       8,    91,     9,    11,    23,    27,    54,    24,   144,    58,
      24,    53,    61,    89,    63,    23,    92,     8,    26,    10,
       3,    16,    30,    31,    53,    74,    75,    76,     3,    16,
      59,    20,    40,   169,     3,    18,    31,    76,    57,   129,
      57,    90,    37,    57,    39,    22,    37,    23,    25,    98,
      37,    59,    39,    60,   190,   191,    64,   106,    53,    98,
       8,   151,    10,   112,    16,   155,    53,   106,    53,   205,
      14,    10,   162,    95,    49,    50,    51,    52,   100,    53,
      49,    50,    51,    52,    53,    93,    18,    39,    32,    37,
      34,    35,     0,    54,   102,   144,   145,   187,    30,     7,
       9,    53,    11,    54,    12,    13,    14,    15,     4,     5,
       6,    19,    54,    26,    22,    56,    57,    25,    54,    27,
     169,    58,   129,    56,    57,    33,    54,   176,    36,    56,
      57,    55,     7,    29,    56,    57,    41,    12,    13,    14,
      15,   190,   191,    58,    19,    56,    57,    22,    28,    21,
      25,   200,    27,    56,    57,    24,   205,    55,    33,    56,
      57,    36,    53,    43,    44,    45,    46,    47,    48,    56,
      57,    56,    57,    43,    53,    42,    17,    17,    55,    57,
      38,    17,    17,    43,    55,   193,    16,    53,    39,    16,
      55,    55,    55,   201,    53,    49,    53,     3,     3,    55,
      17,    17,    30,    56,    56,    55,    55,    40,    55,    40,
      56,    55,    12,   156,   121,   110,   137,   135
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    12,    13,    14,    15,    19,    22,    25,    27,
      33,    36,    61,    62,    63,    64,    65,    66,    67,    54,
       9,    11,     8,    10,    37,     8,    10,    37,    53,    83,
      20,    23,    53,    84,    53,    59,    77,    80,    84,    85,
      10,    84,     0,    62,    54,    54,    54,    54,    54,    83,
      84,    53,    86,    83,    84,    86,    84,    84,    26,    23,
      57,    58,    84,    55,    41,    21,    24,    53,    79,    85,
      81,    84,    77,    85,    14,    32,    34,    35,    16,    39,
      68,    69,    85,    84,    55,    72,    75,    76,    77,    24,
      57,    43,    24,    57,    16,    37,    39,    85,    85,    31,
      37,    69,    42,    17,    17,    56,    57,     4,     5,     6,
      29,    70,    55,     3,    49,    50,    51,    52,    73,    74,
      57,    38,    28,    43,    44,    45,    46,    47,    48,    78,
      76,    85,    74,    76,    84,    17,    86,    17,    69,    53,
      87,    88,    86,    84,    55,    55,    69,    55,    71,    71,
      18,    30,    82,    85,    56,    57,    55,    75,     3,    18,
      74,    77,    43,    53,    88,    53,    87,    39,    16,    55,
      82,    85,    49,     3,    74,    56,    57,    74,    73,     3,
      74,    17,    17,    82,    56,    56,    56,    30,    85,    56,
      55,    55,    56,    40,    74,    82,    82,    84,    56,    56,
      55,    40,    85,    84,    56,    55,    82,    56
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    62,    62,    62,    62,
      63,    64,    64,    64,    64,    65,    65,    65,    65,    65,
      65,    65,    66,    66,    66,    66,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    68,    68,    69,    69,    69,
      69,    69,    69,    70,    70,    70,    70,    71,    71,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    75,    75,
      75,    75,    76,    76,    77,    77,    78,    78,    78,    78,
      78,    78,    79,    79,    80,    80,    80,    81,    81,    82,
      82,    83,    84,    85,    86,    87,    88
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     2,     2,     6,     3,     2,     5,     5,
       6,     6,     8,     3,     9,     6,     5,     5,     6,     6,
       6,    11,     7,    16,     7,     1,     3,     2,     4,     4,
       6,     5,    10,     2,     2,     1,     1,     3,     0,     3,
       5,     1,     3,     1,     1,     1,     1,     1,     3,     3,
       4,     3,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     5,     1,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1
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
#line 1574 "y.tab.c"
    break;

  case 3:
#line 73 "../queryparser/yacc.y"
       {

       }
#line 1582 "y.tab.c"
    break;

  case 4:
#line 80 "../queryparser/yacc.y"
       {
         YYACCEPT;
       }
#line 1590 "y.tab.c"
    break;

  case 5:
#line 83 "../queryparser/yacc.y"
                     { }
#line 1596 "y.tab.c"
    break;

  case 6:
#line 84 "../queryparser/yacc.y"
                     { }
#line 1602 "y.tab.c"
    break;

  case 7:
#line 85 "../queryparser/yacc.y"
                     { }
#line 1608 "y.tab.c"
    break;

  case 8:
#line 86 "../queryparser/yacc.y"
                     { }
#line 1614 "y.tab.c"
    break;

  case 9:
#line 87 "../queryparser/yacc.y"
                     { }
#line 1620 "y.tab.c"
    break;

  case 10:
#line 92 "../queryparser/yacc.y"
       {
          sysStmt* stmt = new sysStmt();
          executer->execSysStmt(stmt);
          delete stmt;
       }
#line 1630 "y.tab.c"
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
#line 1642 "y.tab.c"
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
#line 1654 "y.tab.c"
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
#line 1666 "y.tab.c"
    break;

  case 14:
#line 125 "../queryparser/yacc.y"
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_SHOW);
          executer->execDbStmt(stmt);
          delete stmt;
       }
#line 1676 "y.tab.c"
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
          //TODO: 添加对PK和FK的处理
          for(int i = 0;i < (yyvsp[-1].infolist)->attrinfos.size();i++) {
             stmt->tableInfo->attrs.push_back(*(yyvsp[-1].infolist)->attrinfos[i]);
          }
          stmt->tableInfo->attrNum = stmt->tableInfo->attrs.size();
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1698 "y.tab.c"
    break;

  case 16:
#line 152 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DROP);
          stmt->tbName = (yyvsp[0].string);
          executer->execTbStmt(stmt);
          delete stmt;
       }
#line 1709 "y.tab.c"
    break;

  case 17:
#line 159 "../queryparser/yacc.y"
       {
          //TODO:还没有相应的底层函数
       }
#line 1717 "y.tab.c"
    break;

  case 18:
#line 163 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_INSERT);
          stmt->tbName = (yyvsp[-2].string);
          stmt->datas = (yyvsp[0].infolist)->valuelists;
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[0].infolist);
       }
#line 1730 "y.tab.c"
    break;

  case 19:
#line 172 "../queryparser/yacc.y"
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DELETE);
          stmt->tbName = (yyvsp[-2].string);
          stmt->relations.assign((yyvsp[0].infolist)->relations.begin(),(yyvsp[0].infolist)->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete (yyvsp[0].infolist);
       }
#line 1743 "y.tab.c"
    break;

  case 20:
#line 181 "../queryparser/yacc.y"
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
#line 1759 "y.tab.c"
    break;

  case 21:
#line 193 "../queryparser/yacc.y"
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
#line 1777 "y.tab.c"
    break;

  case 22:
#line 210 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_CREATE);
          stmt->tbName = (yyvsp[-3].string);
          stmt->idxName = (yyvsp[-5].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1791 "y.tab.c"
    break;

  case 23:
#line 220 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_DROP);
          executer->execIdxStmt(stmt);
          stmt->idxName = (yyvsp[0].string);
          delete stmt;
       }
#line 1802 "y.tab.c"
    break;

  case 24:
#line 227 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_ADD);
          stmt->tbName = (yyvsp[-6].string);
          stmt->idxName = (yyvsp[-3].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1816 "y.tab.c"
    break;

  case 25:
#line 237 "../queryparser/yacc.y"
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_DROP);
          stmt->tbName = (yyvsp[-3].string);
          stmt->idxName = (yyvsp[0].string);
          executer->execIdxStmt(stmt);
          delete stmt;
       }
#line 1828 "y.tab.c"
    break;

  case 26:
#line 248 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD);
          stmt->tbName = (yyvsp[-2].string);
          stmt->field = (yyvsp[0].attrinfo);
          executer->execAlterStmt(stmt);
         //  delete stmt;
         //  delete $5;
       }
#line 1841 "y.tab.c"
    break;

  case 27:
#line 257 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP);
          stmt->tbName = (yyvsp[-2].string);
          stmt->colName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1853 "y.tab.c"
    break;

  case 28:
#line 265 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_CHANGE);
          stmt->tbName = (yyvsp[-3].string);
          stmt->colName = (yyvsp[-1].string);
          stmt->field = (yyvsp[0].attrinfo);
          executer->execAlterStmt(stmt);
          delete stmt;
          delete (yyvsp[0].attrinfo);
       }
#line 1867 "y.tab.c"
    break;

  case 29:
#line 275 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_RENAME);
          stmt->tbName = (yyvsp[-3].string);
          stmt->newtbName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1879 "y.tab.c"
    break;

  case 30:
#line 283 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY);
          stmt->tbName = (yyvsp[-3].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1890 "y.tab.c"
    break;

  case 31:
#line 290 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD_PRIMARY);
          stmt->tbName = (yyvsp[-8].string);
          stmt->pkName = (yyvsp[-5].string);
          stmt->collist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
          executer->execAlterStmt(stmt);
          delete stmt;
          delete (yyvsp[-1].infolist);
       }
#line 1904 "y.tab.c"
    break;

  case 32:
#line 300 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY_WNAME);
          stmt->tbName = (yyvsp[-4].string);
          stmt->pkName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1916 "y.tab.c"
    break;

  case 33:
#line 308 "../queryparser/yacc.y"
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
#line 1933 "y.tab.c"
    break;

  case 34:
#line 321 "../queryparser/yacc.y"
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_FOREIGN_WNAME);
          stmt->tbName = (yyvsp[-4].string);
          stmt->fkName = (yyvsp[0].string);
          executer->execAlterStmt(stmt);
          delete stmt;
       }
#line 1945 "y.tab.c"
    break;

  case 35:
#line 332 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->attrinfos.clear();
          (yyval.infolist)->attrinfos.push_back((yyvsp[0].attrinfo));
       }
#line 1955 "y.tab.c"
    break;

  case 36:
#line 338 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->attrinfos.push_back((yyvsp[0].attrinfo));
       }
#line 1964 "y.tab.c"
    break;

  case 37:
#line 345 "../queryparser/yacc.y"
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
#line 1979 "y.tab.c"
    break;

  case 38:
#line 356 "../queryparser/yacc.y"
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
#line 1994 "y.tab.c"
    break;

  case 39:
#line 367 "../queryparser/yacc.y"
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
#line 2010 "y.tab.c"
    break;

  case 40:
#line 379 "../queryparser/yacc.y"
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
#line 2026 "y.tab.c"
    break;

  case 41:
#line 391 "../queryparser/yacc.y"
       {
         (yyval.attrinfo) = new AttrInfo();
         (yyval.attrinfo)->namelist.assign((yyvsp[-1].infolist)->namelist.begin(),(yyvsp[-1].infolist)->namelist.end());
         (yyval.attrinfo)->createPk = true;
       }
#line 2036 "y.tab.c"
    break;

  case 42:
#line 397 "../queryparser/yacc.y"
       {
         (yyval.attrinfo) = new AttrInfo();
         (yyval.attrinfo)->attrName = (yyvsp[-6].string);
	      (yyval.attrinfo)->reference = (yyvsp[-3].string);
         (yyval.attrinfo)->foreignKeyName = (yyvsp[-1].string); 
       }
#line 2047 "y.tab.c"
    break;

  case 43:
#line 407 "../queryparser/yacc.y"
       {
         (yyval.datatype) = new DataType(INT,(yyvsp[0].ivalue));
       }
#line 2055 "y.tab.c"
    break;

  case 44:
#line 411 "../queryparser/yacc.y"
       {
          (yyval.datatype) = new DataType(STRING,(yyvsp[0].ivalue));
       }
#line 2063 "y.tab.c"
    break;

  case 45:
#line 415 "../queryparser/yacc.y"
       {
          (yyval.datatype) = new DataType(DATE,8);
       }
#line 2071 "y.tab.c"
    break;

  case 46:
#line 419 "../queryparser/yacc.y"
       {
          (yyval.datatype) = new DataType(FLOAT,8);
       }
#line 2079 "y.tab.c"
    break;

  case 47:
#line 426 "../queryparser/yacc.y"
       {

       }
#line 2087 "y.tab.c"
    break;

  case 48:
#line 430 "../queryparser/yacc.y"
       {

       }
#line 2095 "y.tab.c"
    break;

  case 49:
#line 437 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->valuelists.clear();
          (yyval.infolist)->valuelists.push_back((yyvsp[-1].infolist)->valuelist);
       }
#line 2105 "y.tab.c"
    break;

  case 50:
#line 443 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-4].infolist);
          (yyval.infolist)->valuelists.push_back((yyvsp[-1].infolist)->valuelist);
       }
#line 2114 "y.tab.c"
    break;

  case 51:
#line 451 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->valuelist.clear();
          (yyval.infolist)->valuelist.push_back((yyvsp[0].valuetype));
       }
#line 2124 "y.tab.c"
    break;

  case 52:
#line 457 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->valuelist.push_back((yyvsp[0].valuetype));
       }
#line 2133 "y.tab.c"
    break;

  case 53:
#line 465 "../queryparser/yacc.y"
       {
         //printf("value int:%d \n",$1);
         int *d = new int;
         *d = (yyvsp[0].ivalue);
         (yyval.valuetype) = new Value(INT,(BufType)d);
       }
#line 2144 "y.tab.c"
    break;

  case 54:
#line 472 "../queryparser/yacc.y"
       {
          //printf("value float:%f \n",$1);
          double *f = new double;
          *f = (yyvsp[0].fvalue);
			 (yyval.valuetype) = new Value(FLOAT,(BufType)f);
       }
#line 2155 "y.tab.c"
    break;

  case 55:
#line 479 "../queryparser/yacc.y"
       {
          printf("value date:%s \n",(yyvsp[0].string));
       }
#line 2163 "y.tab.c"
    break;

  case 56:
#line 483 "../queryparser/yacc.y"
       {
          (yyval.valuetype) = new Value(STRING,(BufType)(yyvsp[0].string));
       }
#line 2171 "y.tab.c"
    break;

  case 57:
#line 487 "../queryparser/yacc.y"
       {
          (yyval.valuetype) = new Value(DNULL,nullptr);
       }
#line 2179 "y.tab.c"
    break;

  case 58:
#line 494 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = (yyvsp[0].valuetype);
          (yyval.relation)->op = (yyvsp[-1].compop);
       }
#line 2193 "y.tab.c"
    break;

  case 59:
#line 504 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = (yyvsp[0].tcol)->tablename;
	       (yyval.relation)->attr2 = (yyvsp[0].tcol)->colname;
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = (yyvsp[-1].compop);
       }
#line 2207 "y.tab.c"
    break;

  case 60:
#line 514 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-3].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-3].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = CompOp::IS_NOT_NULL;
       }
#line 2221 "y.tab.c"
    break;

  case 61:
#line 524 "../queryparser/yacc.y"
       {
          (yyval.relation) = new Relation();
          (yyval.relation)->table1 = (yyvsp[-2].tcol)->tablename;
	       (yyval.relation)->attr1 = (yyvsp[-2].tcol)->colname;
          (yyval.relation)->table2 = "";
	       (yyval.relation)->attr2 = "";
          (yyval.relation)->value = new Value(DNULL,nullptr);
          (yyval.relation)->op = CompOp::IS_NULL;
       }
#line 2235 "y.tab.c"
    break;

  case 62:
#line 536 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->relations.push_back((yyvsp[0].relation));
       }
#line 2244 "y.tab.c"
    break;

  case 63:
#line 541 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->relations.push_back((yyvsp[0].relation));
       }
#line 2253 "y.tab.c"
    break;

  case 64:
#line 549 "../queryparser/yacc.y"
       {
          (yyval.tcol) = new Tcol("",(yyvsp[0].string));
       }
#line 2261 "y.tab.c"
    break;

  case 65:
#line 553 "../queryparser/yacc.y"
       {
          (yyval.tcol) = new Tcol((yyvsp[-2].string),(yyvsp[0].string));
       }
#line 2269 "y.tab.c"
    break;

  case 66:
#line 558 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::EQ_OP;}
#line 2275 "y.tab.c"
    break;

  case 67:
#line 559 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::GT_OP;}
#line 2281 "y.tab.c"
    break;

  case 68:
#line 560 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::LT_OP;}
#line 2287 "y.tab.c"
    break;

  case 69:
#line 561 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::GE_OP;}
#line 2293 "y.tab.c"
    break;

  case 70:
#line 562 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::LE_OP;}
#line 2299 "y.tab.c"
    break;

  case 71:
#line 563 "../queryparser/yacc.y"
       {(yyval.compop) = CompOp::NE_OP;}
#line 2305 "y.tab.c"
    break;

  case 72:
#line 567 "../queryparser/yacc.y"
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
#line 2320 "y.tab.c"
    break;

  case 73:
#line 578 "../queryparser/yacc.y"
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
#line 2335 "y.tab.c"
    break;

  case 74:
#line 592 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->collist.push_back(new Tcol("","*"));
       }
#line 2344 "y.tab.c"
    break;

  case 75:
#line 597 "../queryparser/yacc.y"
       {
          (yyval.infolist) = new Lists();
          (yyval.infolist)->collist.push_back((yyvsp[0].tcol));
       }
#line 2353 "y.tab.c"
    break;

  case 76:
#line 602 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->collist.push_back((yyvsp[0].tcol));
       }
#line 2362 "y.tab.c"
    break;

  case 77:
#line 610 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2371 "y.tab.c"
    break;

  case 78:
#line 615 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2380 "y.tab.c"
    break;

  case 79:
#line 623 "../queryparser/yacc.y"
       {
         (yyval.infolist) = new Lists();
         (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2389 "y.tab.c"
    break;

  case 80:
#line 628 "../queryparser/yacc.y"
       {
          (yyval.infolist) = (yyvsp[-2].infolist);
          (yyval.infolist)->namelist.push_back((yyvsp[0].string));
       }
#line 2398 "y.tab.c"
    break;


#line 2402 "y.tab.c"

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
#line 640 "../queryparser/yacc.y"


void yyerror(const char *msg) {
   printf("YACC error: %s\n", msg);
}
