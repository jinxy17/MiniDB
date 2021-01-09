%{
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
%}

%union {
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
}

// 重复关键字加前缀KW
%token KWNULL KWINT KWFLOAT KWDATE

// 添加一个退出关键字
%token QUIT
%token DATABASE	DATABASES TABLE	TABLES SHOW	CREATE
%token DROP	USE	PRIMARY	KEY NOT
%token INSERT INTO VALUES DELETE FROM WHERE
%token UPDATE SET SELECT IS VARCHAR
%token DEFAULT CONSTRAINT CHANGE ALTER ADD RENAME
%token DESC INDEX AND FOREIGN
%token REFERENCES ON TO	

%token EQ GT LT GE LE NE

%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING
%token <string> IDENTIFIER 

%type <ivalue> type_width
%type <string> dbName tbName colName idxName pkName fkName
%type <attrinfo> field
%type <datatype> type
%type <valuetype> value
%type <infolist> fieldList valueLists valueList columnList tableList whereClauses selector
%type <tcol> col
%type <relation> whereClause
%type <compop> op
%type <assigns> setClause

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
          sysStmt* stmt = new sysStmt();
          executer->execSysStmt(stmt);
          delete stmt;
       }
    ;

dbStmt:
      CREATE DATABASE dbName
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_CREATE);
          stmt->dbName = $3;
          executer->execDbStmt(stmt);
          delete $3;
          delete stmt;
       }
    | DROP DATABASE dbName
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_DROP);
          stmt->dbName = $3;
          executer->execDbStmt(stmt);
          delete $3;
          delete stmt;
       }
    | USE dbName
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_USE);
          stmt->dbName = $2;
          executer->execDbStmt(stmt);
          delete $2;
          delete stmt;
       }
    | SHOW TABLES
       {
          dbStmt* stmt = new dbStmt(dbStmt::DB_SHOW);
          executer->execDbStmt(stmt);
          delete stmt;
       }
    ;

tbStmt:
      CREATE TABLE tbName '(' fieldList ')'
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_CREATE);
          stmt->tbName = $3;

          stmt->tableInfo = new TableInfo();
			 stmt->tableInfo->tableName = stmt->tbName;
          stmt->tableInfo->recordSize = 0;
          stmt->tableInfo->attrs.clear();
          //TODO: 添加对PK和FK的处理
          for(int i = 0;i < $5->attrinfos.size();i++) {
             stmt->tableInfo->attrs.push_back(*$5->attrinfos[i]);
          }
          stmt->tableInfo->attrNum = stmt->tableInfo->attrs.size();
          executer->execTbStmt(stmt);
          delete stmt;
          delete $5;
       }
    | DROP TABLE tbName
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DROP);
          stmt->tbName = $3;
          executer->execTbStmt(stmt);
          delete stmt;
       }
    | DESC tbName
       {
          //TODO:还没有相应的底层函数
       }
    | INSERT INTO tbName VALUES valueLists
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_INSERT);
          stmt->tbName = $3;
          stmt->datas = $5->valuelists;
          executer->execTbStmt(stmt);
          delete stmt;
          delete $5;
       }
    | DELETE FROM tbName WHERE whereClauses
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_DELETE);
          stmt->tbName = $3;
          stmt->relations.assign($5->relations.begin(),$5->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete $5;
       }
    | UPDATE tbName SET setClause WHERE whereClauses
       {
          tbStmt* stmt = new tbStmt(tbStmt::TB_UPDATE);
          stmt->tbName = $2;
          stmt->assigns = $4;
          stmt->assigns->table = $2;
          stmt->relations.assign($6->relations.begin(),$6->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete $4;
          delete $6;
       }
    | SELECT selector FROM tableList WHERE whereClauses
       {
          // TODO:三张以上表的连接
          tbStmt* stmt = new tbStmt(tbStmt::TB_SELECT);
          //printf("select: cols num:%ld,tables num:%ld,whereclauses num:%ld\n",$2->collist.size(),$4->namelist.size(),$6->relations.size());
          stmt->collist.assign($2->collist.begin(),$2->collist.end());
          stmt->tablelist.assign($4->namelist.begin(),$4->namelist.end());
          stmt->relations.assign($6->relations.begin(),$6->relations.end());
          executer->execTbStmt(stmt);
          delete stmt;
          delete $2;
          delete $4;
          delete $6;
       }
	;

idxStmt:
	  CREATE INDEX idxName ON tbName '(' columnList ')'
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_CREATE);
          stmt->tbName = $5;
          stmt->idxName = $3;
          stmt->collist.assign($7->namelist.begin(),$7->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete $7;
       }
    | DROP INDEX idxName
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_DROP);
          executer->execIdxStmt(stmt);
          stmt->idxName = $3;
          delete stmt;
       }
	| ALTER TABLE tbName ADD INDEX idxName '(' columnList ')'
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_ADD);
          stmt->tbName = $3;
          stmt->idxName = $6;
          stmt->collist.assign($8->namelist.begin(),$8->namelist.end());
          executer->execIdxStmt(stmt);
          delete stmt;
          delete $8;
       }
	| ALTER TABLE tbName DROP INDEX idxName
       {
          idxStmt* stmt = new idxStmt(idxStmt::IDX_ALTER_DROP);
          stmt->tbName = $3;
          stmt->idxName = $6;
          executer->execIdxStmt(stmt);
          delete stmt;
       }
    ;

