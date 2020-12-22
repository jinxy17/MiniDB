#pragma once
#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include "../sysmgr/SysManager.h"
#include "DataOperater.h"
#include "Stmt.h"

#include <cassert>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
using namespace std;

class Executer {
public:
    Executer();
    ~Executer();

	string currentdb;
	void execSysStmt(sysStmt* stmt);
	void execDbStmt(dbStmt* stmt);
	void execTbStmt(tbStmt* stmt);
	void execIdxStmt(idxStmt* stmt);
	void execAlterStmt(alterStmt* stmt);
    void init();
    void exit();
private:
	const string rootpath = ".MiniDB";
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	IndexManager *indexManager;
	SysManager *smm;
	DataOperater *qlm;
};
