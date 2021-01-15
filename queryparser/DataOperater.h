#pragma once

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include "../sysmgr/SysManager.h"
#include "Stmt.h"
#include <vector>
#include <string>

class DataOperater {
public:
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	SysManager *systemmgr;
	IndexManager *indexmgr;

	DataOperater(SysManager *smm, IndexManager *ixm, FileManager *fileManager, BufPageManager *bufPageManager);
	~DataOperater();
	
    //插入一条数据
	bool Insert(const string tableName, vector<Value*> insertvalues, vector<int> columns,int nullcolumnnub);
	//插入一条数据
	void Update(const Assigns assigns, vector<Relation*> relations);
	void Delete(const string tableName, vector<Relation*> relations);
	void Select(const string tableName, vector<Relation*> relations, vector<string> attrNames);
	void Select(vector<string> tableNames, vector<Tcol*> cols, vector<Relation *> relations);
	void Load(const string tableName,const char* line);

};