alterStmt: 
      ALTER TABLE tbName ADD field
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD);
          stmt->tbName = $3;
          stmt->field = $5;
          executer->execAlterStmt(stmt);
         //  delete stmt;
         //  delete $5;
       }
	| ALTER TABLE tbName DROP colName 
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP);
          stmt->tbName = $3;
          stmt->colName = $5;
          executer->execAlterStmt(stmt);
          delete stmt;
       }
	| ALTER TABLE tbName CHANGE colName field
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_CHANGE);
          stmt->tbName = $3;
          stmt->colName = $5;
          stmt->field = $6;
          executer->execAlterStmt(stmt);
          delete stmt;
          delete $6;
       }
	| ALTER TABLE tbName RENAME TO tbName
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_RENAME);
          stmt->tbName = $3;
          stmt->newtbName = $6;
          executer->execAlterStmt(stmt);
          delete stmt;
       }
	| ALTER TABLE tbName DROP PRIMARY KEY
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY);
          stmt->tbName = $3;
          executer->execAlterStmt(stmt);
          delete stmt;
       }
	| ALTER TABLE tbName ADD CONSTRAINT pkName PRIMARY KEY '(' columnList ')'
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD_PRIMARY);
          stmt->tbName = $3;
          stmt->pkName = $6;
          stmt->collist.assign($10->namelist.begin(),$10->namelist.end());
          executer->execAlterStmt(stmt);
          delete stmt;
          delete $10;
       }
	| ALTER TABLE tbName DROP PRIMARY KEY pkName
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_PRIMARY_WNAME);
          stmt->tbName = $3;
          stmt->pkName = $7;
          executer->execAlterStmt(stmt);
          delete stmt;
       }
	| ALTER TABLE tbName ADD CONSTRAINT fkName FOREIGN KEY '(' columnList ')' REFERENCES tbName '(' columnList ')'
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_ADD_FOREIGN);
          stmt->tbName = $3;
          stmt->fkName = $6;
          stmt->collist.assign($10->namelist.begin(),$10->namelist.end());
          stmt->fktbName = $13;
          stmt->fkcollist.assign($15->namelist.begin(),$15->namelist.end());
          executer->execAlterStmt(stmt);
          delete stmt;
          delete $10;
          delete $15;
       }
	| ALTER TABLE tbName DROP FOREIGN KEY fkName
       {
          alterStmt* stmt = new alterStmt(alterStmt::ALTER_DROP_FOREIGN_WNAME);
          stmt->tbName = $3;
          stmt->fkName = $7;
          executer->execAlterStmt(stmt);
          delete stmt;
       }
    ;

fieldList:
      field
       {
          $$ = new Lists();
          $$->attrinfos.clear();
          $$->attrinfos.push_back($1);
       }
    | fieldList ',' field
       {
          $$ = $1;
          $$->attrinfos.push_back($3);
       }
    ;
field:
      colName type
       {
          $$ = new AttrInfo();
          $$->attrName = $1;
	       $$->attrType = $2->dtype;
          $$->attrLength = $2->setlength;
	       $$->notNull = false;
	       $$->primary = false;
	       $$->haveIndex = false;
          delete $2;
       }
    | colName type NOT KWNULL
       {
          $$ = new AttrInfo();
          $$->attrName = $1;
	       $$->attrType = $2->dtype;
          $$->attrLength = $2->setlength;
	       $$->notNull = true;
	       $$->primary = false;
	       $$->haveIndex = false;
          delete $2;
       }
    | colName type DEFAULT value
       {
          $$ = new AttrInfo();
          $$->attrName = $1;
	       $$->attrType = $2->dtype;
          $$->attrLength = $2->setlength;
	       $$->notNull = false;
	       $$->primary = false;
	       $$->haveIndex = false;
          //TODO：sys模块需要添加对int转float,date转int的处理
          $$->defaultValue = $4->data;
       }
    | colName type NOT KWNULL DEFAULT value
       {
          $$ = new AttrInfo();
          $$->attrName = $1;
	       $$->attrType = $2->dtype;
          $$->attrLength = $2->setlength;
	       $$->notNull = true;
	       $$->primary = false;
	       $$->haveIndex = false;
          //TODO：sys模块需要添加对int转float,date转int的处理
          $$->defaultValue = $6->data;
       }
    | PRIMARY KEY '(' columnList ')'
       {
         $$ = new AttrInfo();
         $$->namelist.assign($4->namelist.begin(),$4->namelist.end());
         $$->createPk = true;
       }
    | FOREIGN KEY '(' colName ')' REFERENCES tbName '(' colName ')'
       {
         $$ = new AttrInfo();
         $$->attrName = $4;
	      $$->reference = $7;
         $$->foreignKeyName = $9; 
       }
    ;

