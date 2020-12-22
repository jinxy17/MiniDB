/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    BufType buftype;
    Lists* infolist;

#line 172 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
