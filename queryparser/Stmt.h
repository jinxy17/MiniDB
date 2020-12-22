#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include "DataOperater.h"
#include "../sysmgr/SysManager.h"
using namespace std;

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

struct Lists{
    vector<AttrInfo*> attrinfos;
    vector<BufType> valuelist;
    vector<vector<BufType>> valuelists;
    vector<string> namelist;
    vector<string> collist; 
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
	    TB_CREATE,TB_DROP,TB_DESC,TB_INSERT,TB_DELETE,TB_UPDATE,TB_SELECT
    };

    string tbName;
    Type tbType;

    TableInfo* tableInfo;           // only for CREATE TABLE
    vector<vector<BufType>> datas;          // valueLists
    Assigns assigns;                // setClause
    vector<Relation> relations;     // whereClause
    vector<string> attrs, tables;   // selector
    vector<string> tablelist;       // tablelist

    tbStmt(Type _tbtype): Stmt(Stmt::TB_STMT) {
        tbType = _tbtype;
    };
    ~tbStmt(){};
};

/* TODO: 补全idx和alter */

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
        ALTER_ADD,ALTER_DROP,ALTER_CHANGE,ALTER_RENAME,ALTER_DROP_PRIMARY
    };

    Type alterType;
    string tbName;

    alterStmt(Type _altertype): Stmt(Stmt::ALTER_STMT) {
        alterType = _altertype;
    };
    ~alterStmt(){};
};