type:
      KWINT type_width 
       {
         $$ = new DataType(INT,$2);
       }
    | VARCHAR type_width
       {
          $$ = new DataType(STRING,$2);
       }
    | KWDATE
       {
          //DATE直接用String表示似乎更方便快捷
          $$ = new DataType(STRING,12);
       }
    | KWFLOAT
       {
          $$ = new DataType(FLOAT,8);
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
          $$ = new Lists();
          $$->valuelists.clear();
          $$->valuelists.push_back($2->valuelist);
       }
    | valueLists ',' '(' valueList ')'
       {
          $$ = $1;
          $$->valuelists.push_back($4->valuelist);
       }
    ;

valueList:
      value
       {
          $$ = new Lists();
          $$->valuelist.clear();
          $$->valuelist.push_back($1);
       }
    | valueList ',' value
       {
          $$ = $1;
          $$->valuelist.push_back($3);
       }
    ;

value:
      VALUE_INT
       {
         //printf("value int:%d \n",$1);
         int *d = new int;
         *d = $1;
         $$ = new Value(INT,(BufType)d);
       }
	 | VALUE_FLOAT
       {
          //printf("value float:%f \n",$1);
          double *f = new double;
          *f = $1;
			 $$ = new Value(FLOAT,(BufType)f);
       }
    | VALUE_STRING
       {
          $$ = new Value(STRING,(BufType)$1);
       }
    | KWNULL
       {
          $$ = new Value(DNULL,nullptr);
       }
    ;

whereClause:
      col op value
       {
          $$ = new Relation();
          $$->table1 = $1->tablename;
	       $$->attr1 = $1->colname;
          $$->table2 = "";
	       $$->attr2 = "";
          $$->value = $3;
          $$->op = $2;
       }
    | col op col
       {
          $$ = new Relation();
          $$->table1 = $1->tablename;
	       $$->attr1 = $1->colname;
          $$->table2 = $3->tablename;
	       $$->attr2 = $3->colname;
          $$->value = new Value(DNULL,nullptr);
          $$->op = $2;
       }
    |col IS NOT KWNULL
       {
          $$ = new Relation();
          $$->table1 = $1->tablename;
	       $$->attr1 = $1->colname;
          $$->table2 = "";
	       $$->attr2 = "";
          $$->value = new Value(DNULL,nullptr);
          $$->op = CompOp::IS_NOT_NULL;
       }
    | col IS KWNULL
       {
          $$ = new Relation();
          $$->table1 = $1->tablename;
	       $$->attr1 = $1->colname;
          $$->table2 = "";
	       $$->attr2 = "";
          $$->value = new Value(DNULL,nullptr);
          $$->op = CompOp::IS_NULL;
       }
    ;
whereClauses:
      whereClause
       {
         $$ = new Lists();
         $$->relations.push_back($1);
       }
    | whereClauses AND whereClause
       {
          $$ = $1;
          $$->relations.push_back($3);
       }
    ;

col: 
      colName
       {
          $$ = new Tcol("",$1);
       }
    | tbName '.' colName
       {
          $$ = new Tcol($1,$3);
       }
    ;

op: EQ {$$ = CompOp::EQ_OP;}
  | GT {$$ = CompOp::GT_OP;}
  | LT {$$ = CompOp::LT_OP;}
  | GE {$$ = CompOp::GE_OP;}
  | LE {$$ = CompOp::LE_OP;}
  | NE {$$ = CompOp::NE_OP;};

setClause: 
      colName EQ value
       {
          $$ = new Assigns();
          $$->attrs.push_back($1);
          $$->values.push_back($3);
          if($3->datatype == DNULL){
             $$->assignnull.push_back(true);
          }else{
             $$->assignnull.push_back(false);
          }
       }
    | setClause ',' colName EQ value
       {
          $$ = $1;
          $$->attrs.push_back($3);
          $$->values.push_back($5);
          if($5->datatype == DNULL){
             $$->assignnull.push_back(true);
          }else{
             $$->assignnull.push_back(false);
          }
       }
    ;

selector:
      '*'
       {
          $$ = new Lists();
          $$->collist.push_back(new Tcol("","*"));
       }
    | col 
       {
          $$ = new Lists();
          $$->collist.push_back($1);
       }
    | selector ',' col
       {
          $$ = $1;
          $$->collist.push_back($3);
       }
    ;

tableList:
      tbName
       {
         $$ = new Lists();
         $$->namelist.push_back($1);
       }
    | tableList ',' tbName
       {
          $$ = $1;
          $$->namelist.push_back($3);
       }
    ;

columnList: 
      colName
       {
         $$ = new Lists();
         $$->namelist.push_back($1);
       }
    | columnList ',' colName
       {
          $$ = $1;
          $$->namelist.push_back($3);
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