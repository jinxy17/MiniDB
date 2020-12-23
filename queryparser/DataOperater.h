#pragma once

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include "../sysmgr/SysManager.h"
#include <vector>
#include <string>

#define INT 0
#define FLOAT 1
#define STRING 2
#define DATE 3
#define DNULL 4

enum CompOp {
	NO_OP, EQ_OP, NE_OP, LT_OP, GT_OP, LE_OP, GE_OP, IS_NULL, IS_NOT_NULL
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
	vector<BufType> values;
	vector<bool> assignnull;
};

struct Assign {
	string table;
	string attr;
	BufType value;
	bool assignnull;
};

class DataOperater {
public:
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	
	DataOperater(SysManager *smm, IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager);
	~DataOperater();
	
    //插入一条数据
	bool Insert(const string tableName, vector<Value*> insertvalues, vector<int> columns,int nullcolumnnub);
	void Update(const Assign assign, vector<Relation*> relations);
	void Update(const Assigns assigns, vector<Relation*> relations);
	void Delete(const string tableName, vector<Relation*> relations);
	void Select(const string tableName, vector<Relation*> relations, vector<string> attrNames);
	void Select(string tableName1, string tableName2, vector<Relation*> relations, vector<string> attrNames);
	// void Load(const string tableName, const string fileName);

	bool _compare(BufType data1, BufType data2, CompOp op, int type);
private:
	SysManager *_smm;
	IndexManager *_ixm;
	RecManager *_rmm;
};
