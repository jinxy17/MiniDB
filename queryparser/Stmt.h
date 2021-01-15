#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include "../sysmgr/SysManager.h"
using namespace std;

enum CompOp {
	NO_OP, EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, IS_NULL, IS_NOT_NULL
};

struct DataType {
    int dtype;
    int setlength;
    DataType(int _dtype,int _setlength):dtype(_dtype),setlength(_setlength){};
};

struct Tcol {
    string tablename;
    string colname;
    Tcol(string _tablename,string _colname):tablename(_tablename),colname(_colname){};
};

struct Value {
    int datatype;
    BufType data;
    Value(int _datatype,BufType _data):datatype(_datatype),data(_data){};
};

struct Relation {
	string table1;
	string attr1;
	string table2;
	string attr2;
	Value* value;
	CompOp op;
};

struct Assigns {
	string table;
	vector<string> attrs;
	vector<Value*> values;
	//在添加了DNULL类型后assignnull实际为冗余成员
	vector<bool> assignnull;
};

struct Assign {
	string table;
	string attr;
	Value* value;
	bool assignnull;
};

struct Lists{
    vector<AttrInfo*> attrinfos;
    vector<Value*> valuelist;
    vector<vector<Value*> > valuelists;
    vector<string> namelist;
    vector<Tcol*> collist; 
    vector<Relation*> relations;
    Lists(){
        attrinfos.clear();
        valuelist.clear();
        valuelists.clear();
        namelist.clear();
        collist.clear();
        relations.clear();
    }
    ~Lists(){
        for(int i = 0;i < attrinfos.size();i++)
            delete attrinfos[i];
        attrinfos.clear();
        for(int i = 0;i < valuelist.size();i++)
            delete valuelist[i];
        valuelist.clear();
        for(int i = 0;i < valuelists.size();i++){
            for(int j = 0;j < valuelists[i].size();j++)
                delete valuelists[i][j];
            valuelists[i].clear();
        }
        valuelists.clear();
        namelist.clear();
        for(int i = 0;i < collist.size();i++)
            delete collist[i];
        collist.clear();
        for(int i = 0;i < relations.size();i++)
            delete relations[i];
        relations.clear();
    }
};

class Stmt {
public:
    enum Type {
	    SYS_STMT,DB_STMT,TB_STMT,IDX_STMT,ALTER_STMT
    };
	Type sttype;
    Stmt(Type _sttype){
        sttype = _sttype;
    };
    ~Stmt(){};
};

//sysStmt只有一句SHOW DATABASES
class sysStmt :public Stmt {    
public:
    sysStmt(): Stmt(Stmt::SYS_STMT){};
    ~sysStmt(){};
};

// dbStmt有:
// CREATE DATABASE <dbName>
// DROP DATABASE <dbName>
// USE <dbName>
// SHOW TABLES
class dbStmt :public Stmt {
public:
    enum Type {
	    DB_CREATE,DB_DROP,DB_USE,DB_SHOW
    };
    string dbName;
    Type dbType;

    dbStmt(Type _dbtype): Stmt(Stmt::DB_STMT) {
        dbType = _dbtype;
    };
    ~dbStmt(){};
};

// tbStmt有:
// CREATE TABLE <tbName> ’(’<fieldList>’)’
// DROP TABLE <tbName>
// DESC <tbName>
// INSERT INTO <tbName> VALUES <valueLists>
// DELETE FROM <tbName> WHERE <whereClause>
// UPDATE <tbName> SET <setClause> WHERE <whereClause>
// SELECT <selector> FROM <tableList> WHERE <whereClause>
class tbStmt :public Stmt {
public:
    enum Type {
	    TB_CREATE,TB_DROP,TB_DESC,TB_INSERT,TB_DELETE,TB_UPDATE,TB_SELECT,TB_LOAD
    };

    string tbName;
    Type tbType;

    TableInfo* tableInfo;           // only for CREATE TABLE
    AttrInfo* pk;                   // only for CREATE TABLE
    vector<AttrInfo*> fks;          // only for CREATE TABLE

    vector<vector<Value*> > datas;   // valueLists
    Assigns* assigns;                // setClause
    vector<Relation*> relations;    // whereClause
    
    vector<Tcol*>   collist;        // selector
    vector<string> tablelist;       // tablelist

    string filename; //for LOAD

    tbStmt(Type _tbtype): Stmt(Stmt::TB_STMT) {
        tbType = _tbtype;
    };
    ~tbStmt(){};
};


// idxStmt:
// CREATE INDEX <idxName> ON <tbName>’ (’< columnList >’) ’
// DROP INDEX <idxName>
// ALTER TABLE <tbName> ADD INDEX <idxName>’(’<columnList>’)’
// ALTER TABLE <tbName> DROP INDEX <idxName>
class idxStmt :public Stmt {
public:
    enum Type {
	    IDX_CREATE,IDX_DROP,IDX_ALTER_ADD,IDX_ALTER_DROP
    };
    Type idxType;
    string idxName;
    string tbName;
    vector<string> collist;       // columnList

    idxStmt(Type _idxtype): Stmt(Stmt::IDX_STMT) {
        idxType = _idxtype;
    };
    ~idxStmt(){};
};

// alterStmt:
//  ALTER TABLE <tbName> ADD <field>
//  ALTER TABLE <tbName> DROP <colName> 
//  ALTER TABLE <tbName> CHANGE <colName> <filed>
//  ALTER TABLE <tbName> RENAME TO <tbName>
//  ALTER TABLE <tbName> DROP PRIMARY KEY
//  ALTER TABLE <tbName> ADD CONSTRAINT <pkName> PRIMARY KEY’ (’<columnList>’) ’
//  ALTER TABLE <tbName> DROP PRIMARY KEY <pkName>
//  ALTER TABLE <tbName> ADD CONSTRAINT <fkName> FOREIGN KEY ’ (’<columnList>’) ’ REFERENCES <tbName>’ (’<columnList>’) ’
//  ALTER TABLE <tbName> DROP FOREIGN KEY <fkName>
class alterStmt :public Stmt {
public:
    enum Type {
        ALTER_ADD,ALTER_DROP,ALTER_CHANGE,ALTER_RENAME,
        ALTER_DROP_PRIMARY,
        ALTER_ADD_PRIMARY,ALTER_DROP_PRIMARY_WNAME,
        ALTER_ADD_FOREIGN,ALTER_DROP_FOREIGN_WNAME
    };

    Type alterType;
    string tbName;
    string newtbName;
    string colName;
    string pkName;

    string fktbName;
    string fkName;

    vector<string> collist;
    vector<string> fkcollist;
    AttrInfo* field;

    alterStmt(Type _altertype): Stmt(Stmt::ALTER_STMT) {
        alterType = _altertype;
    };
    ~alterStmt(){};
};
