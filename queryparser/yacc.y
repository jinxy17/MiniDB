%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "Executer.h"
#include "Stmt.h"
int yylex();
void yyerror(const char *);
Executer * executer;
class Stmt;
%}

%union {
    int ivalue;
    float fvalue;
    char * string;
    Stmt * ystmt;
}

// 重复关键字加前缀KW
%token KWNULL KWINT KWFLOAT

// 添加一个退出关键字
%token QUIT
%token DATABASE	DATABASES TABLE	TABLES SHOW	CREATE
%token DROP	USE	PRIMARY	KEY NOT
%token INSERT INTO VALUES DELETE FROM WHERE
%token UPDATE SET SELECT IS VARCHAR
%token DEFAULT CONSTRAINT CHANGE ALTER ADD RENAME
%token DESC INDEX AND DATE FOREIGN
%token REFERENCES ON TO	

%token EQ GT LT GE LE NE

%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING
%token <string> IDENTIFIER 

%type <ivalue> type_width
%type <string> dbName
%type <ystmt> sysStmt dbStmt tbStmt idxStmt alterStmt
%start program

%%

program:
      stmt
       {

       }
    | program stmt
       {

       }
    ;

stmt:
      QUIT ';'
       {
         YYACCEPT;
       }
    | sysStmt ';'    { }
    | dbStmt ';'     { }
    | tbStmt ';'     { }
    | idxStmt ';'    { }
    | alterStmt ';'  { }
    ;

sysStmt:
      SHOW DATABASES
       {
          $$ = new sysStmt();
          executer->execStmt($$);
       }
    ;

dbStmt:
      CREATE DATABASE dbName
       {
          $$ = new dbStmt(dbStmt::DB_CREATE);
          ((dbStmt*)$$)->dbName = $3;
          executer->execStmt($$);
       }
    | DROP DATABASE dbName
       {

       }
    | USE dbName
       {

       }
    | SHOW TABLES
       {

       }
    ;

tbStmt:
      CREATE TABLE tbName '(' fieldList ')'
       {

       }
    | DROP TABLE tbName
       {

       }
    | DESC tbName
       {

       }
    | INSERT INTO tbName VALUES valueLists
       {

       }
    | DELETE FROM tbName WHERE whereClauses
       {

       }
    | UPDATE tbName SET setClause WHERE whereClauses
       {

       }
    | SELECT selector FROM tableList WHERE whereClauses
       {

       }
	;

idxStmt:
	  CREATE INDEX idxName ON tbName '(' columnList ')'
       {

       }
    | DROP INDEX idxName
       {

       }
	| ALTER TABLE tbName ADD INDEX idxName '(' columnList ')'
       {

       }
	| ALTER TABLE tbName DROP INDEX idxName
       {

       }
    ;

alterStmt: 
      ALTER TABLE tbName ADD field
       {

       }
	| ALTER TABLE tbName DROP colName 
       {

       }
	| ALTER TABLE tbName CHANGE colName field
       {

       }
	| ALTER TABLE tbName RENAME TO tbName
       {

       }
	| ALTER TABLE tbName DROP PRIMARY KEY
       {

       }
	| ALTER TABLE tbName ADD CONSTRAINT pkName PRIMARY KEY '(' columnList ')'
       {

       }
	| ALTER TABLE tbName DROP PRIMARY KEY pkName
       {

       }
	| ALTER TABLE tbName ADD CONSTRAINT fkName FOREIGN KEY '(' columnList ')' REFERENCES tbName '(' columnList ')'
       {

       }
	| ALTER TABLE tbName DROP FOREIGN KEY fkName
       {

       }
    ;

fieldList:
      field
       {

       }
    | fieldList ',' field
       {

       }
    ;

field:
      colName type
       {

       }
    | colName type NOT KWNULL
       {

       }
    | colName type DEFAULT value
       {

       }
    | colName type NOT KWNULL DEFAULT value
       {

       }
    | PRIMARY KEY '(' columnList ')'
       {

       }
    | FOREIGN KEY '(' colName ')' REFERENCES tbName '(' colName ')'
       {

       }
    ;

type:
      KWINT type_width
       {

       }
    | VARCHAR type_width
       {

       }
    | DATE
       {

       }
    | KWFLOAT
       {

       }
    ;

type_width:
      '(' VALUE_INT ')'
       {

       }
    |  /* nothing */
       {

       }
    ;

valueLists:
      '(' valueList ')'
       {

       }
    | valueLists ',' '(' valueList ')'
       {

       }
    ;

valueList:
      value
       {

       }
    | valueList ',' value
       {

       }
    ;

value:
      VALUE_INT
       {

       }
	| VALUE_FLOAT
       {

       }
    | VALUE_STRING
       {

       }
    | KWNULL
       {

       }
    ;

whereClause:
      col op expr
       {

       }
    | col IS NOT KWNULL
       {

       }
    | col IS KWNULL
       {

       }
    ;
whereClauses:
      whereClause
       {

       }
    | whereClauses AND whereClause
       {

       }
    ;

col: 
      colName
    | tbName '.' colName
       {

       }
    ;

op: EQ | GT | LT | GE | LE | NE;
expr: 
      value
    | col
    ;

setClause: 
      colName EQ value
       {

       }
    | setClause ',' colName EQ value
       {

       }
    ;

selector:
      '*'
    | col 
    | selector ',' col
    ;

tableList:
      tbName
       {

       }
    | tableList ',' tbName
       {

       }
    ;

columnList: 
      colName
       {

       }
    | columnList ',' colName
       {

       }
    ;

dbName: IDENTIFIER;
tbName: IDENTIFIER;
colName: IDENTIFIER;
idxName: IDENTIFIER;
fkName: IDENTIFIER;
pkName: IDENTIFIER;
%%

void yyerror(const char *msg) {
   printf("YACC error: %s\n", msg);
}

int main() {
   executer = new Executer();
   executer->init();
   int rc = yyparse();
   while(rc) {
      rc = yyparse();
   }
   executer->exit();
   return 0;
}